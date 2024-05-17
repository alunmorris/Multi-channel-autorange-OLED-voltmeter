EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 9055 5512
encoding utf-8
Sheet 1 1
Title "Wireless voltmeter transmitter"
Date "2020-11-25"
Rev "1"
Comp ""
Comment1 "Alun Morris"
Comment2 "Resistors 5%, capacitors 20% tolerance"
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 1300 4100 0    50   Input ~ 0
GND
Text GLabel 1300 4450 0    50   Input ~ 0
UPDI
Text Label 8300 5000 0    50   ~ 0
k
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5EB3FB20
P 1850 4200
F 0 "J1" H 1775 4525 50  0000 L CNN
F 1 "single row header" H 1929 4152 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1850 4200 50  0001 C CNN
F 3 "~" H 1850 4200 50  0001 C CNN
	1    1850 4200
	1    0    0    -1  
$EndComp
Text Notes 1150 4350 0    50   ~ 0
(pin cut off)
Wire Wire Line
	5400 2250 5700 2250
Wire Wire Line
	6750 3050 6750 3650
$Comp
L RF:NRF24L01_Breakout U2
U 1 1 5FB83F40
P 6750 2450
F 0 "U2" H 6950 2950 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 5900 3000 50  0000 L CNN
F 2 "RF_Module:nRF24L01_Breakout" H 6900 3050 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 6750 2350 50  0001 C CNN
	1    6750 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1650 6750 1850
Wire Wire Line
	7300 1650 6750 1650
Wire Wire Line
	7300 2650 7300 3650
Wire Wire Line
	7300 1650 7300 2350
$Comp
L Device:CP C3
U 1 1 5FB86BB7
P 7300 2500
F 0 "C3" H 7418 2546 50  0000 L CNN
F 1 "33u 16V" H 7418 2455 50  0000 L CNN
F 2 "" H 7338 2350 50  0001 C CNN
F 3 "~" H 7300 2500 50  0001 C CNN
	1    7300 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2550 5550 2550
$Comp
L Device:R_US R9
U 1 1 5FB90F6A
P 6100 2900
F 0 "R9" H 6168 2945 50  0000 L CNN
F 1 "15k" H 6168 2855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6140 2890 50  0001 C CNN
F 3 "~" H 6100 2900 50  0001 C CNN
F 4 "C25585" H 6100 2900 50  0001 C CNN "Part"
	1    6100 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5FB960EC
P 6500 3300
F 0 "C2" H 6615 3346 50  0000 L CNN
F 1 "1u" H 6615 3255 50  0000 L CNN
F 2 "" H 6538 3150 50  0001 C CNN
F 3 "~" H 6500 3300 50  0001 C CNN
	1    6500 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R7
U 1 1 5FB97C10
P 5550 2700
F 0 "R7" H 5618 2745 50  0000 L CNN
F 1 "330k" H 5618 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5590 2690 50  0001 C CNN
F 3 "~" H 5550 2700 50  0001 C CNN
F 4 "C25585" H 5550 2700 50  0001 C CNN "Part"
	1    5550 2700
	1    0    0    -1  
$EndComp
Connection ~ 5550 2550
Wire Wire Line
	6100 2750 6100 2550
Connection ~ 6100 2550
Wire Wire Line
	6100 2550 6250 2550
Wire Wire Line
	5550 2850 5550 3100
$Comp
L Device:R_US R8
U 1 1 5FB982D3
P 5550 3400
F 0 "R8" H 5618 3445 50  0000 L CNN
F 1 "68k" H 5618 3355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5590 3390 50  0001 C CNN
F 3 "~" H 5550 3400 50  0001 C CNN
F 4 "C25585" H 5550 3400 50  0001 C CNN "Part"
	1    5550 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3650 6750 3650
Wire Wire Line
	4800 2950 4800 3650
Wire Wire Line
	4800 3650 5550 3650
Connection ~ 6750 3650
$Comp
L Device:R_US R6
U 1 1 5FBAC5C9
P 5250 3100
F 0 "R6" H 5318 3145 50  0000 L CNN
F 1 "10M" H 5318 3055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5290 3090 50  0001 C CNN
F 3 "~" H 5250 3100 50  0001 C CNN
F 4 "C25585" H 5250 3100 50  0001 C CNN "Part"
	1    5250 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 3100 5550 3100
Connection ~ 5550 3100
Wire Wire Line
	5550 3100 5550 3250
