/*   Wireless voltmeter transmitter using ATTINY402 and NRF24L01+.
****************************************************************************************************
     21 Dec Alun Morris

     Optimised for low power (depending on options). Will stop sendng data after a timeout.
     Optional On and Off button s (see below for function) 
     Optionally sends min & max voltages measured during the current On period.
     Sends Vdd (battery?) voltage and warning flags for low voltage states.
     It will turn off when the battery is flat.
     Status is shown via an optional LED.
     
     Measures Positive voltages only but will optionally flag a negative voltage.
     Sends voltage as signed 16 bit, to be converted to float at receiver, to save space here.
     Over-range flag is max +ve val, -ve is max -ve val.
     Timeout to Off after LOOPS_BEFORE_HIBERNATE seconds.
     Processor current (sleep enabled) in Off = hibernate is <2uA for ATTINY 0 or 1 series.
                        NRF24L01+ should take <6uA in hibernate but Chinese clones take a lot more.
                        My samples take 20-30uA.
     Average current during On (one reading per sec, lowest radio power, sleep enabled))
     is ~150uA with my sample of NRF24L01+ ICs.
     If buttons used: On = Start (but1) will restart sending (get one LED flash) 
                            or reset timeout to 0 (no flash) if already on.
                            If battery is flat will send one reading and power off again.
                           Long press (configurable time) will prevent hibernations until Off pressed 
                            or battery flat.
                      Off = Stop (but2). Needs 1s press. Flashes LED 8x (configurable)
                             then hibernates until On pressed
                             
     Compiles for AVR 0 or 1 series eg ATTINY402

     Compile settings:  Use megaTinyCore Board Manager. Brown out detector (BoD) disabled, 
                        Use 1MHz for lowest power (DEBUG must be off - serial baud rate wrong < 5MHz)
     Before flashing you must use the megaTinyCore option to disable the BoD, burn Bootloader 
     (to set fuses*) then power cycle to turn off BoD. CPU will not sleep properly if BoD not 
     disabled. *Ignore error at end re file bootloader.OSCCFG
     To compile for T402/412: DEBUG false, SLEEP true or false.
     Currently just fits in 4KB with all features enabled.
     
     NOTE: If compiled to T402, use megaTinyCore 2.1.2 for correct ADC zero, or later if fixed
****************************************************************************************************
     Copyright (C) 2020 Alun Morris
     This program is free software; you can redistribute it and/or
     modify it under the terms of the GNU General Public License
     version 3 as published by the Free Software Foundation.

  BUGS: none 
*/
#include <SPI.h>
#include "RF24.h"           //for NRF24L01+ radio
#include <avr/sleep.h>
/*
 * Options to set (apart from pins):
 */
#define VERSION 1           //byte
#define SUBVERSION 0        //byte so get version eg 21.17
#define SLEEP true          //use sleep mode to time loop to cut power. Corrupts serial output.
#define SEND_MINMAX true    //track and send min V and max V. Values reset by hibernation
#define LOOPTIME 1          //s. How often to transmit
#define LOOP_TIME_RANDOMISE true    //Option. Loop extended by random 0 to 1023us to help
                                    // avoid Tx collisions
#define LOOPS_BEFORE_HIBERNATE 35   //sec. How long before hibernating. Must be > LONG_PRESS_TIME 
                                    // because we loop rapidly when button held down
#define LONG_PRESS_TIME 32  //in 1/16s
uint8_t channel = 119;      //radio channel - set Rx to same
#define TX_POWER RF24_PA_MIN //RF24_PA_MIN, HIGH or MAX
byte addressOfMe[6] = {"4Chan"}; //change digit for different sender on same channel.
                                 // Only the digit may change due to NRF24L01+ limitation
byte addressOfRx[6] = {"4Disp"}; //digit same as above
#define VDD_CHECK_LOW 308   //Flag low Vdd if read1V1_over_VDD() > than this
                            // Value = 1023 * 1.1V/threshhold V = 3.65V for lithium cell
#define VDD_CHECK_OFFVAL 321  //Hibernate if read1V1_over_VDD() > than this or may damaging battery
                              // Also NFR24L01+ below min supply. V= 3.5V for lithium cell
