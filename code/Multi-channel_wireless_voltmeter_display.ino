/*   Multi-channel wireless voltmeter display using NRF24L01+ and 128 x 32 I2C OLED.
****************************************************************************************************
     26 Nov 2020 Alun Morris

     Receives voltage data from senders and displays them (up to 4) on a 0.91" OLED display.
     The Mode button rotates around display modes:
     1. Summary display (4 channels): Row 1: '1 vv.vv*?2 nn.nn*?
                                      Row 2: '3 vv.vv*?4 nn.nn*?
       where vv.vv = voltage. *=battery alert, ? = timeout AKA lost signal alert. 16 pixel rows
       Over voltage shows as -----, under voltage as -0.00, no data is just blank

     2. Single channel: Row 1: N vv.vv *   Min
                        Row 2:             mm.mm
                        Row 3:         ?   Max
                        Row 4:  Bat b.bb   mm.mm

       where N=channel, nn.nn = volatage,, mm.mm= min or max voltage, 
       b.bb=sender's Vdd (bat) voltage (appears for a few seconds only to reduce clutter)
       Rows are 8 pixels high in Single view, though N and vv.vv are taller.

      A customised font file is used with battery state and signal timeout chars
      Tested on Arduino nano and Auduino IDE 1.8.13
      
      BUGS: none
**************************************************************************************************
 Copyright (C) 2020 Alun Morris
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 3 as published by the Free Software Foundation.
*/
#include <SPI.h>
#include "RF24.h"
#include <Wire.h>                   //for I2C
#include "SSD1306Ascii.h"           //no graphics library for small size
#include "SSD1306AsciiWire.h"
#include "MyFonts.h"                //modified X11fixed7x14B font with low battery chars 126, 127
                                    // and lost signal char 96

/*
 * Options to set
 */
#define LOST_INPUT_TIMEOUT  5000    //in ms
#define NOISE_FILTER true           //reduce jitter in diplayed voltage
#define SHOW_VDD true               //display Vdd in single view?
#define VDD_DISPLAY_DURATION 3000   //ms. Time to display Vdd in single view
#define BUTTON_PIN 3                //input - needs internal pullup
#define LOOP_TIME 100               //ms. How often to poll receiver and update display
#define CHANNELS 4                  //no of voltage channels shown. Limited by 128x32 display
#define HELLO_STR "Voltage monitor "
#define VERSION_STR "0.6"
#define TX_POWER RF24_PA_MIN        //use _MIN, _HIGH or _MAX
const uint8_t RadioChannel = 119;   //0-124. UK Wifi does not use higher channels.

#define STAT_VDD_CRITICAL_bm 1 << 0
#define STAT_VDD_LOW_bm 1 << 1
#define STAT_NOINPUT (uint16_t) 0xFFFF  //means no input yet. Set by receiver
#define FULL_SCALE 32768            //received int, not sender's ADC
#define OVER_RANGE_VAL 32767        //max +ve val
#define NEGATIVE_V_VAL -32768       //max -ve val
int16_t NoiseTolerance = 32;        //one LSB if 10-bit ADC in transmitter
                                    //pipes for each Tx. Addrs can only differ in firstAddrs can
                                    // only differ in first char due to NRF24L01+ limitation
byte addressOfSender[][6] = {"1Sndr", "2Sndr", "3Sndr", "4Sndr", "5Sndr"};
byte addressOfMe[][6]     = {"1Disp", "2Disp", "3Disp", "4Disp", "5Disp"}; //display chan in each pipe
const float VddReference  = 1.1;     //used to calc Vdd = SenderADCfullScale*VddReference/vddValue
const int SenderADCfullScale = 1023; //used to calc Vdd
#define SUMMARY 0                   //display mode that shows all channels. Other modes are channel
                                    //no 1 to CHANNELS which show a single channel
/* 
 * Hardware configuration: nRF24L01+ uses SPI pins + the two below
 */
RF24 radio(10, 8);   //CE, CSN
/*
 * OLED setup and screen formats
 */
#define I2C_ADDRESS 0x3C            //OLED: 0X3C+SA0 - 0x3C or 0x3D
SSD1306AsciiWire oled;
#define MAIN_FONT   X11fixed7x14B_with_low_battery
#define SMALL_FONT  Adafruit5x7
#define MEDIUM_NUMBERS_FONT lcdnums14x24
const char LostSignal = 96;     //custome font character nos
const char BatteryEmpty = 126;
const char BatteryLow = 127;
/*
 * text positions on oled: Summary view
 */
