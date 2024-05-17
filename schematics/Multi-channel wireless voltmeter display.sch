EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 9055 5512
encoding utf-8
Sheet 1 1
Title "Multi-channel wireless voltmeter display"
Date "2020-11-23"
Rev "1"
Comp ""
Comment1 "Alun Morris"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 8300 5000 0    50   ~ 0
k
Wire Wire Line
	4200 2450 4200 3050
Wire Wire Line
	4200 2450 3750 2450
Wire Wire Line
	4300 2250 3750 2250
Text Notes 2050 2050 0    50   ~ 0
USB power input\n not shown
Wire Wire Line
	3950 3650 4400 3650
Connection ~ 3950 3650
Wire Wire Line
	3950 3550 3950 3650
Wire Wire Line
	3950 1750 3750 1750
Wire Wire Line
	3950 3150 3950 1750
$Comp
L Switch:SW_Push SW1
U 1 1 5FB97C2D
P 3950 3350
F 0 "SW1" V 3900 3100 50  0000 L CNN
F 1 "Mode" V 4000 3000 50  0000 L CNN
F 2 "" H 3950 3550 50  0001 C CNN
F 3 "~" H 3950 3550 50  0001 C CNN
	1    3950 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 1250 4400 3650
Wire Wire Line
	1750 3650 3250 3650
Wire Wire Line
	1750 3350 1750 3650
Wire Wire Line
	3050 850  3050 1050
Wire Wire Line
	4100 850  3050 850 
$Comp
L Regulator_Linear:L78L33_TO92 U2
U 1 1 5FB8AF8F
P 4400 850
F 0 "U2" H 4400 1092 50  0000 C CNN
F 1 "L78L33_TO92" H 4400 1001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4400 1075 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/15/55/e5/aa/23/5b/43/fd/CD00000446.pdf/files/CD00000446.pdf/jcr:content/translations/en.CD00000446.pdf" H 4400 800 50  0001 C CNN
	1    4400 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3650 3950 3650
Connection ~ 3250 3650
Wire Wire Line
	3250 3050 3250 3650
Wire Wire Line
	3150 950  3150 1050
Wire Wire Line
	1750 950  3150 950 
Wire Wire Line
	1750 2350 1750 950 
Connection ~ 4400 1250
Wire Wire Line
	4800 1250 4400 1250
Wire Wire Line
	4800 850  5400 850 
Connection ~ 4800 850 
Wire Wire Line
	4800 1050 4800 850 
$Comp
L Device:C_Small C1
U 1 1 5FB8DD44
P 4800 1150
F 0 "C1" H 4892 1196 50  0000 L CNN
F 1 "100n" H 4892 1105 50  0000 L CNN
F 2 "" H 4800 1150 50  0001 C CNN
F 3 "~" H 4800 1150 50  0001 C CNN
	1    4800 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2050 5400 2250
Connection ~ 5400 2050
Wire Wire Line
	5950 2050 5400 2050
Wire Wire Line
	5400 850  5400 2050
Wire Wire Line
	4700 850  4800 850 
Wire Wire Line
	4400 3650 5400 3650
Connection ~ 4400 3650
Wire Wire Line
	4400 1150 4400 1250
Wire Wire Line
	2500 2650 2250 2650
Wire Wire Line
	2500 2450 2500 2650
Wire Wire Line
	2750 2450 2500 2450
Wire Wire Line
	2250 2550 2750 2550
Wire Wire Line
	4200 3050 4900 3050
Wire Wire Line
	4300 2850 4300 2250
Wire Wire Line
	3750 2750 4900 2750
Wire Wire Line
	3750 2650 4900 2650
Wire Wire Line
	3750 2550 4900 2550
Wire Wire Line
	5950 3050 5950 3650
Wire Wire Line
	5950 2050 5950 2750
$Comp
L Device:CP C2
U 1 1 5FB86BB7
P 5950 2900
F 0 "C2" H 6068 2946 50  0000 L CNN
F 1 "10u 16V" H 6068 2855 50  0000 L CNN
F 2 "" H 5988 2750 50  0001 C CNN
F 3 "~" H 5950 2900 50  0001 C CNN
	1    5950 2900
	1    0    0    -1  
$EndComp
$Comp
L RF:NRF24L01_Breakout U3
U 1 1 5FB83F40
P 5400 2850
F 0 "U3" H 5600 3350 50  0000 L CNN
F 1 "NRF24L01_Breakout" H 4550 3400 50  0000 L CNN
F 2 "RF_Module:nRF24L01_Breakout" H 5550 3450 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2730/34105/file/nRF24L01_Product_Specification_v2_0.pdf" H 5400 2750 50  0001 C CNN
	1    5400 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2850 4900 2850
$Comp
L Display_Character:OLED_SSD1306 U1
U 1 1 5FB7669A
P 1750 2850
F 0 "U1" H 2180 2896 50  0000 L CNN
F 1 "OLED_SSD1306" H 2180 2805 50  0000 L CNN
F 2 "Display:EA_T123X-I2C" H 1750 2250 50  0001 C CNN
F 3 "http://www.lcd-module.de/pdf/doma/t123-i2c.pdf" H 1750 2350 50  0001 C CNN
	1    1750 2850
	-1   0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x U4
U 1 1 5FB6E92B
P 3250 2050
F 0 "U4" H 4000 2300 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 4100 2450 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 3250 2050 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 3250 2050 50  0001 C CNN
	1    3250 2050
	-1   0    0    -1  
$EndComp
Connection ~ 5400 3650
Wire Wire Line
	5950 3650 5400 3650
Wire Wire Line
	5400 3450 5400 3650
$EndSCHEMATC