/*
 * pins. SPI uses default pins
 */
#define CSN_PIN 0
#define CE_PIN 1            //can be shared with LED_PIN and/or NEG_TEST_PIN
#define RF_DATA_RATE RF24_250KBPS //250k, 1Mb or 2Mb 
#define BUTTON_PIN 5        //Optional. Connect 1 or more buttons via resistors. 
                            // Must be an ADC pin. Can be UPDI pin which has ~30k nominal pullup
#define BUTTON2OTHER_PIN 1   //Connects to other side of button2.
                            //Used to distinguish between the 2 buttons. Can be shared with LED_PIN
#define BUTTON_ON_UPDI      //Comment out if buttons not using UPDI pin on AVR series 0,1,2 etc
#define BUTTON_ANALOG_THRESHOLD 921   //when using ADC to read buttons 
                                      // Allows for tolerance of UPDI internal pullup.
#define SENSOR_PIN 3        // Vin. Can be MISO pin since MISO is undriven when CSN is high
                            // MISO is pin 3 on T402, 9 on T1604.
#define LED_PIN 1           //Optional, comment out to disable
                            // Can be shared with CE_PIN and/or NEG_TEST_PIN
#define NEG_TEST_PIN 1      //Optional. Can be shared with CE and/or LED. Low = normal,
                            // high to offset V in by 2 LSB of ADC. If reading still 0: V is -ve
/*
 * constants:
 */
#define HELLO_STR "\n\rRemote Voltage Tx"
#define FULL_SCALE_V 20     //what voltage sending 32767 represents. 255 max
#define MAX_SLEEP_TIME 0x7FFFFFFL  //max long val. L forces it to long. Longer than battery life
                                   // Omitting the L causes bug in function.

//LED flash times, used if LED_PIN defined
#define HELLO_TIME 200      //ms
#define NO_CHIP_TIME 100
#define TX_OK_TIME 0        //keep short or even 0 for low-power use. Flashes for 1ms every Tx
                            // anyway if LED_PIN = CE_PIN but can use capacitor to keep LED off
#define HIBERNATE_TIME 50   //for hibernate on or disabled
#define ON_TIME 50          //On pressed to leave hibernation

/*
 * voltage in, scale and cal
 */
#define ADC_REF_LEVEL INTERNAL1V1 //depends on CPU. Use INTERNAL for MEGA AVR
#define ADC_REF_VDD VDD     //depends on MCU. Use DEFAULT for T328P etc, VDD for AVR 0,1 series
#define FULLSCALE 32767     // of signed int16
#define HALFSCALE 16384     //FULLSCALE/2 rounded up
#define MAX_RANGE_V 32736   //32 * 1023
#define NEGATIVE_FLAG_V -32768 //max -ve  as flag
#define OVER_RANGE_V 32767  //max +ve V
#define SCALE_NOMINAL 8     //=(FULLSCALE+1)/4096 gives Tx 16bit signed int
#define ADC_SETTLE_TIME 100 //us. Wait for ADC cap to charge after V change
                            // Could prob be less, depends on ADC pin input impedance
//flags in status byte sent, as bit masks
#define STAT_VDD_CRITICAL_bm 1
#define STAT_VDD_LOW_bm 2

enum buttonNo {none = 0, but1, but2}; //value of readButtons()
/*
 * Init NFR24L01+
*/
RF24 radio(CE_PIN, CSN_PIN); //CE, CSN. CE not driven if pins are same - tie CE high

/*
* set ADC clock prescaler. Assume ADC clk 1MHz max for general AVR compatibility
* unless CPU clk > 16MHz. AVR 0 & 1 series is 1.5MHz max.
*/
#if F_CPU <= 2000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV2_gc    //divide by 2
#elif F_CPU <= 4000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV4_gc    //divide by 4
#elif F_CPU <= 8000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV8_gc    //divide by 8
#else
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV16_gc   //divide by 16. Gives 1.25Mz at 20MHz
#endif