Wire Wire Line
	5550 3550 5550 3650
Connection ~ 5550 3650
Wire Wire Line
	5550 3650 6100 3650
Connection ~ 6100 3650
Wire Wire Line
	6100 3650 6500 3650
Wire Wire Line
	6500 3450 6500 3650
Connection ~ 6500 3650
Wire Wire Line
	6500 3650 6750 3650
$Comp
L Device:LED D1
U 1 1 5FBB81C5
P 6100 3300
F 0 "D1" V 6139 3182 50  0000 R CNN
F 1 "LED" V 6048 3182 50  0000 R CNN
F 2 "" H 6100 3300 50  0001 C CNN
F 3 "~" H 6100 3300 50  0001 C CNN
	1    6100 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6100 3450 6100 3650
Wire Wire Line
	6100 3050 6100 3150
Wire Wire Line
	6500 3150 6100 3150
Connection ~ 6100 3150
$Comp
L MCU_Microchip_ATtiny:ATtiny402-SS U1
U 1 1 5FB56F67
P 4800 2350
F 0 "U1" H 4271 2396 50  0000 R CNN
F 1 "ATtiny402-SS" H 5400 2950 50  0000 R CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 4800 2350 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny202-402-AVR-MCU-with-Core-Independent-Peripherals_and-picoPower-40001969A.pdf" H 4800 2350 50  0001 C CNN
	1    4800 2350
	1    0    0    -1  
$EndComp
$Comp
L Simulation_SPICE:DIODE D2
U 1 1 5FBBF8D2
P 6350 1650
F 0 "D2" H 6350 1867 50  0000 C CNN
F 1 "1N4001" H 6350 1776 50  0000 C CNN
F 2 "" H 6350 1650 50  0001 C CNN
F 3 "~" H 6350 1650 50  0001 C CNN
F 4 "Y" H 6350 1650 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 6350 1650 50  0001 L CNN "Spice_Primitive"
	1    6350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1650 6750 1650
Connection ~ 6750 1650
Wire Wire Line
	4100 3650 4800 3650
Connection ~ 4800 3650
Wire Wire Line
	4100 2800 4100 3650
$Comp
L Device:C C1
U 1 1 5FBC5D65
P 4100 2650
F 0 "C1" H 3850 2700 50  0000 L CNN
F 1 "100n" H 3800 2600 50  0000 L CNN
F 2 "" H 4138 2500 50  0001 C CNN
F 3 "~" H 4100 2650 50  0001 C CNN
	1    4100 2650
	1    0    0    -1  
$EndComp
Connection ~ 4100 3650
$Comp
L Device:R_US R4
U 1 1 5FBCBE3D
P 3400 3450
F 0 "R4" H 3250 3550 50  0000 L CNN
F 1 "47k" H 3200 3450 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3440 3440 50  0001 C CNN
F 3 "~" H 3400 3450 50  0001 C CNN
F 4 "C25585" H 3400 3450 50  0001 C CNN "Part"
	1    3400 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	5700 2250 5700 1100
Connection ~ 5700 2250
$Comp
L Device:R_US R2
U 1 1 5FBD2A3B
P 3250 1100
F 0 "R2" H 3318 1145 50  0000 L CNN
F 1 "1M" H 3318 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3290 1090 50  0001 C CNN
F 3 "~" H 3250 1100 50  0001 C CNN
F 4 "C25585" H 3250 1100 50  0001 C CNN "Part"
	1    3250 1100
	0    1    1    0   
$EndComp
Text GLabel 1250 1100 0    50   Input ~ 0
VIN
Wire Wire Line
	1250 1100 3100 1100
Text GLabel 1300 1350 0    50   Input ~ 0
UPDI
Text GLabel 1300 3650 0    50   Input ~ 0
GND
Wire Wire Line
	1300 3650 1650 3650
$Comp
L Switch:SW_Push SW1
U 1 1 5FBD9BCE
P 2550 2200
F 0 "SW1" V 2504 2348 50  0000 L CNN
F 1 "On" V 2595 2348 50  0000 L CNN
F 2 "" H 2550 2400 50  0001 C CNN
F 3 "~" H 2550 2400 50  0001 C CNN
	1    2550 2200
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5FBDC0B8
P 3000 2200
F 0 "SW2" V 2954 2348 50  0000 L CNN
F 1 "Off" V 3045 2348 50  0000 L CNN
F 2 "" H 3000 2400 50  0001 C CNN
F 3 "~" H 3000 2400 50  0001 C CNN
	1    3000 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 2150 6250 2150
