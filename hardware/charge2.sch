EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:charge-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP4922-E/P U7
U 1 1 575C4AAC
P 3400 2350
F 0 "U7" H 3000 2750 50  0000 L CNN
F 1 "MCP4922-E/P" H 3550 2750 50  0000 L CNN
F 2 "IC:MCP4922-E_P" H 3400 2350 50  0001 C CIN
F 3 "" H 3400 2350 50  0000 C CNN
	1    3400 2350
	-1   0    0    -1  
$EndComp
$Comp
L MCP3425A0T-E/CH U6
U 1 1 575C4B07
P 2450 5950
F 0 "U6" H 2050 6250 50  0000 L CNN
F 1 "MCP3425A0T-E/CH" H 2550 6250 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 2450 5950 50  0001 C CIN
F 3 "" H 2450 5950 50  0000 C CNN
	1    2450 5950
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR047
U 1 1 575B22CF
P 2450 4950
F 0 "#PWR047" H 2450 4800 50  0001 C CNN
F 1 "+5V" H 2450 5090 50  0000 C CNN
F 2 "" H 2450 4950 50  0000 C CNN
F 3 "" H 2450 4950 50  0000 C CNN
	1    2450 4950
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR048
U 1 1 575B23D3
P 3500 1050
F 0 "#PWR048" H 3500 900 50  0001 C CNN
F 1 "+5V" H 3500 1190 50  0000 C CNN
F 2 "" H 3500 1050 50  0000 C CNN
F 3 "" H 3500 1050 50  0000 C CNN
	1    3500 1050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR049
U 1 1 575B2808
P 3500 3150
F 0 "#PWR049" H 3500 2900 50  0001 C CNN
F 1 "GND" H 3500 3000 50  0000 C CNN
F 2 "" H 3500 3150 50  0000 C CNN
F 3 "" H 3500 3150 50  0000 C CNN
	1    3500 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR050
U 1 1 575B2830
P 2450 6600
F 0 "#PWR050" H 2450 6350 50  0001 C CNN
F 1 "GND" H 2450 6450 50  0000 C CNN
F 2 "" H 2450 6600 50  0000 C CNN
F 3 "" H 2450 6600 50  0000 C CNN
	1    2450 6600
	1    0    0    -1  
$EndComp
Text GLabel 1150 2150 0    60   Input ~ 0
DA1
Text GLabel 1150 2550 0    60   Input ~ 0
DA2
$Comp
L GND #PWR051
U 1 1 575B2F4D
P 1500 6200
F 0 "#PWR051" H 1500 5950 50  0001 C CNN
F 1 "GND" H 1500 6050 50  0000 C CNN
F 2 "" H 1500 6200 50  0000 C CNN
F 3 "" H 1500 6200 50  0000 C CNN
	1    1500 6200
	1    0    0    -1  
$EndComp
Text GLabel 1150 5850 0    60   Input ~ 0
AD
$Comp
L GND #PWR052
U 1 1 575B4396
P 4800 5050
F 0 "#PWR052" H 4800 4800 50  0001 C CNN
F 1 "GND" H 4800 4900 50  0000 C CNN
F 2 "" H 4800 5050 50  0000 C CNN
F 3 "" H 4800 5050 50  0000 C CNN
	1    4800 5050
	1    0    0    -1  
$EndComp
$Comp
L C C17
U 1 1 575B43DD
P 1800 5450
F 0 "C17" H 1825 5550 50  0000 L CNN
F 1 "0.1u" H 1825 5350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1838 5300 50  0001 C CNN
F 3 "" H 1800 5450 50  0000 C CNN
	1    1800 5450
	1    0    0    -1  
$EndComp
$Comp
L C C20
U 1 1 575B476A
P 3900 1550
F 0 "C20" H 3925 1650 50  0000 L CNN
F 1 "0.1u" H 3925 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3938 1400 50  0001 C CNN
F 3 "" H 3900 1550 50  0000 C CNN
	1    3900 1550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR053
U 1 1 575B486E
P 3900 1700
F 0 "#PWR053" H 3900 1450 50  0001 C CNN
F 1 "GND" H 3900 1550 50  0000 C CNN
F 2 "" H 3900 1700 50  0000 C CNN
F 3 "" H 3900 1700 50  0000 C CNN
	1    3900 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR054