/**
* Data structure for transmitting
*/
struct dataStruct {
  byte stat = 0;            // status byte
  byte mainVersion;
  byte subVersion;
  int16_t value = 0;        //voltage
  uint8_t fullScaleV;       //set to FULL_SCALE_V
  int16_t valueMin = OVER_RANGE_V;  //voltage, optionally set
  int16_t valueMax = NEGATIVE_FLAG_V; //voltage, optionally set
  int16_t vddValue;         //not same scale as value, to save floating point calc in sender
  char unused[1];           //pad for future compatibility
} myData;                   //global

int loopTimeRandomised = LOOPTIME; //add an offset later
/*
* prototypes
*/
int16_t checkVdd();
int16_t read1V1_over_VDD(void);
buttonNo readButtons(void);
buttonNo delayOrSleep(uint32_t sec);
void calibrate();
int16_t readVoltage(void);
void flashLED(uint16_t durationH, uint16_t durationL, uint8_t times);
void maketone(unsigned int freq, unsigned long toneDuration, unsigned long silenceDuration);
void RTC_init(void);
ISR(RTC_PIT_vect);

void setup() {

/*
 * set versin info in transmit packet
 */
  myData.mainVersion = VERSION;
  myData.subVersion = SUBVERSION;

  //    sleep mode

#if SLEEP == true
  RTC_init();
  set_sleep_mode(SLEEP_MODE_STANDBY); //PWR_DOWN saving negligible for AVR 0,1 series over STANDBY
  sleep_enable();
#endif

#if defined BUTTON_PIN
  pinMode(BUTTON_PIN, INPUT);
#if !defined BUTTON_ON_UPDI
  pinMode(BUTTONS_PIN, INPUT_PULLUP);
#endif
#endif

  flashLED(HELLO_TIME, HELLO_TIME, 3) ;

  myData.fullScaleV = FULL_SCALE_V;

  radio.begin();

  if (!radio.RF24::isChipConnected()) {
    flashLED(NO_CHIP_TIME, NO_CHIP_TIME, 20) ;
    while (true) {          //stop
      sleep_cpu();          //low power
    }
  }
  radio.setPALevel(TX_POWER);
  radio.setDataRate(RF_DATA_RATE);
/*
 * Open a writing and reading pipe on each radio, with opposite addresses
 */
  radio.openWritingPipe(addressOfRx);
  radio.openReadingPipe(1, addressOfMe);
/*
 * Start the radio listening for data
 */
  radio.setChannel(channel);
}

