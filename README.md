# Multi-channel-wireless-voltmeter
A multi-channel wireless (2.4GHz) voltmeter display using ATTINY402, Arduino nano, NRF24L01+ and 128x32 I2C OLED.

The transmitter is optimised for low power and will run from a battery for weeks or months depending on usage, sample frequency and of course battery capacity.

Range is a few meters to tens of meters depending on how well the radio module is isolated from the controller, and the RF power setting.

I did the project as an exercise in minimalism. Demo at https://youtu.be/bwuttJC_tyY

Developed on Arduino

![Display top no bat small-](https://user-images.githubusercontent.com/4630866/100372525-44a9a080-3001-11eb-8f44-4add414d4948.jpg)

## Notable design features
* Transmitter code just about fits on the 8-pin 4KB flash ATTINY402. It should compile on ATTINY412 and larger AVR 0 and 1 series chips. UPDI (but not HV type) programmer needed.
* Display code <16KB and so fits on eg ATMEGA168P or 328P eg an Arduino Uno or nano.
* Pin saving by multiple use of microcontroller (MPU) pins:
  * SPI chip-enable, LED drive, negative input test mode and switch A/B detection on one pin
  * MISO and analog input on one pin
  * UPDI programming and switch input on one pin (without needing a high-voltage UPDI programmer to re-program)
* Measuring MPU Vdd without using an input pin
* Very-low power sleep mode of ATTINY 0 and 1 series
* Small form factors

## Function Overview
The display receives voltage data from up to 4 transmitters (one channel each) and shows them on a 128x32 OLED display.

Features:
* 0-20V range, can be configured by resistor selection. 0.01V resolution.
* Accuracy better than 1% (by calibration). May deteriorate - depends on resistor quality.
* Temperature variation is mostly dependent on the resistors. The MPU ADC is +/-0.1% over 0 to 40C. 
* Positive voltage only. Over voltage and undervoltage (negative) indication. +/-100V input tolerant.
* 1Mohm input impedance, can be configured by resistor selection
* Sample rate once/s, configurable
* Minimum and maximum display
* Transmitter low battery warning and battery voltage display. Low battery protection.
* Transmitter auto power-off
* TRansmitter retries if the data is not received
* Display modes: all channels or single channel with more detais

### Transmitter operation
* On and Off buttons. Off is sleep mode
* Configurable auto power-off period. LED flashes 8 times when powering off.
* Powers off if battery below critical voltage - configurable
* Disable auto power-off by long press of On button. LED flashes 4 times.
* MPU sums four voltage readings to reduce noise and to improved resolution of the 10-bit ADC
* LED flashes 20 times if radio IC not responding on power-up
* LED flashes briefly on each failed data transmission
* The buttons are read 16 times a second

### Display operation
* Displays version info, radio channel and lost-signal timeout on power-up/ reset
* Then shows Summary view of all available voltage channels
* Each channel voltage field is blank until data received
* Low battery, critical battery and lost signal indication
* Single channel view shows voltage, Min, Max, low battery, critical battery and lost signal
* Single view also shows Transmitter battery voltage (Vdd) for a few seconds
* Mode button rotates around display modes from Summary to each single channel in rotation

#### Summary view (up to 4 channels)
Image shows data received from channel 1-3. Low battery warning on 3. Critical battery warning and lost-signal on 2.
 
![Summary](https://user-images.githubusercontent.com/4630866/99881511-32e78800-2c12-11eb-9359-a8b6a553febb.png)

Over voltage shows as -----, under voltage as -0.00, no data is just blank

#### Single channel view

![Single view](https://user-images.githubusercontent.com/4630866/99881510-32e78800-2c12-11eb-8216-1242ac274dff.png)
![Single view after a few seconds](https://user-images.githubusercontent.com/4630866/99881507-2c591080-2c12-11eb-9e34-ab5b6b527589.png)
 
 The lost signal and battery warnings appear the same as in the Summary view.

## Repository overview
The repositry contains C++ code for the Arduino IDE and circuit diagrams. The Transmitter and Receiver are in separate folders. Read through the options to set and the compile settings guidance.

They have been complied on Arduino 1.8.13.

### Transmitter code
One .ino file. Uses SPI, RF24 and avr/sleep libraries.

Compiled to 3986 bytes with Board Manager megaTinyCore version 2.1.2 or 2.1.5 (the latest as at 21 Nov 2020)

### Display Code
One .ino file and two .h files (in /library) for the custom font. Uses SPI, RF24, Wire, SSD1306Ascii and SSD1306AsciiWire libraries.

Compiled to 11856 bytes with 'noise filter' and 'show Vdd' options used.

### Circuit diagrams
In eeschema/Kicad .sch format.

#### Transmitter

![Transmitter circuit 25 Nov 2020](https://user-images.githubusercontent.com/4630866/100223155-05048b00-2f13-11eb-9c7b-396bb5dd92e8.png)

![Transmitter](https://user-images.githubusercontent.com/4630866/100109045-59e8c880-2e63-11eb-8b30-06eb8d54f72b.jpg)

![Transmitter bottom small](https://user-images.githubusercontent.com/4630866/100223954-139f7200-2f14-11eb-8a21-4cf984bad546.jpg)

**How it works**
**Vin** is divided by R2/(R4+R5)so that 20V becomes 1.1V - the ADC reference. R4 calibrates the result.

Negative input test: after the ADC is read, MPU pin PA7 is switched from low to high. This is divided and raises the ADC pin by ~2mV or around 2LSB for the 10 bit resolution. If Vin =0V the ADC wil now read positive and we deduce that Vin was not negative. 

The ADC reading is scaled to 32767 (=20V) and transmitted. along with all other info every time (including version no) for simplicity.

**Switch input:** shared with the programming pin UPDI. UPDI can be used normally as an input without blowing fuses (meaning a HV programmer is needed to re-program). UPDI has an internal pull up of 20-50kohm. I tried a switch connected to ground but contact bounce can occasionally trip the MPU's programming circuit on. The spec says the MPU should automatically exit a bogus triggering after 10-20ms but it often does not. The MPU keeps operating normally but draws over 1mA in sleep mode. The work around here is to use the ADC to read the UPDI pin, with the internal reference changed to Vdd. R1 and R3 are chosen so that UPDI does not fall below 0.6 of Vdd for the wide range of the internal pull-up even when both switches are pressed.

PA7 is low when the switch input is read so either switch will be detected. If a switch is detected, PA7 is then set high and UPDI read again. If the reading is near 1023 then it must be SW2 pressed.

**Power:** the operating range of a rechargeable lithium cell is 3.6-4.2V. There are low drop-out regulator that can manage a 0.6V dropand have <10uA quiescent current but a diode works fine if a large decoupling capacitor is used. The 0.6V typical diode drop brings the voltage within U2's rating of 3-3.6V. Well it would do with the nominal drop, but at the low current taken the drop is ~0.5V so the spec is exceeded by 0.1V on a full cell. Very unlikly to cause a problem.
I tested current draw with a few cheap NRF24L01+ modules from a batch I bought from Aliexpress. The receiver took 30-40uA during sleep. The MPU should take 6uA max in idle mode so the radio modules were taking far more tha the 6uA maximum in the spec. I read that many cheap Chinese ones are clones of the Nordic chip, some with far worse power use than mine.

I could not get the MPU's power-down mode to work but it would only save 6uA max anyway.

The MPU was taking over 2mA in sleep until I noticed I had not diabled the brown-out detector (BoD). Read the compile advice in the source code onhow to do this.

Use a 16V rating for C2 (if electrolytic) because lower voltage ones can drain several uA (so I read).

**Multi-use of PA7**: I orginally modified the RF24 library for U2 to use only 4 pins (SCKM MOSIM MISO & CSN) to drive U2. CE was tied high. CE turns the chip on and off and I found that even when powered-down with the library call the chip used far more than the specified idle current, and using a modifed library gave update problems.
There is a 3-pin inteface mode in RF24 (using bit-banging) for the ATTINY84/85 but I could not make it work with the ATTINY402. It also needs more components.

It was cleaner to use the normal 5-pin connectin and re-use pins. Using MISO as an ADC input when CSN is high was straighforward - U2 dos not drive MISO then. CE is more complex.

U2 doesn't mind if CE is pulsed high when idle so I use PA7 = CE for dring the LED, driving negative input test bias and for reading SW2.

**Programming header J1:** I've standardised on this 0.1in pitch male header for my AVR series 0/1 projects. The cut off pins 3 provides keying (the programmer has a blanked hole for that pin). The programmer drives pin 2 with 3.3V to power targets if needed. A self powered target can use pin 2 for something else. Here it is Vin.

I have extended the header for another project so that one header does all interfacing as well as programming. UPDI remains at one end so that position keying is maintained. Maybe I should have made UPDI pin 1 but it's conventional to make ground pin 1.

**Construction** The range may be poor or non-existent if the NRF24L01+ is mounted right behind the MPU part.

#### Receiver

![Receiver](https://user-images.githubusercontent.com/4630866/99911593-69460580-2ced-11eb-8d88-94d7b3283639.png)

This is simply an NRF24L01+ and an OLED display wired to an Arduino nano. The OLED is powered by the nano 3.3V output but U3 has its own regulator to give a clean power supply. 

**Construction** The range may be poor or non-existent if the NRF24L01+ is mounted right behind and parallel to the MPU part.

## Car battery monitor application
I have fitted a transmitter inside an OBD-2 plug (the European car diagnostic standard). 12V is always live on the interface. A 3.3V regulator powers the MPU and NRF24L01+. The switches and negative voltage test components are omitted.

![DSC_2034_300x198](https://user-images.githubusercontent.com/4630866/100904465-f55ee680-34be-11eb-9bbf-50b8507cc48f.JPG) ![DSC_2033_300x254](https://user-images.githubusercontent.com/4630866/100904035-7b2e6200-34be-11eb-94e8-f1383603a0f9.JPG)

