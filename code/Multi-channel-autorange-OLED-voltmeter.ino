/* Multi-channel-autorange-OLED-voltmeter
****************************************************************************************************
    21 May 2024 Alun Morris ver 1.0.0

    4 channel +ve only auto-range voltmeter OLED 72x40 I2C
    Will read each channel up to 6 times, from highest sensitivity (550mV) to lowest (~27.4V)
    The first ADC reading to not overflow (ie 1023 at H/W level) is used.
    Normal range input has protection resistor 1MΩ to V1
    High range input has 1MΩ to V1, 100kΩ to 0V giving a 1/11 divider
    Max input range just over 27V, displays ++++ above this.
    Optional hysteresis to prevent voise making the numbers jiggle about

  COMPILE OPTIONS
    Set CPU clock to 10MHz if running ATTINY16XX at 3.3V
    Set BOD level to 2.6V
  BUGS
    Can hunt between normal/high ranges if voltage is near max for normal range 
    - displayed val jumps about.
****************************************************************************************************
    This program is free software; you can redistribute it and/or modify it under the terms of the 
    GNU General Public License version 3 as published by the Free Software Foundation.
*/
/*345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <stdlib.h>               //for ltoa()
/*
 * pins. SPI, DAC 0 use default pins
 */
#define V1_PIN 0
#define V2_PIN 1
#define V3_PIN 2
#define V4_PIN 3
#define HIGH_RANGE1_PIN 10                //high range control for chan 1
#define HIGH_RANGE2_PIN 9
#define HIGH_RANGE3_PIN 8
#define HIGH_RANGE4_PIN 4
#define UNUSED_PIN 5
/*
 * constants:
 *
 * configuration options
 */
#define HYSTERESIS_ON                    //comment out if unwanted
#define HYSTERESIS_AMOUNT 3              //ignore ADC values that change by up to this
#define LOOP_DELAY 100                   //display update interval, ms
/*
 * ADC:
 */
#define ADC_REF_LEVEL_0V55 INTERNAL0V55   //depends on CPU. Use INTERNAL for MEGA AVR
#define ADC_REF_LEVEL_1V1 INTERNAL1V1     //depends on CPU. Use INTERNAL for MEGA AVR
#define ADC_REF_LEVEL_2V5 INTERNAL2V5     //depends on CPU. Use INTERNAL for MEGA AVR
#define ADC_REF_MV_0v55 550               //in mV
#define ADC_REF_MV_1V1 1100               //in mV
#define ADC_REF_MV_2V5 2500               //in mV
#define ADC_MV_FOR_1024_0V55 551          //mV in if ADC reads 1024. Speeds calulation
#define ADC_MV_FOR_1024_1V1 1101          //mV in if ADC reads 1024
#define ADC_MV_FOR_1024_2V5 2502          //mV in if ADC reads 1024
#define NO_OF_ADC_READS 16                //sum this no of reads
#define HIGH_RANGE_RATIO 11               //higher than normal range
const pin_size_t ChannelPin[5] = {0, V1_PIN, V2_PIN, V3_PIN, V4_PIN}; //index is channel no
const pin_size_t HighRangePin[5] = {0, HIGH_RANGE1_PIN, HIGH_RANGE2_PIN, 
                                    HIGH_RANGE3_PIN, HIGH_RANGE4_PIN};
const unsigned int HighRangeCal[5] = {0, 1024, 1024, 1024, 1024}; //Change to cal high range
#define OVER_RANGE_ADC 1023
#define ADC_SETTLE_TIME 100               //us. Wait for ADC cap to charge after V change
                                          //Could prob be less, depends on ADC pin input cap
#define OVERRANGE 1000000000              //flags overrange when value = this
/*
* set ADC clock prescaler. Assume ADC clk 1MHz max for general AVR compatibility
* unless CPU clk > 16MHz. AVR 0 & 1 series is 1.5MHz max.
*/
#if F_CPU <= 2000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV2_gc  //divide by 2
#elif F_CPU <= 4000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV4_gc  //divide by 4
#elif F_CPU <= 8000000
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV8_gc  //divide by 8
#else
#define ADC_PRESC_DIV_gc ADC_PRESC_DIV16_gc //divide by 16. Gives 1.25Mz at 20MHz
#endif
/*
 * logic levels
 */