void loop() {
  int16_t volts = 0; //-32768 to 32767
  bool tx_ok = true;
  static long loopCtr = 0 ;
  static unsigned int button1Time = 0;   //how long Start button has been pressed, in 1/16s
  static bool hibernateOK = true;        //will hibernate after timeout if true
  static buttonNo button = none;         //enum. Which button if any
/*
 * Transmit
 */

  volts = readVoltage();    //50% of max V = SCALE_NOMINAL * ADC_NOMINAL_HALF_SCALE.  Must not be 
                            // immediately after radio disabled with CE, in case SENSEPIN=CE_PIN
#if defined NEG_TEST_PIN
  if (volts == 0) {         //could be because Vin is negative
    digitalWrite(NEG_TEST_PIN, HIGH); //apply tiny offset to ADC input pin
    delayMicroseconds(ADC_SETTLE_TIME);
    if (readVoltage() == 0) {
      volts = NEGATIVE_FLAG_V;
    }
    digitalWrite(NEG_TEST_PIN, HIGH); //back to normal
  }
#endif
  myData.value = volts;

  if (volts == MAX_RANGE_V) {
    myData.value = OVER_RANGE_V; //flag over-range
  }
#if SEND_MINMAX
  myData.valueMin = min(myData.valueMin, myData.value);
  myData.valueMax = max(myData.valueMax, myData.value);
#endif
/*
 * check Vdd every 256 loops or every time wake from hibernate if sooner
 */
  if ((loopCtr++ & 0xFF) == 0) {
    checkVdd(); //sets myData.vdddValue and maybe flags in myData.stat
  }
  tx_ok = radio.write( &myData, sizeof(myData)); //send packet;
  if (tx_ok) {
  }
/*
 * Rest of loop handles loop delay/sleep or hibernate (ie Off) and Vdd too low.
 * Will not reach end of loop until done delay/sleep or Start button hit
 * Note button was read near end of loop by delayOrSleep()
 */
  button1Time = (button == but1) ? button1Time + 1: 0; //count how long button 1 down

  if (button1Time >= LONG_PRESS_TIME) { //disable hibernation. While Start is pressed
                                        // delayOrSleep() returns in 1/16s
    button1Time = 0;        //reset ctr
    hibernateOK = false;
    flashLED(HIBERNATE_TIME, 2 * HIBERNATE_TIME, 4); //signal never hibernate
  }
/*
 *Test for Off button or timeout or critically low Vdd. Turn off if true.
 */
  if (  button == but2 
     || (loopCtr >= LOOPS_BEFORE_HIBERNATE && hibernateOK)
     || (myData.stat & STAT_VDD_CRITICAL_bm)
     ) {
    flashLED(HIBERNATE_TIME, 2 * HIBERNATE_TIME, 8);  //signal hibernating
    button = delayOrSleep(MAX_SLEEP_TIME) ;           //good as infinity.
                                                      // Exits within 1/16s when On button hit
/*
 * wakes up - must be because On pressed
 */
    hibernateOK = true;     //here on start button pressed, so restart
    flashLED(ON_TIME, 0, 1); //signal On pressed to leave hibernation
    loopCtr = 0;
    button1Time = 0;
#if SEND_MINMAX
    myData.valueMin = OVER_RANGE_V; //reset min and max
    myData.valueMax = NEGATIVE_FLAG_V;
#endif
  }
  else {                    //keep sending
    button = delayOrSleep(LOOPTIME);  //will exit within 1/16s if a button hit
    if (button == but1) {
      loopCtr = 0;          //reset timeout
    }
  }
#if LOOP_TIME_RANDOMISE
  delayMicroseconds((unsigned int)ADC0.RES);  //last ADC reg value: random between multiple senders.
                                              //Small and fast.
#endif
} // Loop. button is set on exit from delayOrSleep()

