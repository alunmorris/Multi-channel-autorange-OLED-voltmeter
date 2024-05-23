EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 9055 5512
encoding utf-8
Sheet 1 1
Title "Four channel OLED voltmeter"
Date "2024-05-20"
Rev "1"
Comp ""
Comment1 "Alun Morris"
Comment2 "Resistors 1M & 100k 1%, rest 5%, capacitors 20% tolerance"
Comment3 ""
Comment4 ""
$EndDescr
Text Label 8300 5000 0    50   ~ 0
k
Text GLabel 1100 1750 0    50   Input ~ 0
VCC
Text GLabel 1100 2500 0    50   Input ~ 0
VIN3
$Comp
L Device:R_Small R?
U 1 1 6647BABE
P 1300 2500
F 0 "R?" V 1196 2500 50  0001 C CNN
F 1 "1M" V 1196 2500 50  0000 C CNN
F 2 "" H 1300 2500 50  0001 C CNN
F 3 "~" H 1300 2500 50  0001 C CNN
	1    1300 2500
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6647C17C
P 1700 2650
F 0 "R?" V 1596 2650 50  0001 C CNN
F 1 "100k" V 1596 2650 50  0000 C CNN
F 2 "" H 1700 2650 50  0001 C CNN
F 3 "~" H 1700 2650 50  0001 C CNN
	1    1700 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 2500 1100 2500
Text GLabel 1100 2750 0    50   Input ~ 0
VIN4
$Comp
L Device:R_Small R?
U 1 1 66480539
P 1300 2750
F 0 "R?" V 1196 2750 50  0001 C CNN
F 1 "1M" V 1196 2750 50  0000 C CNN
F 2 "" H 1300 2750 50  0001 C CNN
F 3 "~" H 1300 2750 50  0001 C CNN
	1    1300 2750
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6648053F
P 1700 2900
F 0 "R?" V 1596 2900 50  0001 C CNN
F 1 "100k" V 1596 2900 50  0000 C CNN
F 2 "" H 1700 2900 50  0001 C CNN
F 3 "~" H 1700 2900 50  0001 C CNN
	1    1700 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 2750 1100 2750
$Comp
L Device:R_Small R?
U 1 1 6648170C
P 1300 2250
F 0 "R?" V 1196 2250 50  0001 C CNN
F 1 "1M" V 1196 2250 50  0000 C CNN
F 2 "" H 1300 2250 50  0001 C CNN
F 3 "~" H 1300 2250 50  0001 C CNN
	1    1300 2250
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 66481712
P 1700 2400
F 0 "R?" V 1596 2400 50  0001 C CNN
F 1 "100k" V 1596 2400 50  0000 C CNN
F 2 "" H 1700 2400 50  0001 C CNN
F 3 "~" H 1700 2400 50  0001 C CNN
	1    1700 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 2250 1100 2250
Text GLabel 1100 2000 0    50   Input ~ 0
VIN1
Wire Wire Line
	4750 2750 4750 2600
$Comp
L MCU_Microchip_ATtiny:ATtiny1614-SS U?
U 1 1 6646561C
P 5350 2300
F 0 "U?" H 5350 3181 50  0001 C CNN
F 1 "ATtiny1614-SS" H 5350 3089 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5350 2300 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny1614-data-sheet-40001995A.pdf" H 5350 2300 50  0001 C CNN
	1    5350 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4750 2500 1600 2500
Wire Wire Line
	1600 2500 1600 2650
Text GLabel 1100 2250 0    50   Input ~ 0
VIN2
Wire Wire Line
	3750 2400 4750 2400
Wire Wire Line
	3850 2000 3850 2300
Wire Wire Line
	3850 2300 4750 2300
Wire Wire Line
	1800 2150 1900 2150
Wire Wire Line
	4750 2150 4750 2200
Wire Wire Line
	1800 2400 2300 2400
Wire Wire Line
	3650 2400 3650 2100
Wire Wire Line
	3650 2100 4750 2100
Wire Wire Line
	3950 2650 3950 2000
Wire Wire Line
	3950 2000 4750 2000
Wire Wire Line
	1800 2900 3100 2900
Wire Wire Line
	4750 2900 4750 3050
Wire Wire Line
	4750 3050 6000 3050
Wire Wire Line
	6000 3050 6000 2200
Wire Wire Line
	6000 2200 5950 2200
Wire Wire Line
	5350 3000 5350 3650
Connection ~ 5350 3650
$Comp
L Device:R_Small R?
U 1 1 6649A416
P 2250 1100
F 0 "R?" V 2146 1100 50  0001 C CNN
F 1 "10R" V 2146 1100 50  0000 C CNN
F 2 "" H 2250 1100 50  0001 C CNN
F 3 "~" H 2250 1100 50  0001 C CNN
	1    2250 1100
	0    1    1    0   