const uint8_t  Vcol[] = { 0, 7 * 9 + 2, 0, 7 * 9 + 2} ; //pixels to print fields for each channel
                                                        //7x14 font + 2 pels for 2nd col
const uint8_t  Vrow[] = { 0, 0, 2, 2} ;                 //8 pix units
/*
 * text positions on oled: Single channel view
 */
const uint8_t  MinTextCursor[2]             = {97, 0};  //rows in pixels, cols in 8 pixels units
const uint8_t  MinCursor[2]                 = {97, 1};
const uint8_t  MaxTextCursor[2]             = {97, 2};
const uint8_t  MaxCursor[2]                 = {97, 3};
const uint8_t  VsingleViewCursor[2]         = {14, 0};
const uint8_t  Warning1SingleViewCursor[2]  = {88, 0};
const uint8_t  Warning2SingleViewCursor[2]  = {88, 2};
const uint8_t  VddTextCursor[2]             = {14, 3};

char MinStr[] = "Min";              //strings for oled
char MaxStr[] = "Max";
char VddStr[] = "Bat ";
/*
 * received data packet
 */
struct dataStruct {
  byte stat;                        //status flagsf
  byte mainVersion;                 //of transmitter
  byte subVersion;
  int16_t value;
  uint8_t fullScaleV;               //what 1024 represents
  int16_t valueMin;
  int16_t valueMax;
  int16_t vddValue;                 //not same scale as value, to save doing division on sender
  char unused[1];                   //padding for compatibility
} myData;

//macros
/*
 * prototypes
 */
  void oledStartScreens(void);
  void oledDisplay(byte, uint8_t, int16_t[], int16_t[], int16_t[], int16_t[], int16_t[], 
                    uint16_t[], bool[], unsigned long);
  void oledPrintV(int16_t);
  void oledDisplaySummaryBackground(void);
  void oledPrintAt(uint8_t, uint8_t, char *, bool);
  void filterNoise(uint8_t, int16_t[], int16_t[]);
  bool buttonPressed(void);
/*
 **********************************
 */
void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP) ;
/*                                                            
 * OLED  & SPI
 */
  Wire.begin();
  Wire.setClock(400000L);
  
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(MAIN_FONT);
  oledStartScreens();
  
  radio.begin();

  if (!radio.RF24::isChipConnected()) {
    delay(4000); //time to read prev text on display
    oled.clear();
    oled.print("Error:no RF module");
    while (true) {                  //halt
    }
  }
  radio.setPALevel(TX_POWER);
  radio.setDataRate(RF24_250KBPS);  //lowest
/*
 * Open a writing and reading pipe on each radio, with opposite addresses
 */
  for (byte i = 0; i <= CHANNELS; ++i) {
    radio.openReadingPipe(i + 1, addressOfMe[i]);
  }
  myData.value = 0;
/*
 *Start the radio listening for data
 */
  radio.setChannel(RadioChannel);
  radio.startListening();
  delay(2000);                      //time to read last text
  oled.clear();
  oledDisplaySummaryBackground();
}
/*
 **********************************
 */
void loop() {
  static byte displayMode = SUMMARY;
  uint8_t pipenum = 0;
  static unsigned long timeOfLastDisplayModeChange = millis(); //to allow view to change over time
  bool lostInput[5] = {false, false, false, false, false};  //lost input. use pipnum as index:
                                                            // index 0 not used
  static int16_t receivedValue[5] = {0, 0, 0, 0, 0};  //use pipnum as index: index 0 not used
  static int16_t minV[5] = {0, 0, 0, 0, 0};
  static int16_t maxV[5] = {0, 0, 0, 0, 0};
  static int16_t vddVal[5] = {1, 1, 1, 1, 1};
  static int16_t displayedValue[5] = {0, 0, 0, 0, 0}; //for noise filter. Use pipnum as index: 
                                                      // index 0 not used
                                                      //STAT_NOINPUT means no input yet on that chan
                                                      // use pipnum as index: index 0 not used
  static uint16_t stat[5] = {STAT_NOINPUT, STAT_NOINPUT, STAT_NOINPUT, STAT_NOINPUT, STAT_NOINPUT};
  static unsigned long lastInputTime[5] = {0, 0, 0, 0, 0}; //CPU ms since last input
  int i = 0;

  if (buttonPressed()) {            //loop round modes: 0 to CHANNELS
    displayMode = (displayMode == CHANNELS) ? SUMMARY : displayMode + 1; 
    oled.clear();                   //always clear on mode change
    timeOfLastDisplayModeChange = millis();
    if (displayMode == SUMMARY) {   //back to summary
      oledDisplaySummaryBackground();
    }
  }
  if (radio.available()) {
    while (radio.available(&pipenum)) {       // While there is data ready. pipenum - channel 1, 2...
      radio.read( &myData, sizeof(myData) );  // Get packet for each chanel with new data
      receivedValue[pipenum] = myData.value;  //update values for this channel
      minV[pipenum] = myData.valueMin;
      maxV[pipenum] = myData.valueMax;
      vddVal[pipenum] = myData.vddValue;      //not same scale as value
      stat[pipenum] = (uint16_t) myData.stat; //also flags no input yet on this channel
      lastInputTime[pipenum] = millis();
    }

  }
/*
 * check for channels that have not had an update in LOST_INPUT_TIMEOUT ms
 */
  for (i = 1 ; i <= CHANNELS; ++i) {  //lastInputTime is 0 if never had a value
    if (lastInputTime[i] != 0 && (millis() - lastInputTime[i]) > LOST_INPUT_TIMEOUT) {
      lostInput[i] = true;
    }
  }
 /*
  * Always update diaplay. The logic for testing if there was something new or a timeout or Vdd needed
  * blanking in single view was too messy
 */
  oledDisplay(displayMode, pipenum, receivedValue, displayedValue, minV, maxV, vddVal, stat, 
              lostInput, timeOfLastDisplayModeChange);
  delay(LOOP_TIME) ;                //don't need to poll furiously
} // Loop
/*
 ************************************ functions
 */
