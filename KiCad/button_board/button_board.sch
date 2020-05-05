EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Switch:SW_MEC_5G_LED SW1
U 1 1 5E9B25D6
P 4000 3200
F 0 "SW1" H 4000 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 4000 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 4000 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 4000 3500 50  0001 C CNN
	1    4000 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5G_LED SW2
U 1 1 5E9B3AAB
P 4700 3200
F 0 "SW2" H 4700 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 4700 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 4700 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 4700 3500 50  0001 C CNN
	1    4700 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5G_LED SW3
U 1 1 5E9B4786
P 5200 3200
F 0 "SW3" H 5200 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 5200 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 5200 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 5200 3500 50  0001 C CNN
	1    5200 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5E9B6539
P 3950 3600
F 0 "J2" V 4058 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 4058 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 3950 3600 50  0001 C CNN
F 3 "~" H 3950 3600 50  0001 C CNN
	1    3950 3600
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5E9B85F4
P 4050 2600
F 0 "J1" V 4158 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 4158 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 4050 2600 50  0001 C CNN
F 3 "~" H 4050 2600 50  0001 C CNN
	1    4050 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 3200 3800 3400
Wire Wire Line
	3800 3400 3950 3400
Wire Wire Line
	4200 3200 4200 3400
Wire Wire Line
	4200 3400 4050 3400
Wire Wire Line
	3800 3100 3800 2800
Wire Wire Line
	3800 2800 3950 2800
Wire Wire Line
	4200 3100 4200 2800
Wire Wire Line
	4200 2800 4050 2800
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5E9BD35E
P 4750 2600
F 0 "J3" V 4858 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 4858 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 4750 2600 50  0001 C CNN
F 3 "~" H 4750 2600 50  0001 C CNN
	1    4750 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5E9BDC0A
P 4650 3600
F 0 "J4" V 4758 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 4758 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 4650 3600 50  0001 C CNN
F 3 "~" H 4650 3600 50  0001 C CNN
	1    4650 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4500 3200 4500 3400
Wire Wire Line
	4500 3400 4650 3400
Wire Wire Line
	4750 3400 4900 3400
Wire Wire Line
	4900 3400 4900 3200
Wire Wire Line
	4900 3100 4900 2800
Wire Wire Line
	4900 2800 4750 2800
Wire Wire Line
	4650 2800 4500 2800
Wire Wire Line
	4500 2800 4500 3100
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 5E9BEDBF
P 5250 2600
F 0 "J5" V 5358 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 5358 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 5250 2600 50  0001 C CNN
F 3 "~" H 5250 2600 50  0001 C CNN
	1    5250 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J6
U 1 1 5E9BF55D
P 5150 3600
F 0 "J6" V 5258 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 5258 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 5150 3600 50  0001 C CNN
F 3 "~" H 5150 3600 50  0001 C CNN
	1    5150 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5000 3200 5000 3400
Wire Wire Line
	5000 3400 5150 3400
Wire Wire Line
	5250 3400 5400 3400
Wire Wire Line
	5400 3400 5400 3200
Wire Wire Line
	5150 2800 5000 2800
Wire Wire Line
	5000 2800 5000 3100
Wire Wire Line
	5400 3100 5400 2800
Wire Wire Line
	5400 2800 5250 2800
$Comp
L Switch:SW_MEC_5G_LED SW4
U 1 1 5E9C426B
P 5700 3200
F 0 "SW4" H 5700 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 5700 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 5700 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 5700 3500 50  0001 C CNN
	1    5700 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J8
U 1 1 5E9C4275
P 5750 2600
F 0 "J8" V 5858 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 5858 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 5750 2600 50  0001 C CNN
F 3 "~" H 5750 2600 50  0001 C CNN
	1    5750 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J7
U 1 1 5E9C427F
P 5650 3600
F 0 "J7" V 5758 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 5758 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 5650 3600 50  0001 C CNN
F 3 "~" H 5650 3600 50  0001 C CNN
	1    5650 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5500 3200 5500 3400
Wire Wire Line
	5500 3400 5650 3400
Wire Wire Line
	5750 3400 5900 3400
Wire Wire Line
	5900 3400 5900 3200
Wire Wire Line
	5650 2800 5500 2800
Wire Wire Line
	5500 2800 5500 3100
Wire Wire Line
	5900 3100 5900 2800
Wire Wire Line
	5900 2800 5750 2800
$Comp
L Switch:SW_MEC_5G_LED SW5
U 1 1 5E9C9D91
P 6200 3200
F 0 "SW5" H 6200 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 6200 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 6200 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 6200 3500 50  0001 C CNN
	1    6200 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J10
U 1 1 5E9C9D9B
P 6250 2600
F 0 "J10" V 6358 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 6358 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 6250 2600 50  0001 C CNN
F 3 "~" H 6250 2600 50  0001 C CNN
	1    6250 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J9
U 1 1 5E9C9DA5
P 6150 3600
F 0 "J9" V 6258 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 6258 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 6150 3600 50  0001 C CNN
F 3 "~" H 6150 3600 50  0001 C CNN
	1    6150 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6000 3200 6000 3400
