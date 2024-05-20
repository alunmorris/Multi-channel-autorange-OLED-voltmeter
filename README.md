# Multi-channel-autorange-OLED-voltmeter
A 4-channel 0 to 27V voltmeter with tiny OLED display. Uses ATTINY1614 MPU (could be 1604) and 72x40 0.42in I2C OLED module.
Also shows inputs as TTL levels: 0, 1 or indeterminate.

I did the project as an exercise in minimalism as well as being actually useful. Demo at https://youtu.be/wHfZQZPxzn4

Developed on Arduino

## Notable design features
* Size 18x21x11mm
* Autoranges six levels for optimal accuracy
* OLED unplugs, allowing the MPU to be programmed via the same header

## Function Overview
The MPU ADC (10-bit) reads voltage data from 4 channels and shows them on an OLED display.

Features:
* 0-27V range. 1mV resolution for V<1V.
* Accuracy c.1% (can be improved by calibration). May deteriorate - depends on resistor quality.
* Temperature variation is mostly dependent on the resistors. The MPU ADC is +/-0.1% over 0 to 40C. 
* Positive voltage only. Over voltage indication. +/-100V input tolerant.
* 1.1Mohm input impedance.
* Sample rate c.10/s, configurable

### Display operation
* Displays version info and help screen on power-up. Then shows 4 lines of voltage and one line with logic levels

## Repository overview
The repositry contains C++ code for the Arduino IDE and a circuit diagrams.

Code been complied on Arduino 2.3.2. Uses U8g2lib, Wire and stdlib libraries.

Compiled to 11593 bytes program and 379 bytes of RAM with Board Manager megaTinyCore version 2.2.9 (the latest as at 17 May 2024)

### Circuit diagrams
In eeschema/Kicad .sch format.

**How it works**
**Vin1-4** Each input is read directly (normal range) or divided by 11 by resistors (high range) in which case 27V becomes 2.5V at the MPU pin. The ADC reference voltages are 0.55V, 1.1V and 2.5V.

**Programming** is via the OLED/UPDI header. The OLED is unplagged and a custom 1.27mm pitch UPDI cable is used. 

**Construction** on a 1.27mm pitch prototyping PCB. using SMD components, apart from the two headers.
