# Multi-channel-autorange-OLED-voltmeter
A 4-channel 0 to 27V voltmeter with tiny OLED display. Uses ATTINY1614 MPU (could be 1604) and 72x40 0.42in I2C OLED module.
Also shows inputs as TTL levels: 0, 1 or indeterminate.

I did the project as an exercise in minimalism as well as being useful. Demo at 

Developed on Arduino

## Notable design features
* Size 18x21x11mm
* Autoranges six levels
* OLED plugs in, allowing MPU to be programmed via the same header

## Function Overview
The MPU reads voltage data from 4 channels and shows them on an OLED display.

Features:
* 0-27V range. 1mV resolution for V<1V.
* Accuracy c.1% (can be improved by calibration). May deteriorate - depends on resistor quality.
* Temperature variation is mostly dependent on the resistors. The MPU ADC is +/-0.1% over 0 to 40C. 
* Positive voltage only. Over voltage indication. +/-100V input tolerant.
* 1.1Mohm input impedance.
* Sample rate c.10/s configurable

### Display operation
* Displays version info and help screen on power-up

## Repository overview
The repositry contains C++ code for the Arduino IDE and circuit diagrams.

Code been complied on Arduino 2.3.2.

### Transmitter code
One .ino file. Uses SPI, RF24 and avr/sleep libraries.

Compiled to 11593 bytes program and 379 bytes of RAM with Board Manager megaTinyCore version 2.2.9 (the latest as at 17 May 2024)

### Circuit diagrams
In eeschema/Kicad .sch format.

**How it works**
**Vin1-4** Each input is read directly (normal range) or divided by 11 (high range) in which case 27V becomes 2.5V at the MPU pin - the highest ADC reference.

**Programming** is via the OLED/UPDI header. The  OLED is unplagged and a custom 1.27mm pitch UPDI cable is used. 

**Construction** on a 1.27mm pitch prototyping PCB. using SMD components apart from the two headers.
