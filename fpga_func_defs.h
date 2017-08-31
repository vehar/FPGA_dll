#ifndef FPGA_VARIABLE_H
#define FPGA_VARIABLE_H

#define NO_VARIBLE					0
#define F_COORD_AFTER_CORR_KM		29
#define F_COORD_BEFORE_CORR_KM		1

#define F_SYNCH_SOURCE				2
	#define F_TO_DP					0
	#define F_INTERNAL				1
	
#define F_DIST_COUNT				3
	#define F_PLUS					1
	#define F_MINUS					0
	
#define F_COORD_BEFORE_CORR_PIKET	4
#define F_COORD_AFTER_CORR_PIKET	5
#define F_UGENV						6
#define F_GEN_FREQUENCY				7
#define F_NUM_OF_PULSES				8
#define F_FREQUENCY_SYNC			9
#define F_SELECT_A_ZONE				10

#define F_FORM						11
	#define F_DAMPER				0
	#define F_FMINUS				1
	#define F_ZERO					2
	#define F_FPLUS					3
	
#define F_LENGTH_MS					12

#define F_ARV						13
	//F_TRUE
	//F_FALSE
	
#define F_VRC						14
	#define F_FALSE					0
	#define F_TRUE					1
	
#define F_CUTOFF_PROC				15
#define F_AVERAGING					16
#define F_2TPM						17

#define F_A0						18
	//45
	//55
	//65
	//70
	
#define F_NPEP						19
#define F_KPDB						20
#define F_THICHNESS					21

#define F_SPEED						22
	//45
	//55
	//65
	//70
	
#define F_METHOD					23
	#define F_SPECTRAL				0
	#define F_SHADOW				1
	#define F_ECHO					2
	
#define F_ACOUST_CONTACT			24
	#define F_OFF					0
	#define f_ON					1
	
#define F_UNITS						25  //????????????? F_VOLUME

#define F_LARGE						26
	#define F_NO					0
	#define F_YES					1
	
#define F_SOUND_ASD					27
	//F_OFF
	//F_ON
	
#define F_LIGHT_ASD					28
	#define F_FALS					0
	#define F_TRUE					1

#define F_SIGNAL_COMPRESS			30
#define F_ADC_DELAY					31

#define F_GEN_SEL					32
#define F_CH_SEL					33
#define F_GAIN     					34
#define F_FPGA_INIT 				35
#define F_LCD_MODE 				    36
#define F_A_PAINTER				    37

#define F_ASCAN_START 				38
#define F_ASCAN_ON 				    39
#define F_ASCAN_OFF				    40

#define F_BSCAN_START				41

#define F_GET_TRACK_PARAMS			42

#define F_A_FREEZE_ON		    	43
#define F_A_FREEZE_OFF		    	44

#define F_B_FREEZE_ON		    	45
#define F_B_FREEZE_OFF		    	46

#define F_ZONE_START				47
#define F_ZONE_END					48

#define F_BSCAN_ON					49
#define F_BSCAN_OFF					50


//Регистр детектирования сигнала 
	/*0 – выкл; 
	1 – отрицательный сигнал; 
	2 – положительный сигнал; 
	3 – отрицательный + положительный сигнал	*/
#define F_DETECTOR_SET				51
	#define DETECT_OFF				0
	#define DETECT_NEG				2
	#define DETECT_POS				3
	#define DETECT_POS_NEG			1 //default



//Количество точек, по которым интегрируется сигнал = 2^ IntegratorKoef //0=off
#define F_INTEGRATOR_SET			52 //val = min 0 max 32. Default 3

#define F_SOFT_SMOOTH				53 //выкл/вкл програмного сглаживания сигнала. Default вкл 

#define F_APAINTER_SET				54 //ВЫБОР режима отрисовки Аскана (Hard on FPGA or Soft on CPU)
	#define APAINTER_OFF	0
	#define FPGA_APAINTER	1
	#define CPU_APAINTER	2


#define F_TGC_ON					55// вкл ВРЧ
#define F_TGC_OFF					56// ввыкл ВРЧ

#define F_FILTER_ON				57// вкл  фильтрация
#define F_FILTER_OFF			58// ввыкл  фильтрация

/*=============================================================================================
 * define для fpgaIO
 */

//	ключь IO для void LoaderFPGA::fpgaIO(int IO, int datatype, buffer& buff);
//	0 - передача данных в FPGA
//	1 - прием данных из FPGA
#define F_IN						0
#define F_OUT						1

//	ключь datatype для void LoaderFPGA::fpgaIO(int IO, int datatype, buffer& buff);
//	какие именно данные передаются
#define F_ASCAN_CURS				0	// координаты курсора если IO = 0, или данные по кусору если IO = 1
#define F_MOVE_DATA				    1	// запрос от FPGA структуры Датчика Пути (Скорость, направление, расстояние)
// HV - проверить конфликты с другими TODO:
// IGOR
#define F_START_PROTOCOL			2	// Начать запись протокола
#define F_STOP_PROTOCOL				3	// Оставить запись протокола
//
//-------------------------------------------------------

#define F_SIGNAL_TEST			59 // тестовые паттерны

// Работа с протоколом контроля
#define F_REGISTRATION_START	60			
#define F_REGISTRATION_STOP		61
#define F_REGISTRATION_PAUSE	62

#define F_TEST_IRQ_EN		63
#define F_TEST_IRQ_DIS   	64
#define F_SCAN_CH_SET   	65

#define F_MULTI_CH			66 //управление работой многоканального режима ХВ
	#define ON				1
	#define OFF				0

#define F_MULTI_CH_GAIN_SET 67 //установка усиления (порога) для выбраного канала в многоканальном режиме


#endif FPGA_VARIABLE_H