int16_t checkVdd() {        //may alter myData.status
  int16_t value = read1V1_over_VDD();
  
  myData.vddValue = value;
/*
 * set status flags(s) if Vdd low
 */
  if (value > VDD_CHECK_LOW) {
    myData.stat |= STAT_VDD_LOW_bm;   //set
  }
  else {
    myData.stat &=  ~STAT_VDD_LOW_bm; //clear
  }
  if (value > VDD_CHECK_OFFVAL) {
    myData.stat |= STAT_VDD_CRITICAL_bm;  //set
  }
  else {
    myData.stat &= ~STAT_VDD_CRITICAL_bm; //clear
  }
  return value;
}
int16_t read1V1_over_VDD(void) {     //returns 10-bit value, the ADC output. Assumes Vref is 1.1V
/*
 * Code mostly from Atmel App Note AN2447. Changes Vref to 1.1V
 */
  int16_t adc_output = 0;

  ADC0.MUXPOS = ADC_MUXPOS_INTREF_gc;   //ADC internal reference, the Vbg

  ADC0.CTRLC = ADC_PRESC_DIV_gc         //CLK_PER divider, set by macro according to CPU clock
               | ADC_REFSEL_VDDREF_gc   //Vdd (Vcc) be ADC reference/
               | 0 << ADC_SAMPCAP_bp;   //Sample Capacitance Selection: disabled/
  ADC0.CTRLA = 1 << ADC_ENABLE_bp       //ADC Enable: enabled
               | 0 << ADC_FREERUN_bp    //ADC Free run mode: disabled
                                        // Mod from enabled since we test ADC ready
               | ADC_RESSEL_10BIT_gc;   //10-bit mode
  ADC0.COMMAND |= 1;                    // start running ADC
  delayMicroseconds(ADC_SETTLE_TIME);   //allow voltage to settle.
  while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {  //wait for ADC ready. From Atmel TB3209.
                                              // The very first value is too low if this not here 
                                              // even though waited long enough
  }
  adc_output = ADC0.RES ;               //ADC output
/*
 * restore ADC to normal: easiest and most robust method is a dummy analogRead()
 */
  analogRead(SENSOR_PIN);               //get wrong values on next 1/2 (?) analogRead(pin)
  return adc_output;
}
buttonNo readButtons(void) {
/*use ADC to read buttons if using UPDI as input. 
 * Changes ADC Vref. BUTTON2OTHER_PIN (which may be LED_PIN) must be low on entry since this is used to distinguish 2 buttons
 */
  buttonNo button = none;
#if defined BUTTON_PIN
  changeAnalogRef(ADC_REF_VDD, BUTTON_PIN); //Not fastest method but smaller and robust.

  if ((analogRead(BUTTON_PIN) < BUTTON_ANALOG_THRESHOLD)) { //Does not give ADC settling time problem
    digitalWrite(BUTTON2OTHER_PIN, HIGH);  //test for Button 1 or 2
    if (analogRead(BUTTON_PIN) < BUTTON_ANALOG_THRESHOLD) {
      button = but1;              //if it stays detected
    }
    else {
      button = but2;
    }
    digitalWrite(BUTTON2OTHER_PIN, LOW);   //restore
  }
#endif
  return button;
}
buttonNo delayOrSleep(uint32_t sec) {
/*
 * polls start button during delay or sleep. Return state of buttons
 */
  buttonNo button = none;
  sec *= 16;                //conv to 1/16s. sleep_cpu can only be set to 1/2^n  sec
  while (sec-- > 0) {
#if SLEEP == true
    sleep_cpu();            //sleep until interrupt = RTC (set to 1/16 sec)/
#else
    delay(62);              //1/16 s
#endif
#if defined BUTTON_PIN
    button = readButtons();
    if (button == but1) {   //On but
      sec = 0;              //force end of sleep
    }
#endif
  }
  return button;            //or none if BUTTON_PIN undefined
}
int16_t readVoltage(void) {
/*
 * Does 4 reads to cut noise & imrove res maybe. Returns 0 to FULLSCALE
 */
  int16_t value = 0;
  byte i = 4;
  
  changeAnalogRef(ADC_REF_LEVEL, SENSOR_PIN); //depends on MCU. Not fastest but smaller and robust.

  while (i-- > 0) {         //sum 4 reads
    value += analogRead(SENSOR_PIN);
  }
  value = (long)(value * SCALE_NOMINAL);
  return value;
}
void changeAnalogRef(uint8_t ref, uint8_t pinNo) {
/*
 * Using this prevents bad ADC reads after changing ref and/or analog pin
 */
  analogReference(ref);     //depends on MCU. Not fastest but smaller and robust.
  analogRead(pinNo);        //next reading may be wrong after changing ref so discard
  delayMicroseconds(ADC_SETTLE_TIME);   //in case pin has changed
}
void flashLED(uint16_t durationH, uint16_t durationL, uint8_t times) {
/*
 * Flash LED_PIN. LED_PIN may be an input pin also, so set back to input at end
 * Do nothing if LED_PIN undefined
 */
#if defined(LED_PIN)
  if (durationH > 0) {
    while (times-- > 0) {
      digitalWrite(LED_PIN, HIGH) ;
      delay(durationH);
      digitalWrite(LED_PIN, LOW) ;

      delay(durationL);
    }
  }
#endif
}
void RTC_init(void) {
/*
 * code for AVR 0 and 1 series only. 
 * From https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/PowerSave.md
 */

  while (RTC.STATUS > 0)    //Initialize RTC
  {
    ;                       //Wait for all register to be synchronized
  }
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc; //32.768kHz Ultra-Low-Power Oscillator (OSCULP32K)

  RTC.PITINTCTRL = RTC_PI_bm; //PIT Interrupt: enabled

  RTC.PITCTRLA = RTC_PERIOD_CYC2048_gc  //RTC Clock Cycles, resulting in 32.768kHz/2048 = 
                                        //16Hz or a 1/16s period*/
                 | RTC_PITEN_bm;        //Enable PIT counter: enabled
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;  //Clear interrupt flag by writing '1' (required)
}