$EndComp
Wire Wire Line
	1900 3200 1900 2150
Connection ~ 1900 2150
Wire Wire Line
	1900 2150 4750 2150
$Comp
L Device:C C?
U 1 1 6649C531
P 2300 3350
F 0 "C?" H 2050 3400 50  0001 L CNN
F 1 "30p" H 2000 3300 50  0000 L CNN
F 2 "" H 2338 3200 50  0001 C CNN
F 3 "~" H 2300 3350 50  0001 C CNN
	1    2300 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6649C686
P 2700 3350
F 0 "C?" H 2450 3400 50  0001 L CNN
F 1 "30p" H 2400 3300 50  0000 L CNN
F 2 "" H 2738 3200 50  0001 C CNN
F 3 "~" H 2700 3350 50  0001 C CNN
	1    2700 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3500 1900 3650
Wire Wire Line
	1900 3650 2300 3650
Wire Wire Line
	2300 3500 2300 3650
Connection ~ 2300 3650
Wire Wire Line
	2300 3650 2700 3650
Wire Wire Line
	2700 3500 2700 3650
Connection ~ 2700 3650
Wire Wire Line
	2700 3650 3100 3650
Wire Wire Line
	3100 3500 3100 3650
Connection ~ 3100 3650
Wire Wire Line
	3100 3650 3400 3650
Wire Wire Line
	2300 3200 2300 2400
Connection ~ 2300 2400
Wire Wire Line
	2300 2400 3650 2400
Wire Wire Line
	2700 3200 2700 2650
Wire Wire Line
	1800 2650 2700 2650
Connection ~ 2700 2650
Wire Wire Line
	2700 2650 3950 2650
Wire Wire Line
	3100 3200 3100 2900
Connection ~ 3100 2900
Wire Wire Line
	3100 2900 4750 2900
$Comp
L Regulator_Linear:L78L33_SOT89 U?
U 1 1 664AA8A4
P 3400 1100
F 0 "U?" H 3400 1342 50  0001 C CNN
F 1 "HT7333" H 3400 1251 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3" H 3400 1300 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/15/55/e5/aa/23/5b/43/fd/CD00000446.pdf/files/CD00000446.pdf/jcr:content/translations/en.CD00000446.pdf" H 3400 1050 50  0001 C CNN
	1    3400 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1650 3400 1400
Wire Wire Line
	3400 1650 3400 1750
Connection ~ 3400 1650
Connection ~ 3400 3650
Wire Wire Line
	3400 3650 5350 3650
$Comp
L Device:C C?
U 1 1 6649A952
P 1900 3350
F 0 "C?" H 1650 3400 50  0001 L CNN
F 1 "30p" H 1600 3300 50  0000 L CNN
F 2 "" H 1938 3200 50  0001 C CNN
F 3 "~" H 1900 3350 50  0001 C CNN
	1    1900 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 664B95EA
P 3900 1400
F 0 "C?" H 3650 1450 50  0001 L CNN
F 1 "470n" H 3600 1350 50  0000 L CNN
F 2 "" H 3938 1250 50  0001 C CNN
F 3 "~" H 3900 1400 50  0001 C CNN
	1    3900 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1100 3900 1250
Wire Wire Line
	3900 1550 3900 1750
Wire Wire Line
	3900 1750 3400 1750
Connection ~ 3400 1750
Wire Wire Line
	3400 1750 3400 3650
Wire Wire Line
	5350 1600 4900 1600
Wire Wire Line
	4900 1600 4900 1100
Wire Wire Line
	3700 1100 3900 1100
Connection ~ 3900 1100
Wire Wire Line
	3900 1100 4900 1100
Wire Wire Line
	5350 3650 7250 3650
$Comp
L Connector_Generic:Conn_01x07 J?
U 1 1 664C4F6A
P 7450 2300
F 0 "J?" H 7368 1775 50  0001 C CNN
F 1 "OLED/UPDI female header 1.27mm pitch" H 7750 1750 50  0000 C CNN
F 2 "" H 7450 2300 50  0001 C CNN
F 3 "~" H 7450 2300 50  0001 C CNN
	1    7450 2300
	1    0    0    1   
$EndComp
Wire Wire Line
	5950 2000 6700 2000
Wire Wire Line
	7250 2200 6450 2200
Wire Wire Line
	6450 1900 5950 1900
Wire Wire Line
	7250 2400 6550 2400
Wire Wire Line
	6550 2400 6550 1600
Wire Wire Line
	6550 1600 5350 1600
Connection ~ 5350 1600
Wire Wire Line
	7250 2600 7250 3650
Wire Wire Line
	4750 1900 4650 1900
Wire Wire Line
	4650 1900 4650 1350
Wire Wire Line
	4650 1350 6800 1350
Wire Wire Line
	6800 1350 6800 2100
Wire Wire Line
	6800 2100 7250 2100