Wire Wire Line
	6000 3400 6150 3400
Wire Wire Line
	6250 3400 6400 3400
Wire Wire Line
	6400 3400 6400 3200
Wire Wire Line
	6150 2800 6000 2800
Wire Wire Line
	6000 2800 6000 3100
Wire Wire Line
	6400 3100 6400 2800
Wire Wire Line
	6400 2800 6250 2800
$Comp
L Switch:SW_MEC_5G_LED SW6
U 1 1 5E9CC11B
P 6700 3200
F 0 "SW6" H 6700 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 6700 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 6700 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 6700 3500 50  0001 C CNN
	1    6700 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_MEC_5G_LED SW7
U 1 1 5E9CC125
P 7200 3200
F 0 "SW7" H 7200 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 7200 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 7200 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7200 3500 50  0001 C CNN
	1    7200 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J12
U 1 1 5E9CC12F
P 6750 2600
F 0 "J12" V 6858 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 6858 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 6750 2600 50  0001 C CNN
F 3 "~" H 6750 2600 50  0001 C CNN
	1    6750 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J11
U 1 1 5E9CC139
P 6650 3600
F 0 "J11" V 6758 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 6758 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 6650 3600 50  0001 C CNN
F 3 "~" H 6650 3600 50  0001 C CNN
	1    6650 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6500 3200 6500 3400
Wire Wire Line
	6500 3400 6650 3400
Wire Wire Line
	6750 3400 6900 3400
Wire Wire Line
	6900 3400 6900 3200
Wire Wire Line
	6900 3100 6900 2800
Wire Wire Line
	6900 2800 6750 2800
Wire Wire Line
	6650 2800 6500 2800
Wire Wire Line
	6500 2800 6500 3100
$Comp
L Connector:Conn_01x02_Male J14
U 1 1 5E9CC14B
P 7250 2600
F 0 "J14" V 7358 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 7358 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 7250 2600 50  0001 C CNN
F 3 "~" H 7250 2600 50  0001 C CNN
	1    7250 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J13
U 1 1 5E9CC155
P 7150 3600
F 0 "J13" V 7258 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 7258 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 7150 3600 50  0001 C CNN
F 3 "~" H 7150 3600 50  0001 C CNN
	1    7150 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7000 3200 7000 3400
Wire Wire Line
	7000 3400 7150 3400
Wire Wire Line
	7250 3400 7400 3400
Wire Wire Line
	7400 3400 7400 3200
Wire Wire Line
	7150 2800 7000 2800
Wire Wire Line
	7000 2800 7000 3100
Wire Wire Line
	7400 3100 7400 2800
Wire Wire Line
	7400 2800 7250 2800
$Comp
L Switch:SW_MEC_5G_LED SW8
U 1 1 5E9CC167
P 7700 3200
F 0 "SW8" H 7700 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 7700 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 7700 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 7700 3500 50  0001 C CNN
	1    7700 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J16
U 1 1 5E9CC171
P 7750 2600
F 0 "J16" V 7858 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 7858 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 7750 2600 50  0001 C CNN
F 3 "~" H 7750 2600 50  0001 C CNN
	1    7750 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J15
U 1 1 5E9CC17B
P 7650 3600
F 0 "J15" V 7758 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 7758 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 7650 3600 50  0001 C CNN
F 3 "~" H 7650 3600 50  0001 C CNN
	1    7650 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7500 3200 7500 3400
Wire Wire Line
	7500 3400 7650 3400
Wire Wire Line
	7750 3400 7900 3400
Wire Wire Line
	7900 3400 7900 3200
Wire Wire Line
	7650 2800 7500 2800
Wire Wire Line
	7500 2800 7500 3100
Wire Wire Line
	7900 3100 7900 2800
Wire Wire Line
	7900 2800 7750 2800
$Comp
L Switch:SW_MEC_5G_LED SW9
U 1 1 5E9CC18D
P 8200 3200
F 0 "SW9" H 8200 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 8200 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 8200 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 8200 3500 50  0001 C CNN
	1    8200 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J18
U 1 1 5E9CC197
P 8250 2600
F 0 "J18" V 8358 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 8358 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 8250 2600 50  0001 C CNN
F 3 "~" H 8250 2600 50  0001 C CNN
	1    8250 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J17
U 1 1 5E9CC1A1
P 8150 3600
F 0 "J17" V 8258 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 8258 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 8150 3600 50  0001 C CNN
F 3 "~" H 8150 3600 50  0001 C CNN
	1    8150 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8000 3200 8000 3400
Wire Wire Line
	8000 3400 8150 3400
Wire Wire Line
	8250 3400 8400 3400
Wire Wire Line
	8400 3400 8400 3200
Wire Wire Line
	8150 2800 8000 2800
Wire Wire Line
	8000 2800 8000 3100
Wire Wire Line
	8400 3100 8400 2800
Wire Wire Line
	8400 2800 8250 2800