void oledStartScreens(void) {
  oled.clear();
  oled.setInvertMode(true);         //black on whte
  oled.println(F(HELLO_STR));
  oled.setInvertMode(false);        //white on black
  oled.print(F("V:")); 
  oled.print(F(VERSION_STR));
  oled.print(F(" Timeout="));
  oled.print(LOST_INPUT_TIMEOUT / 1000);
  oled.write('s');
  delay(3000);                      //allow time to read that
  oled.setCol(0);
  oled.clearToEOL();
  oled.print(F("Radio channel "));
  oled.print(RadioChannel);
}
void oledDisplay(byte displayMode, uint8_t pipenum, int16_t receivedValue[], int16_t displayedValue[],
                 int16_t minV[], int16_t maxV[], int16_t vddVal[], uint16_t stat[], bool lostInput[],
                 unsigned long timeOfLastDisplayModeChange) {
/*
 * print whole display - won't flicker as long as we don't blank a section before overwriting
 */
  char floatStr[6] = "-----" ; //to hold voltage                                                                       
  float v = 0.0;

  if (displayMode == SUMMARY) {
    for (pipenum = 1; pipenum <= CHANNELS; ++pipenum ) {    //check each channel
      if (stat[pipenum] != STAT_NOINPUT) {                  //if channel is live
        oled.setCol(Vcol[pipenum - 1]);
        oled.setInvertMode(true) ; //print channel number inverted
        oled.setRow(Vrow[pipenum - 1]);
        oled.print(pipenum);   //channel no
        oled.setInvertMode(false) ;
/*
 * stop the voltage dithering between adjacent values
 */
        filterNoise(pipenum, displayedValue, receivedValue);
        oled.print(' ');
        oledPrintV(receivedValue[pipenum]);
        if (stat[pipenum] & STAT_VDD_CRITICAL_bm) {   //Vdd low flag will be on as well in this case,
                                                      // so test Vdd critical first
          oled.print(BatteryEmpty);
        }
        else if (stat[pipenum] & STAT_VDD_LOW_bm) {
          oled.print(BatteryLow);
        }
        else {
          oled.print(' ');  //clear field
        }
        if (lostInput[pipenum]) {     // mark if not had an update in too long
          oled.print(LostSignal);
        }
        else {
          oled.print(' ');            //clear field
        }
      }//if had input
    }//for all channels
  }//if Summary
/*
 * single channel view
 */
 else {
    oled.home();
    oled.setInvertMode(true);
    oled.print(displayMode);        //channel no
    oled.setInvertMode(false);
    
    if (stat[displayMode] != STAT_NOINPUT) {                  //if channel is live
      filterNoise(pipenum, displayedValue, receivedValue);
      oled.setFont(MEDIUM_NUMBERS_FONT);                      //may have only digita . and -
      oled.setCursor(VsingleViewCursor[0], VsingleViewCursor[1]);
      oledPrintV(receivedValue[displayMode]);
      /*do warnings*/
      oled.setFont(MAIN_FONT);
      oled.setCursor(Warning1SingleViewCursor[0], Warning1SingleViewCursor[1]);
      if (stat[displayMode] & STAT_VDD_CRITICAL_bm) {         //Vdd low flag will be on as well in
                                                              // this case, so test Vdd critical first
        oled.print(BatteryEmpty);
      }
      else if (stat[displayMode] & STAT_VDD_LOW_bm) {
        oled.print(BatteryLow);
      }
      else {
        oled.print(' ');  //clear field
      }
      oled.setCursor(Warning2SingleViewCursor[0], Warning2SingleViewCursor[1]);
      if (lostInput[displayMode]) {                           // mark if not had an update in too long
        oled.print(LostSignal);
      }
      else {
        oled.print(' ');                                      //clear field
      }
      /*print Vdd, min and max */
      oled.setFont(SMALL_FONT);
#if SHOW_VDD
      if ((vddVal[displayMode] != 0)                                              //valid vddVal
          && (((millis() - timeOfLastDisplayModeChange) < VDD_DISPLAY_DURATION)   //a short time
              || (stat[displayMode] & STAT_VDD_CRITICAL_bm)                       //or Vdd critical
              || (stat[displayMode] & STAT_VDD_LOW_bm)                            //or Vdd low
             )
         ) {
        oledPrintAt(VddTextCursor[0], VddTextCursor[1], VddStr, false);
        v = (float) SenderADCfullScale * VddReference / vddVal[displayMode];
        dtostrf(v, 4, 2, floatStr); // format to 4 chars tot, 2 dp:
        oled.print(floatStr);
      }
      else {
        oled.clearField(VddTextCursor[0], VddTextCursor[1], 8); //clear text when no longer displayed
      }
#endif

      oledPrintAt(MinTextCursor[0], MinTextCursor[1], MinStr, false);
      oled.setCursor(MinCursor[0], MinCursor[1]);
      oledPrintV(minV[displayMode]);
      oledPrintAt(MaxTextCursor[0], MaxTextCursor[1], MaxStr, false);
      oled.setCursor(MaxCursor[0], MaxCursor[1]);
      oledPrintV(maxV[displayMode]);
    }//if chan live
    oled.setFont(MAIN_FONT);
  }//else single chan view
}
void oledPrintV(int16_t value) {
/*
 * print value at oled cursor as 5 chars
 */
  char floatStr[6] ;  //to hold print string
  float v = 0.0;

  if (value == OVER_RANGE_VAL) {
    oled.print(F("1----"));
  }
  else if (value == NEGATIVE_V_VAL) {
    oled.print(F("-0.00"));
  }
  else {
    v = (float) value * myData.fullScaleV / FULL_SCALE ;
    dtostrf(v, 5, 2, floatStr);       //eg '12.34' or ' 9.93'
    if (floatStr[0] == ' ') {
      floatStr[0] = '0';              //replace leading space with 0 in case font has no space char
    }
    oled.print(floatStr);
  }
}
void filterNoise(uint8_t pipenum, int16_t displayedValue[], int16_t receivedValue[]) {
/*
 * optionally prevent diplay V dithering between two adjacent values
 */
#if NOISE_FILTER
        if (abs(displayedValue[pipenum] - receivedValue[pipenum]) <= NoiseTolerance) {
          receivedValue[pipenum] = displayedValue[pipenum]; //don't change display
        }
        else {
          displayedValue[pipenum] = receivedValue[pipenum]; //do change and update diaplayedValue
        }
#endif
}
void oledDisplaySummaryBackground(void) {
/*
 * shows channel nos as inverted digits. Exits with invert off
 */
  oled.setInvertMode(true) ;
  for (byte i = 1  ; i <= CHANNELS ; ++i) {  //background = just 1 to 4
    oled.setCursor(Vcol[i - 1], Vrow[i - 1]);
    oled.print(i);          //channel no
  }
  oled.setInvertMode(false) ;
}
void oledPrintAt(uint8_t x, uint8_t y, char * str, bool invtMode) {
/*
 * y is in units of 8 pixels. Exits with invesr mode off
 */
  oled.setCursor(x, y);
  oled.setInvertMode(invtMode);
  oled.print(str);
  oled.setInvertMode(false);
}
bool buttonPressed(void) {
/*
 * Return true if button gone down since last call. Next call returns false even if button still down
 * Debounced by virtue of delay between button sampling
 */
  static bool previousButtonState = HIGH; //HIGH = not pressed
  bool returnValue = false;

  if (digitalRead(BUTTON_PIN) == HIGH) {  //button up
    returnValue = false;
    previousButtonState = HIGH;
  }
  else if (previousButtonState == HIGH) { //button gone from up to down
    returnValue = true;
    previousButtonState = LOW;
  }
  else {
    returnValue = false;
    previousButtonState = LOW;
  }
  return returnValue;
}