#define LOGIC_L_mV 800                      //max VL for 3.3V & 5V logic (except 74HC, AHC, AC)
#define LOGIC_H_mV 2000                     //min VH
/*
* OLED driver
*/
#define U8X8_HAVE_HW_I2C
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
/*
 * Globals
 */
/* 
 * Hardware configuration
 */

U8G2_SSD1306_72X40_ER_1_HW_I2C u8g2(U8G2_R1, /* reset=*/ U8X8_PIN_NONE);   // 0.42" OLED, portrait mode
/*
* prototypes
*/
char * mVtoStr(long mV, char* vStr) ;
long readVoltage(unsigned char channel);
long readADCmV(unsigned char channel, bool range);
long readADC(unsigned char channel);
void changeAnalogRef(uint8_t ref, unsigned char channel);

void setup(void) {
int lineHeight = 10;                         //for OLED printing

  pinMode(UNUSED_PIN, INPUT_PULLUP);

  changeAnalogRef(ADC_REF_LEVEL_0V55, V1_PIN); //depends on MCU. Not fastest but smaller and robust
/*
 *  OLED
 */                   
  u8g2.begin();  
//  print title/version & set background colour
  u8g2.setContrast(255);                      //in case OLED defaults to low brightnes 
  u8g2.setFont( u8g2_font_t0_11_tr);          //small font for splash page
  u8g2.firstPage();
  do {
    u8g2.drawStr(0,    lineHeight - 2,"4 chan");
    u8g2.drawStr(0, 2* lineHeight - 2,"auto-");
    u8g2.drawStr(0, 3* lineHeight - 2,"range");
    u8g2.drawStr(0, 4* lineHeight - 2,"meter");
    u8g2.drawStr(0, 5* lineHeight - 2,"0-25V");
    u8g2.drawStr(0, 6* lineHeight - 2,"ver:");
    u8g2.drawStr(0, 7* lineHeight - 2,"1.0.0");
  } while (u8g2.nextPage());
 delay(2000);
                                              //display format help screen
  lineHeight = 16;
   u8g2.firstPage();
   do {
                                              //now print
    u8g2.setFont(u8g2_font_10x20_mn);         //Font for voltages. 5 chars/row. Numeric only
    u8g2.drawStr(0,    lineHeight - 2, "1");
    u8g2.drawStr(0, 2* lineHeight - 2, "2");
    u8g2.drawStr(0, 3* lineHeight - 2, "3");
    u8g2.drawStr(0, 4* lineHeight - 2, "4");
    u8g2.setFont( u8g2_font_t0_11_tr);         //small font
    u8g2.drawStr(3,72, "<1234>");              //1 pel below bottom of OLED. OK if no descenders.
  } while (u8g2.nextPage() );
  delay(2000);
}

void loop(void) {
  char voltageStr[5][7];                       //index is channel
  char logicStr[7] = "<....>", tempStr[7];        
  long voltage_mV[5] = {0};                    //index is channel
  int i;
  const int lineHeight = 16;

 
  for (i=1; i<=4; i++) {
  voltage_mV[i] = readVoltage(i);
  if (voltage_mV[i] == OVERRANGE) {
    strcpy(voltageStr[i], "++++");
  } else {
    strcpy(voltageStr[i], mVtoStr(voltage_mV[i], tempStr)); //convert
  }
  if (voltage_mV[i] < LOGIC_L_mV) {             //convert V to logic levels
    logicStr[i] = '_';
  } else if (voltage_mV[i] > LOGIC_H_mV) {
    logicStr[i] = '-';
  } else {
    logicStr[i] = '*';                          //indeterminate
  }
}
  u8g2.setDrawColor(1);
  u8g2.firstPage();
   do {
                                                //now print
    u8g2.setFont(u8g2_font_10x20_mn);           //Font for voltages. 5 chars/row. Numeric only
    u8g2.drawStr(0,    lineHeight - 2, voltageStr[1]);
    u8g2.drawStr(0, 2* lineHeight - 2, voltageStr[2]);
    u8g2.drawStr(0, 3* lineHeight - 2, voltageStr[3]);
    u8g2.drawStr(0, 4* lineHeight - 2, voltageStr[4]);
    u8g2.setFont( u8g2_font_t0_11_tr);          //small font for logic levels
    u8g2.drawStr(3,72, logicStr);               //1 pel below bottom of OLED. OK if no descenders.
  } while (u8g2.nextPage() );

  delay(LOOP_DELAY);
}
/*
 ************************************ functions
 */