$Comp
L Switch:SW_MEC_5G_LED SW10
U 1 1 5E9D85DB
P 8700 3200
F 0 "SW10" H 8700 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 8700 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 8700 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 8700 3500 50  0001 C CNN
	1    8700 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J20
U 1 1 5E9D85E5
P 8750 2600
F 0 "J20" V 8858 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 8858 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 8750 2600 50  0001 C CNN
F 3 "~" H 8750 2600 50  0001 C CNN
	1    8750 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J19
U 1 1 5E9D85EF
P 8650 3600
F 0 "J19" V 8758 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 8758 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 8650 3600 50  0001 C CNN
F 3 "~" H 8650 3600 50  0001 C CNN
	1    8650 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8500 3200 8500 3400
Wire Wire Line
	8500 3400 8650 3400
Wire Wire Line
	8750 3400 8900 3400
Wire Wire Line
	8900 3400 8900 3200
Wire Wire Line
	8650 2800 8500 2800
Wire Wire Line
	8500 2800 8500 3100
Wire Wire Line
	8900 3100 8900 2800
Wire Wire Line
	8900 2800 8750 2800
$Comp
L Switch:SW_MEC_5G_LED SW11
U 1 1 5E9D8601
P 9200 3200
F 0 "SW11" H 9200 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 9200 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 9200 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9200 3500 50  0001 C CNN
	1    9200 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J22
U 1 1 5E9D860B
P 9250 2600
F 0 "J22" V 9358 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 9358 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 9250 2600 50  0001 C CNN
F 3 "~" H 9250 2600 50  0001 C CNN
	1    9250 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J21
U 1 1 5E9D8615
P 9150 3600
F 0 "J21" V 9258 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 9258 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 9150 3600 50  0001 C CNN
F 3 "~" H 9150 3600 50  0001 C CNN
	1    9150 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9000 3200 9000 3400
Wire Wire Line
	9000 3400 9150 3400
Wire Wire Line
	9250 3400 9400 3400
Wire Wire Line
	9400 3400 9400 3200
Wire Wire Line
	9150 2800 9000 2800
Wire Wire Line
	9000 2800 9000 3100
Wire Wire Line
	9400 3100 9400 2800
Wire Wire Line
	9400 2800 9250 2800
$Comp
L Switch:SW_MEC_5G_LED SW12
U 1 1 5E9DD249
P 9850 3200
F 0 "SW12" H 9850 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 9850 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 9850 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 9850 3500 50  0001 C CNN
	1    9850 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J24
U 1 1 5E9DD253
P 9900 2600
F 0 "J24" V 10008 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 10008 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 9900 2600 50  0001 C CNN
F 3 "~" H 9900 2600 50  0001 C CNN
	1    9900 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J23
U 1 1 5E9DD25D
P 9800 3600
F 0 "J23" V 9908 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 9908 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 9800 3600 50  0001 C CNN
F 3 "~" H 9800 3600 50  0001 C CNN
	1    9800 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9650 3200 9650 3400
Wire Wire Line
	9650 3400 9800 3400
Wire Wire Line
	9900 3400 10050 3400
Wire Wire Line
	10050 3400 10050 3200
Wire Wire Line
	9800 2800 9650 2800
Wire Wire Line
	9650 2800 9650 3100
Wire Wire Line
	10050 3100 10050 2800
Wire Wire Line
	10050 2800 9900 2800
$Comp
L Switch:SW_MEC_5G_LED SW13
U 1 1 5E9DD26F
P 10350 3200
F 0 "SW13" H 10350 3494 50  0000 C CNN
F 1 "SW_MEC_5G_LED" H 10350 3494 50  0001 C CNN
F 2 "LED_Button_Switch_THT:SW_PUSH_6mm" H 10350 3500 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 10350 3500 50  0001 C CNN
	1    10350 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J26
U 1 1 5E9DD279
P 10400 2600
F 0 "J26" V 10508 2644 50  0000 L CNN
F 1 "Conn_01x02_Male" H 10508 2690 50  0001 C CNN
F 2 "Connectors:PinHeader_1x02_P4.5mm_Horizontal" H 10400 2600 50  0001 C CNN
F 3 "~" H 10400 2600 50  0001 C CNN
	1    10400 2600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J25
U 1 1 5E9DD283
P 10300 3600
F 0 "J25" V 10408 3412 50  0000 R CNN
F 1 "Conn_01x02_Male" H 10408 3690 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 10300 3600 50  0001 C CNN
F 3 "~" H 10300 3600 50  0001 C CNN
	1    10300 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10150 3200 10150 3400
Wire Wire Line
	10150 3400 10300 3400
Wire Wire Line
	10400 3400 10550 3400
Wire Wire Line
	10550 3400 10550 3200
Wire Wire Line
	10300 2800 10150 2800
Wire Wire Line
	10150 2800 10150 3100
Wire Wire Line
	10550 3100 10550 2800
Wire Wire Line
	10550 2800 10400 2800
$EndSCHEMATC