U 1 1 575B4C2F
P 1800 5600
F 0 "#PWR054" H 1800 5350 50  0001 C CNN
F 1 "GND" H 1800 5450 50  0000 C CNN
F 2 "" H 1800 5600 50  0000 C CNN
F 3 "" H 1800 5600 50  0000 C CNN
	1    1800 5600
	1    0    0    -1  
$EndComp
$Comp
L NJM2825 U5
U 1 1 575B536F
P 1650 1400
F 0 "U5" H 1350 1650 50  0000 C CNN
F 1 "NJM2825" H 1650 1600 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 1650 1500 50  0001 C CIN
F 3 "" H 1650 1400 50  0000 C CNN
	1    1650 1400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR055
U 1 1 575B548C
P 1650 1850
F 0 "#PWR055" H 1650 1600 50  0001 C CNN
F 1 "GND" H 1650 1700 50  0000 C CNN
F 2 "" H 1650 1850 50  0000 C CNN
F 3 "" H 1650 1850 50  0000 C CNN
	1    1650 1850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR056
U 1 1 575B54DB
P 1000 650
F 0 "#PWR056" H 1000 500 50  0001 C CNN
F 1 "+5V" H 1000 790 50  0000 C CNN
F 2 "" H 1000 650 50  0000 C CNN
F 3 "" H 1000 650 50  0000 C CNN
	1    1000 650 
	1    0    0    -1  
$EndComp
$Comp
L R R22
U 1 1 575B5565
P 1000 900
F 0 "R22" V 1080 900 50  0000 C CNN
F 1 "20k" V 1000 900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 930 900 50  0001 C CNN
F 3 "" H 1000 900 50  0000 C CNN
	1    1000 900 
	1    0    0    -1  
$EndComp
Text Notes 900  1000 1    60   ~ 0
5%
$Comp
L R R27
U 1 1 575BAB03
P 5100 2000
F 0 "R27" V 5180 2000 50  0000 C CNN
F 1 "100" V 5100 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5030 2000 50  0001 C CNN
F 3 "" H 5100 2000 50  0000 C CNN
	1    5100 2000
	0    1    1    0   
$EndComp
$Comp
L R R28
U 1 1 575BABBC
P 5100 2250
F 0 "R28" V 5180 2250 50  0000 C CNN
F 1 "100" V 5100 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5030 2250 50  0001 C CNN
F 3 "" H 5100 2250 50  0000 C CNN
	1    5100 2250
	0    1    1    0   
$EndComp
$Comp
L R R29
U 1 1 575BAC86
P 5100 2500
F 0 "R29" V 5180 2500 50  0000 C CNN
F 1 "100" V 5100 2500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5030 2500 50  0001 C CNN
F 3 "" H 5100 2500 50  0000 C CNN
	1    5100 2500
	0    1    1    0   
$EndComp
$Comp
L R R30
U 1 1 575BAD43
P 5100 2750
F 0 "R30" V 5180 2750 50  0000 C CNN
F 1 "100" V 5100 2750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5030 2750 50  0001 C CNN
F 3 "" H 5100 2750 50  0000 C CNN
	1    5100 2750
	0    1    1    0   
$EndComp
$Comp
L R R26
U 1 1 575BBA30
P 4450 1850
F 0 "R26" V 4530 1850 50  0000 C CNN
F 1 "10k" V 4450 1850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 1850 50  0001 C CNN
F 3 "" H 4450 1850 50  0000 C CNN
	1    4450 1850
	1    0    0    -1  
$EndComp
Text Notes 4350 1950 1    60   ~ 0
5%
$Comp
L C C18
U 1 1 575CDDE3
P 2450 1650
F 0 "C18" H 2475 1750 50  0000 L CNN
F 1 "0.1u" H 2475 1550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2488 1500 50  0001 C CNN
F 3 "" H 2450 1650 50  0000 C CNN
	1    2450 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR057
