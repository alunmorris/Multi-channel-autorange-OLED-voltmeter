# Multi-channel-autorange-OLED-voltmeter
A 4-channel 0 to 27V voltmeter with tiny OLED display. Uses ATTINY1614 MPU (could be 1604) and 72x40 0.42in I2C OLED module.
Also shows inputs as TTL levels: 0, 1 or indeterminate.

![multi-channel-voltmeter-1-crop-smaller](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/249b5f1d-4014-4a00-bd19-23b4a5b1b42a)


I did the project as an exercise in minimalism as well as being actually useful. Demo at https://youtu.be/wHfZQZPxzn4

Developed on Arduino

## Notable design features
* Size 18x21x11mm
* Autoranges six levels for optimal accuracy
* OLED unplugs, allowing the MPU to be programmed via the same header

![multi-channel-voltmeter-5-smaller](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/3fbe69d8-d6f0-4e4b-b7f5-889d0f148057)


## Function Overview
The MPU ADC (10-bit) reads voltage data from 4 channels and shows them on an OLED display.

Features:
* 0-27V range. 1mV resolution for V<1V.
* 3.3-12V supply. Reverse polarity protection.
* Accuracy c.1% (can be improved by calibration). May deteriorate - depends on resistor quality.
* Temperature variation is mostly dependent on the resistors. The MPU ADC is +/-0.1% over 0 to 40C. 
* Positive voltage only. Over voltage indication. +/-100V input tolerant.
* 1.1Mohm input impedance (or 1.1Mohm in series with 30pF when V<2.5V).
* Sample rate c.10/s, configurable

### Display operation
* Displays version info and help screen on power-up. Then shows 4 lines of voltage and one line with logic levels

## Repository overview
The repositry contains C++ code for the Arduino IDE and a circuit diagrams.

Code been complied on Arduino 2.3.2. Uses U8g2lib, Wire and stdlib libraries.

Compiled to 11593 bytes program and 379 bytes of RAM with Board Manager megaTinyCore version 2.2.9 (the latest as at 17 May 2024)

### Circuit diagrams
In eeschema/Kicad .sch format.

![Four-channel-OLED-voltmeter](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/72b6e7c1-ed9d-4c7e-8ad1-f4040416877c)

**How it works**

**Vin1-4** Each input is read directly (normal range) or divided by 11 by resistors (high range) in which case 27V becomes 2.5V at the MPU pin.
Taking VIN1 for example, PA3 is floating in normal range but is set to logic L in high range, creating a potential divider. The ADC reference voltages are 0.55V, 1.1V and 2.5V. Noise is reduced by 30pF capacitors which are connected to the floating/logic L end of the 100k resistors for ease of layout.

**Programming** is via the OLED/UPDI header. The OLED is unplagged and a custom 1.27mm pitch UPDI cable is used. 

**Construction**

![multi-channel-voltmeter-2-small](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/1868fad7-b057-40a1-8313-594184f2765f) ![multi-channel-voltmeter-3-small](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/a3aee87b-8293-4396-891f-ee12c38c857d)
Built on a 1.27mm pitch prototyping PCB. using SMD components, apart from the two headers. I removed the 4 pin 2.54mm header from a standard 72x40 0.42in OLED module and replaced with a 7 pin 1.27mm male header with pins 2, 3 and 5 removed so that it goes through the same holes.

![multi-channel-voltmeter-4-small](https://github.com/alunmorris/Multi-channel-autorange-OLED-voltmeter/assets/4630866/53c8bc0e-42db-4ccd-876c-d15efc19537f)

Another 1 pin header is soldered to one of the mounting holes on the module. This plugs into a small female header on the MPU board, for rigidity.
A turned pin (instead of the common square pin) header is used for the 6-pin power and signal inputs, for easier insertion into a breadboard.

**Possible improvements** 
1. Connect the 30pF capacitors to the junction of 1Mohm/100k resistors. This would improve noise reduction in high range. Note that noise is lower on high range in any event due to the lower impedance at the ADC pin.
2. Make the input impedance a constant 1.1Mohm. The present cicruit changes the input impedance in high range (Vin>2.5V). The 100k resistors would be tied to ground and two analog inputs used for each Vin channel. Normal range inputs would need an extra 1Mohm resisitor as input protection. The SDA or SCL pin would need to be moved to one of PB2 or PB3 pins to allow 8 analog input pins. SW I2C would have to be used.
3. Use a larger display. I used a 0.42in OLED to make the voltemter as small as possible. The logic level readout needs a tiny font as a result.