Wire Wire Line
	5700 2250 6250 2250
Wire Wire Line
	5400 2350 6250 2350
Wire Wire Line
	5400 2450 6250 2450
Wire Wire Line
	6250 2550 6250 2650
Wire Wire Line
	5400 2050 5550 2050
Wire Wire Line
	5550 2050 5550 1350
Wire Wire Line
	5550 2550 5850 2550
Connection ~ 5850 2550
Wire Wire Line
	5850 2550 6100 2550
Wire Wire Line
	5850 2550 5850 3000
Wire Wire Line
	4100 2500 4100 1650
Wire Wire Line
	4100 1650 4800 1650
Wire Wire Line
	4800 1650 4800 1750
$Comp
L Device:R_US R3
U 1 1 5FC24E2F
P 3000 2700
F 0 "R3" H 3068 2745 50  0000 L CNN
F 1 "150k" H 3068 2655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3040 2690 50  0001 C CNN
F 3 "~" H 3000 2700 50  0001 C CNN
F 4 "C25585" H 3000 2700 50  0001 C CNN "Part"
	1    3000 2700
	1    0    0    1   
$EndComp
$Comp
L Device:R_US R1
U 1 1 5FC2798D
P 2550 2650
F 0 "R1" H 2618 2695 50  0000 L CNN
F 1 "150k" H 2618 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2590 2640 50  0001 C CNN
F 3 "~" H 2550 2650 50  0001 C CNN
F 4 "C25585" H 2550 2650 50  0001 C CNN "Part"
	1    2550 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2500 2550 2400
Wire Wire Line
	2550 2800 2550 3650
Connection ~ 2550 3650
Wire Wire Line
	3000 2000 2550 2000
Connection ~ 2550 2000
Wire Wire Line
	2550 1350 1300 1350
$Comp
L Device:Battery_Cell BT1
U 1 1 5FC2D0CB
P 1650 2550
F 0 "BT1" H 1768 2646 50  0000 L CNN
F 1 "Lithium rechargeable cell" V 1450 2050 50  0000 L CNN
F 2 "" V 1650 2610 50  0001 C CNN
F 3 "~" V 1650 2610 50  0001 C CNN
	1    1650 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4100 1650 1650 1650
Wire Wire Line
	1650 1650 1650 2350
Connection ~ 4100 1650
Wire Wire Line
	1650 2650 1650 3650
Connection ~ 1650 3650
Wire Wire Line
	1650 3650 2550 3650
Wire Wire Line
	5550 1350 2550 1350
Connection ~ 2550 1350
Wire Wire Line
	2550 1350 2550 2000
Wire Wire Line
	6200 1650 4800 1650
Connection ~ 4800 1650
Wire Wire Line
	1300 4100 1650 4100
Wire Wire Line
	1300 4450 1450 4450
Wire Wire Line
	1450 4450 1450 4400
Wire Wire Line
	1450 4400 1650 4400
Wire Wire Line
	1300 4200 1650 4200
Text GLabel 1300 4200 0    50   Input ~ 0
VIN
Text Notes 1950 4250 0    50   ~ 0
Programming/Vin header
Wire Wire Line
	3400 1100 3650 1100
$Comp
L Device:RTRIM R5
U 1 1 5FC4F49D
P 3650 3250
F 0 "R5" H 3750 3200 50  0000 L CNN
F 1 "20k preset" H 3650 3100 50  0000 L CNN
F 2 "" V 3580 3250 50  0001 C CNN
F 3 "~" H 3650 3250 50  0001 C CNN
	1    3650 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3650 3200 3650
Wire Wire Line
	3200 3450 3200 3650
Connection ~ 3200 3650
Wire Wire Line
	3200 3650 4100 3650
Wire Wire Line
	3550 3450 3650 3450
Wire Wire Line
	3650 3100 5100 3100
Wire Wire Line
	3650 3100 3650 1100
Connection ~ 3650 1100
Wire Wire Line
	3650 1100 5700 1100
Wire Wire Line
	3650 3400 3650 3450
Connection ~ 3650 3100
Wire Wire Line
	3250 3450 3200 3450
Wire Wire Line
	3000 3000 3000 2850
Wire Wire Line
	3000 3000 5850 3000
Wire Wire Line
	3000 2550 3000 2400
$EndSCHEMATC