U 1 1 575CDDE9
P 2450 1800
F 0 "#PWR057" H 2450 1550 50  0001 C CNN
F 1 "GND" H 2450 1650 50  0000 C CNN
F 2 "" H 2450 1800 50  0000 C CNN
F 3 "" H 2450 1800 50  0000 C CNN
	1    2450 1800
	1    0    0    -1  
$EndComp
Text GLabel 7150 2000 2    60   Input ~ 0
SDI
Text GLabel 7150 2250 2    60   Input ~ 0
SCK
Text GLabel 7150 2500 2    60   Input ~ 0
CS
Text GLabel 7150 2750 2    60   Input ~ 0
LDAC
Text GLabel 7000 5850 2    60   Input ~ 0
I2C_SCL
Text GLabel 7000 6050 2    60   Input ~ 0
I2C_SDA
$Comp
L AQM1602A DS1
U 1 1 57B1E68B
P 4150 4100
F 0 "DS1" H 3350 4500 50  0000 C CNN
F 1 "AQM1602A" H 4850 4500 50  0000 C CNN
F 2 "charge:AQM1602A" H 4150 4050 50  0000 C CIN
F 3 "" H 4150 4100 50  0000 C CNN
	1    4150 4100
	1    0    0    -1  
$EndComp
$Comp
L C C22
U 1 1 57B1E97D
P 4450 5200
F 0 "C22" H 4475 5300 50  0000 L CNN
F 1 "1u" H 4475 5100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4488 5050 50  0001 C CNN
F 3 "" H 4450 5200 50  0000 C CNN
	1    4450 5200
	1    0    0    -1  
$EndComp
$Comp
L C C19
U 1 1 57B1EE0B
P 3850 5200
F 0 "C19" H 3875 5300 50  0000 L CNN
F 1 "1u" H 3875 5100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3888 5050 50  0001 C CNN
F 3 "" H 3850 5200 50  0000 C CNN
	1    3850 5200
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 57B1EE8E
P 4150 5200
F 0 "C21" H 4175 5300 50  0000 L CNN
F 1 "1u" H 4175 5100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4188 5050 50  0001 C CNN
F 3 "" H 4150 5200 50  0000 C CNN
	1    4150 5200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR058
U 1 1 57B1FBB4
P 3050 4450
F 0 "#PWR058" H 3050 4300 50  0001 C CNN
F 1 "+5V" H 3050 4590 50  0000 C CNN
F 2 "" H 3050 4450 50  0000 C CNN
F 3 "" H 3050 4450 50  0000 C CNN
	1    3050 4450
	1    0    0    -1  
$EndComp
$Comp
L R R23
U 1 1 57B1FE43
P 2850 4800
F 0 "R23" V 2930 4800 50  0000 C CNN
F 1 "10k" V 2850 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2780 4800 50  0001 C CNN
F 3 "" H 2850 4800 50  0000 C CNN
	1    2850 4800
	1    0    0    -1  
$EndComp
$Comp
L R R24
U 1 1 57B1FF8F
P 3050 4800
F 0 "R24" V 3130 4800 50  0000 C CNN
F 1 "10k" V 3050 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2980 4800 50  0001 C CNN
F 3 "" H 3050 4800 50  0000 C CNN
	1    3050 4800
	1    0    0    -1  
$EndComp
$Comp
L R R25
U 1 1 57B20016
P 3250 4800
F 0 "R25" V 3330 4800 50  0000 C CNN
F 1 "10k" V 3250 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3180 4800 50  0001 C CNN
F 3 "" H 3250 4800 50  0000 C CNN
	1    3250 4800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR059
U 1 1 57B206E2
P 4800 4750
F 0 "#PWR059" H 4800 4600 50  0001 C CNN
F 1 "+5V" H 4800 4890 50  0000 C CNN
F 2 "" H 4800 4750 50  0000 C CNN
F 3 "" H 4800 4750 50  0000 C CNN
	1    4800 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2150 1150 2150
Wire Wire Line
	1150 2550 2800 2550
Wire Wire Line
	3500 2850 3500 3150
Wire Wire Line
	3500 1050 3500 1850
Wire Wire Line
	2450 4950 2450 5550
Wire Wire Line
	2450 6350 2450 6600
Wire Wire Line
	1500 6200 1500 6050