Text Notes 7550 2000 0    50   ~ 0
SDA
Text Notes 7550 2100 0    50   ~ 0
UPDI
Text Notes 7550 2200 0    50   ~ 0
SCL
Text Notes 7550 2300 0    50   ~ 0
blanked off
Text Notes 7550 2500 0    50   ~ 0
blanked off
Text Notes 7550 2400 0    50   ~ 0
VCC
Text Notes 7550 2600 0    50   ~ 0
GND
$Comp
L Device:R_Small R?
U 1 1 664D8FC9
P 6400 1100
F 0 "R?" V 6296 1100 50  0001 C CNN
F 1 "7k5" V 6296 1100 50  0000 C CNN
F 2 "" H 6400 1100 50  0001 C CNN
F 3 "~" H 6400 1100 50  0001 C CNN
	1    6400 1100
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 664D98EC
P 6050 1250
F 0 "R?" V 5946 1250 50  0001 C CNN
F 1 "7k5" V 5946 1250 50  0000 C CNN
F 2 "" H 6050 1250 50  0001 C CNN
F 3 "~" H 6050 1250 50  0001 C CNN
	1    6050 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 1100 5850 1100
Connection ~ 4900 1100
Wire Wire Line
	6500 1100 6700 1100
Wire Wire Line
	6700 1100 6700 2000
Connection ~ 6700 2000
Wire Wire Line
	6700 2000 7250 2000
Wire Wire Line
	6450 1250 6450 1900
Wire Wire Line
	6150 1250 6450 1250
Connection ~ 6450 1900
Wire Wire Line
	6450 1900 6450 2200
Wire Wire Line
	5950 1250 5850 1250
Wire Wire Line
	5850 1250 5850 1100
Connection ~ 5850 1100
Wire Wire Line
	5850 1100 6300 1100
Text GLabel 1100 1500 0    50   Input ~ 0
GND
$Comp
L Device:D_Schottky D?
U 1 1 664E92B6
P 2600 1500
F 0 "D?" V 2554 1580 50  0001 L CNN
F 1 "D_Schottky" V 2600 1580 50  0000 L CNN
F 2 "" H 2600 1500 50  0001 C CNN
F 3 "~" H 2600 1500 50  0001 C CNN
	1    2600 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 1650 3400 1650
Wire Wire Line
	2600 1350 2600 1100
Connection ~ 2600 1100
Wire Wire Line
	2600 1100 3100 1100
Wire Wire Line
	2350 1100 2600 1100
Wire Wire Line
	2100 1750 2100 1100
Wire Wire Line
	2100 1100 2150 1100
Wire Wire Line
	2300 1500 2300 1650
Wire Wire Line
	2300 1650 2600 1650
Connection ~ 2600 1650
Text Notes 700  1350 0    50   ~ 0
male header 2.54mm pitch
Wire Wire Line
	1200 2000 1100 2000
$Comp
L Device:R_Small R?
U 1 1 6648229F
P 1300 2000
F 0 "R?" V 1196 2000 50  0001 C CNN
F 1 "1M" V 1196 2000 50  0000 C CNN
F 2 "" H 1300 2000 50  0001 C CNN
F 3 "~" H 1300 2000 50  0001 C CNN
	1    1300 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	1100 1500 2300 1500
Wire Wire Line
	1100 1750 2100 1750
Wire Wire Line
	3750 2400 3750 2250
Wire Wire Line
	1600 2250 3750 2250
Wire Wire Line
	1600 2250 1600 2400
Wire Wire Line
	1400 2250 1600 2250
Connection ~ 1600 2250
Wire Wire Line
	1400 2500 1600 2500
Connection ~ 1600 2500
$Comp
L Device:C C?
U 1 1 6649CF1E
P 3100 3350
F 0 "C?" H 2850 3400 50  0001 L CNN
F 1 "30p" H 2800 3300 50  0000 L CNN
F 2 "" H 3138 3200 50  0001 C CNN
F 3 "~" H 3100 3350 50  0001 C CNN
	1    3100 3350
	1    0    0    -1  
$EndComp
Connection ~ 1600 2750
Wire Wire Line
	1400 2750 1600 2750
Wire Wire Line
	1600 2750 4750 2750
Wire Wire Line
	1600 2750 1600 2900
Connection ~ 1600 2000
Wire Wire Line
	1600 2000 3850 2000
Wire Wire Line
	1600 2000 1600 2150
Wire Wire Line
	1400 2000 1600 2000
$Comp
L Device:R_Small R?
U 1 1 664822A5
P 1700 2150
F 0 "R?" V 1596 2150 50  0001 C CNN
F 1 "100k" V 1596 2150 50  0000 C CNN
F 2 "" H 1700 2150 50  0001 C CNN
F 3 "~" H 1700 2150 50  0001 C CNN
	1    1700 2150
	0    1    1    0   
$EndComp
$EndSCHEMATC