char * mVtoStr(long mV, char* vStr) {
/*
 * return str length 4 always
 */
  char bufferStr[7];
  long fraction = abs(mV)%1000;

  strcpy(vStr, ltoa(abs(mV)/1000, bufferStr, 10)); //whole volts: 0 to 99 possibly
  if (vStr[0] == '0') {
    vStr[0] = '.';                              //remove leading 0 to save length
  } else {
    strcat(vStr, ".");                          //add decimal pt
  }
   if (fraction <10) {                          //decimal point & leading 0s
    strcat(vStr, "00");
  }
  else if (fraction <100) {
    strcat(vStr, "0");
  }
  strcat(vStr, ltoa(fraction, bufferStr, 10));
  vStr[4] = '\0';                               //needs truncating in case V>=10V or <= -10V
  return vStr;
}
long readVoltage(unsigned char channel) {
  long value = OVERRANGE, adc_mV = 0;
  adc_mV = readADCmV(channel, false);           //try normal range first
  if (adc_mV != OVERRANGE){                     //if V in normal range
    value = adc_mV;
  } else {                                      //use high range
    pinMode(HighRangePin[channel], OUTPUT);
    digitalWrite(HighRangePin[channel], LOW);
    delay(1);                                   //let ADC in settle
    adc_mV = readADCmV(channel, true);
    if (adc_mV != OVERRANGE){
      value = (HIGH_RANGE_RATIO * adc_mV);      //scale to high range
      value = value * HighRangeCal[channel] / 1024; //calibration
    }
  }
  pinMode(HighRangePin[channel], INPUT);        //restore normal range
  return value;
}

long readADCmV(unsigned char channel, bool range) { //virtual ADC, returns mV. range true if high
/*
 * ifdef HYSTERESIS_ON then reduce effect of noise on display by ignoring small changes in ADC val
 */
  long adc = 0, value = 0;
  enum vref_levels {VREF0V55, VREF1V1, VREF2V5};
  vref_levels vrefVal = VREF0V55;

  #ifdef HYSTERESIS_ON
  static long previousADCval[2][5] = {0};          //index is range then channel
  #endif

//Start with lowest VREF and go up if overrange
  vrefVal = VREF0V55;                              //flag for later
  changeAnalogRef(ADC_REF_LEVEL_0V55, channel);
  adc = readADC(channel);
  if (adc == OVER_RANGE_ADC) {
    vrefVal = VREF1V1;
    changeAnalogRef(ADC_REF_LEVEL_1V1, channel);
    adc = readADC(channel);
    if (adc == OVER_RANGE_ADC) {
      vrefVal = VREF2V5;
      changeAnalogRef(ADC_REF_LEVEL_2V5, channel);
      adc = readADC(channel);
    }
  }
  #ifdef HYSTERESIS_ON
  if (adc == OVER_RANGE_ADC 
      || (abs(adc - previousADCval[range][channel]) > HYSTERESIS_AMOUNT)) {
    previousADCval[range][channel] = adc;           //use new value
  } else {
    adc = previousADCval[range][channel];           //use old value
  }
  #endif

  switch (vrefVal) {                                //calc mV
    case VREF0V55:
      value = (adc * ADC_MV_FOR_1024_0V55) / 1024;
      break;
    case VREF1V1:
      value = (adc * ADC_MV_FOR_1024_1V1) / 1024;
      break;
    case VREF2V5:
      value = (adc * ADC_MV_FOR_1024_2V5) / 1024;
      break;
  }
  if (adc == OVER_RANGE_ADC) {
    value = OVERRANGE;
  }
  return value;
}
long readADC(unsigned char channel) {
/* Takes NO_OF_ADC_READS samples to cut noise 
 * Returns 0 to 1023. 
 */
  long value = 0;

  byte i = NO_OF_ADC_READS;
  while (i-- > 0) {
    value += analogRead(ChannelPin[channel]);
  }
  value = value/NO_OF_ADC_READS;

  return value;
}

void changeAnalogRef(uint8_t ref, unsigned char channel) {
/*
 * Using this prevents bad ADC reads after changing ref and/or analog pin
 */
  analogReference(ref);                           //depends on MCU. Not fastest but smaller and robust.
  analogRead(ChannelPin[channel]);                //next reading may be wrong after changing ref so discard
  delayMicroseconds(ADC_SETTLE_TIME);             //in case pin has changed
}