Wire Wire Line
	1500 6050 1950 6050
Wire Wire Line
	1150 5850 1950 5850
Wire Wire Line
	2950 5850 7000 5850
Wire Wire Line
	3550 4600 3550 5850
Wire Wire Line
	2950 5950 6350 5950
Wire Wire Line
	3900 1400 3900 1250
Wire Wire Line
	3500 1250 4450 1250
Connection ~ 3500 1250
Wire Wire Line
	1800 5300 1800 5100
Wire Wire Line
	1800 5100 2450 5100
Connection ~ 2450 5100
Wire Wire Line
	2450 1350 3300 1350
Wire Wire Line
	3300 1350 3300 1850
Wire Wire Line
	1000 1350 1200 1350
Wire Wire Line
	1000 1050 1000 1350
Wire Wire Line
	1000 750  1000 650 
Wire Wire Line
	2650 1350 2650 2850
Wire Wire Line
	2650 2850 3300 2850
Connection ~ 2650 1350
Wire Wire Line
	4000 2150 4700 2150
Wire Wire Line
	4700 2150 4700 2000
Wire Wire Line
	4700 2000 4950 2000
Wire Wire Line
	4950 2250 4000 2250
Wire Wire Line
	4000 2350 4700 2350
Wire Wire Line
	4700 2350 4700 2500
Wire Wire Line
	4700 2500 4950 2500
Wire Wire Line
	4000 2450 4600 2450
Wire Wire Line
	4600 2450 4600 2750
Wire Wire Line
	4600 2750 4950 2750
Wire Wire Line
	4000 2550 4450 2550
Wire Wire Line
	4450 2550 4450 2000
Wire Wire Line
	4450 1250 4450 1700
Connection ~ 3900 1250
Wire Wire Line
	5250 2000 7150 2000
Wire Wire Line
	5250 2250 7150 2250
Wire Wire Line
	5250 2500 7150 2500
Wire Wire Line
	5250 2750 7150 2750
Connection ~ 3550 5850
Wire Wire Line
	2450 1100 2450 1500
Connection ~ 2450 1350
Wire Wire Line
	1000 1100 2450 1100
Connection ~ 1000 1100
Wire Wire Line
	1650 1700 1650 1850
Wire Wire Line
	1650 1750 2100 1750
Wire Wire Line
	2100 1750 2100 1350
Connection ~ 1650 1750
Wire Wire Line
	7000 6050 6350 6050
Wire Wire Line
	6350 6050 6350 5950
Wire Wire Line
	4450 4600 4450 5050
Wire Wire Line
	4300 5700 4450 5700
Wire Wire Line
	4450 5700 4450 5350
Wire Wire Line
	4300 4600 4300 5700
Wire Wire Line
	4150 5050 4150 4600
Wire Wire Line
	3850 4600 3850 5050
Wire Wire Line
	4000 4600 4000 5700
Wire Wire Line
	3850 5700 4150 5700
Wire Wire Line
	4150 5700 4150 5350
Wire Wire Line
	3850 5350 3850 5700
Connection ~ 4000 5700
Wire Wire Line
	3700 4600 3700 5950
Connection ~ 3700 5950
Wire Wire Line
	3250 4550 3250 4650
Wire Wire Line
	2850 4550 3250 4550
Wire Wire Line
	2850 4550 2850 4650
Wire Wire Line
	3050 4450 3050 4650
Connection ~ 3050 4550
Wire Wire Line
	3250 4950 3250 5050
Wire Wire Line
	3250 5050 3400 5050
Wire Wire Line
	3400 5050 3400 4600
Wire Wire Line
	3050 4950 3050 5150
Wire Wire Line
	3050 5150 3550 5150
Connection ~ 3550 5150
Wire Wire Line
	2850 4950 2850 5250
Wire Wire Line
	2850 5250 3700 5250
Connection ~ 3700 5250
Wire Wire Line
	4800 5050 4800 4950
Wire Wire Line
	4800 4950 3850 4950
Connection ~ 3850 4950
Wire Wire Line
	4800 4750 4800 4850
Wire Wire Line
	4800 4850 4000 4850
Connection ~ 4000 4850
$EndSCHEMATC
