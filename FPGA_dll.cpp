// FPGA_dll.cpp : Defines the entry point for the DLL application.
//

//#undef WINCE
//#define DEBUG 
#define FPGA_BUS_TESTS

#include <windows.h>
#include <commctrl.h>
#include <winbase.h>
#include <WinIoCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "protocolThread.hpp"
#include "Freeze/Freeze.h" 

#include "debug.h" 

#ifdef WINCE

#include "FPGA_dll.h"
#include "FPGA_dll_func.h"

#include "FPGA_func_defs.h"

#include "MathHelpers.h"
#include "BScanHelper\BScanHelper.h"

#endif

void AScanFileWrite(LPCVOID lpParam); //test Ascan wr file
void AScanFileInit (void);//test Ascan wr file
void FPGADeinit(void); //

int DebugOutActive = 0; //if(DebugOutActive) printf

int AScanShiftVal = 110; //сдвиг А-скана по OY

int bus_ok = 1;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved 
					 )
{
	if(DebugOutActive) printf("In DLL main\n");
//	DEBUGMSG(TRUE,( TEXT(" FpgaDll: in file %s funk %s at line %d \r\n"), TEXT(__FILE__) , TEXT(__FUNCTION__) , __LINE__ ));

	switch (ul_reason_for_call) 
    {
        case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls((HINSTANCE)hModule);  
			if(DebugOutActive) printf("FPGA_dll - DLL_PROCESS_ATTACH\n");
			if(DebugOutActive) printf("DllMain_build_date: %s at %s\n", __DATE__, __TIME__);

			DEBUGMSG(TRUE,( TEXT("\r\n\r\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n")));
			DBG_SHOW_DATE;
			DBG_SHOW_FUNC;
			
			break;
        case DLL_THREAD_ATTACH:
			if(DebugOutActive) printf("FPGA_dll - DLL_THREAD_ATTACH\n");
			DBG_SHOW_FUNC;
			break;
        case DLL_THREAD_DETACH:
			if(DebugOutActive) printf("FPGA_dll - DLL_THREAD_DETACH\n");
			DBG_SHOW_FUNC;
			break;
        case DLL_PROCESS_DETACH:
			if(DebugOutActive) printf("FPGA_dll - DLL_PROCESS_DETACH\n");
			DBG_SHOW_FUNC;
			FPGADeinit();
            break;
    }
    return TRUE;
}

#ifdef WINCE


FPGACommunication FPGA;

DWORD WINAPI ThreadSoft_AScan(LPVOID lpParameter);
DWORD WINAPI ThreadSoft_BScan(LPVOID lpParameter);


int AScan_not_freesed = 1;
int BScan_not_freesed = 1;

buffer CursorElevationBuff;

void test(void)
{

}

DWORD WINAPI ThreadSoft_BScan(LPVOID lpParameter)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , lpParameter);
	DBG_SHOW_FUNC;

	int i = LCD_WIDTH;
	while(1)
	{
		Sleep(20);
	if(BScan_not_freesed)
	{
		FPGA.setAScanWrCS(64+128);

		//Просто горизонтальный курсор индицирующий прямо на А-скане момент превышения
		FPGA.getGateMeasureTFirst(0, elevation);

		FPGA.setCursorX(0, (elevation-1000)/4/4);
		//FPGA.setCursorY(0, ToLcdScaleY(activeScheme->gates[0].levelPercent)); //TODO: fix
		FPGA.setCursorY(0,  AScanShiftVal-50/*ToLcdScaleY(activeScheme->gates[0].levelPercent)*/);
		////////////////////////////////////////////////////////////////

		CursorElevationBuff.buff_сd.excess = elevation;//Заполнение данными о превышении для отправки наверх и расчёта маркеров

		BSkan_errase_colomn(i);
		//А тут - сами точки Б-скана
		if(elevation != 0)
		{			
			FPGA.setCursorX(1, i);
			//FPGA.setCursorY(1, VAL_MIRROR_X((elevation-1000)/4/5));//LCD_MAX_ASCAN_Y_RANGE LCD_ASCAN_Y_BIAS
			FPGA.setCursorY(1, VAL_MIRROR_X(ToLcdScaleX(elevation/2+100)));//LCD_MAX_ASCAN_Y_RANGE LCD_ASCAN_Y_BIAS

			FPGA.setAScanBuff(1, 3);
		}
		i--;
		if(i==0)
		{
			i = LCD_WIDTH; /*BSkan_errase(); */
		}
	}
	}
	return 0;
}

#endif WINCE


void SettingsHndl (int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;
	if(n)
	{
	#ifdef WINCE

		int s_Сompress = 0;
		int s_AdcDelay = 1300;
		int s_Amplification = 700;
		int s_StrobeStart = 1000;
		int s_StrobeEnd  = 4500;
		int s_StrobeLevel  = 400;

		FPGA.setSignalCompress(s_Сompress);
		FPGA.setSignalADCDelay(s_AdcDelay);

		for(int i = 0; i< LCD_WIDTH-1; i++)
		{
			FPGA.setDACGain(s_Amplification);
		}

		FPGA.setGateStart(0, s_StrobeStart);
		FPGA.setGateEnd(0, s_StrobeEnd);
		//FPGA.setGateLevel(0, s_StrobeLevel);

	#endif WINCE
	}
}

///bug here
void startAScan(int n) 
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE

		DWORD dwThreadId = 0;
		HANDLE hAScan = NULL;

		//HardAScan_Start
FPGA.setGenSel(GEN4); //3
FPGA.setAnalogChSwich(CH4);//4

FPGA.setSignalADCDelay(1300);
FPGA.setSignalCompress(2);//Compress //3 //>>IN_SET
FPGA.setSignalDetector(1);//Detector = pos+neg 
FPGA.setDACGain(654);

FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt

//hAScan = CreateThread(NULL, 0, ThreadSoft_AScan, (LPVOID)0, 0, &dwThreadId);

	#endif WINCE
	}
}

int InMultiChMode_f = 0;

void onAScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE
		startAScan(1);// DELETE IT!

		AScanShiftVal = 10; //сдвинули вниз экрана
		//FPGA.setApainter(1);
		FPGA.setScanMode(0x00); //Set manual ch control
		FPGA.setAScanEnAddr(1);//on

	#endif WINCE
	}
}
void offAScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE
		//FPGA.setApainter(0);//
		FPGA.setScanMode(0x0F); //back to auto ch inc
		FPGA.setAScanEnAddr(0);//off

	#endif WINCE
	}
}


//=================================================================================================
void startBScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE

		DWORD dwThreadId = 0;
//for debug		HANDLE hBScan = CreateThread(NULL, 0, ThreadSoft_BScan, (LPVOID)0, 0, &dwThreadId); 

	#endif WINCE
	}
}

void onBScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE

	//	AScanShiftVal = 125;// сдвинули А-скан вверх
		//Sleep(5000);
	//	FPGA.setAScanEnAddr(2+64+128); //64+128 for B-scan

	#endif WINCE
	}
}
void offBScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE

		//FPGA.setAScanEnAddr(0);

	#endif WINCE
	}
}

int Gain_tmp = 0;
void SetChannelParams(WORD channel, WORD delay, WORD CZone, WORD Gain)
{
	FPGA_Write(ADC_DELAY_DR ,delay);
	FPGA_Write(ADC_CZONEDUR_DR ,CZone);
	FPGA_Write(DAC_GAIN_DR ,Gain);

	FPGA_Write(AN_CH_CSR ,channel);
	FPGA.setScanMode(0);
	FPGA.setScanMode(1);
}
	
static int ChGenResArr[8] = {5, 0, 1, 2, 6, 3, 7, 4};
void SetScanChannel(int num) //channel setter for AUTOSCAN mode
{	
	int chPhy = ChGenResArr[num];
	DBG_SHOW_FUNC;
	DEBUGMSG(TRUE, (TEXT("F_DLL: num: %u = chPhy: %u \r\n"), num, chPhy));

//	FPGA.setScanMode(0); //channel autoinc off	
	FPGA.setAnalogChSwich(chPhy);	//BUG HERE!!!
	FPGA.setGenSel(chPhy);

	if(InMultiChMode_f)
	{
	SetChannelParams(chPhy, 0, 0xFFFF, Gain_tmp);//FIX:

	FPGA.setScanMode(0xFF);
	DEBUGMSG(TRUE, (TEXT("----FIX: Default setCzone = 0xFFFF, setDACGain = %u, setSignalADCDelay = 0, setSignalCompress = 2 \r\n"), Gain_tmp));//TODO: rewrite dbg string
	}
}

//////////////------DEBUG_TESTS-------------------
void TestIRQ_En(void)
{
FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt
FPGA.MainSyncEn(1);//SyncCtrl_nENABLE - on
}

void TestIRQ_Dis(void)
{
FPGA.MainSyncEn(0);//SyncCtrl_nENABLE - oFF
FPGA.setSyncSource(SyncStop);
}
//////////////------DEBUG_TESTS-------------------


void ToFpgaDllSend(int with_fpga, int funk, int val)
{	
	int with_fpga_1 = with_fpga;
	int funk_1 = funk;
	int val_1 = val;

	if(DebugOutActive) printf("(UNDER_CE: %i, From: %s, funk = %i, val = %i )\n", with_fpga, TEXT(__FUNCTION__) ,  funk, val);
	DBG_SHOW_FUNC;
	DEBUGMSG(TRUE, (TEXT("		with_fpga = %u, funk = %u, val = %u \r\n"),  with_fpga, funk, val));//TODO: rewrite dbg string

#ifdef WINCE
	if((with_fpga) && (bus_ok))
	{ 
		switch(funk) 
		{
		case F_FPGA_INIT:		if(DebugOutActive) printf("FPGAinit_case_start\n"); 
								FPGA.systemReset();//restore init values 	FIX
								FPGAinit(with_fpga); 
								
								if(DebugOutActive) printf("FPGAinit_case_end\n");		
								break;
		
		case F_SIGNAL_COMPRESS: FPGA.setSignalCompress(val);			break;

		case F_ADC_DELAY:		//FPGA.setScanMode(0); //channel autoinc off	
								FPGA.setSignalADCDelay(val);
								break;
		case F_GAIN:			//FPGA.setScanMode(0); 
								FPGA.setDACGain(val);	
								Gain_tmp = val;
								break; //Усиление

		case F_ZONE_START:	  	FPGA.setGateStart(0, val);				break; //
		case F_ZONE_END:		FPGA.setGateEnd(0, val); 				break; //

		case F_SYNCH_SOURCE:	FPGA.setSyncFreq(val);				break;

		case F_GEN_SEL:			//FPGA.setScanMode(0); //channel autoinc off	
								FPGA.setGenSel(val);	
								break;
		case F_CH_SEL:			
								//FPGA.setScanMode(0); //channel autoinc off
								FPGA.setAnalogChSwich(val);		
								//FPGA.setSignalADCDelay(val);
								//FPGA.setScanMode(1); //channel autoinc on			
			break; 
		

		case F_LCD_MODE:		FPGA.setLcdMode(val);				break;//0-switch to cpu; 1-black screen 
		case F_A_PAINTER:		FPGA.setApainter(val);				break;//
		
		//case F_GET_TRACK_PARAMS:		FPGA.getTrackParams(val);break;//!!!!!!!

		case F_ASCAN_START:		startAScan(val);				break;//start thread
		case F_ASCAN_ON:		onAScan(val);				break;
		case F_ASCAN_OFF:		offAScan(val);				break;
				
		case F_BSCAN_START:		startBScan(val);				break;//start thread
		case F_BSCAN_ON:		onBScan(val);					break;
		case F_BSCAN_OFF:		offBScan(val);					break;

		case F_A_FREEZE_ON:		AScan_not_freesed = 0; FPGA.setSyncSource(0); AScanFileInit(); AScanFileWrite(0);break;//СТОП-КАДР ВКЛ (A-scan)
		case F_A_FREEZE_OFF:	AScan_not_freesed = 1; FPGA.setSyncSource(1); ReadDisplayAScan(); break;//СТОП-КАДР ВЫКЛ (A-scan)

		case F_B_FREEZE_ON:		BScan_not_freesed = 0;
								FPGA.systemReset();//restore init values 	FIX
		break;//СТОП-КАДР ВКЛ (B-scan)
		case F_B_FREEZE_OFF:	BScan_not_freesed = 1; break;//СТОП-КАДР ВЫКЛ (B-scan)

		//signal
		case F_DETECTOR_SET:	FPGA.setSignalDetector(val); break; //3 = Detector = pos+neg  
		case F_INTEGRATOR_SET:	FPGA.setSignalIntegration(val); break;//Количество точек, по которым интегрируется сигнал = 2^ IntegratorKoef //0=off
		case F_SOFT_SMOOTH:		while(0); /*obsolete*/ break;//вкл-выкл програмное сглаживание сигнала


		case F_TGC_ON:		FPGA.setTgcState(1);			 break;//
		case F_TGC_OFF:		FPGA.setTgcState(0);			 break;//
/*
		case F_FILTER_ON:	FPGA.setFilterEn(1);			 break;//
		case F_FILTER_OFF:	FPGA.setFilterEn(0);			 break;//
*/
		case F_SIGNAL_TEST:	FPGA.setSignalPattern(val);		 break;//

		case F_REGISTRATION_START:	while(0);			 break;//
		case F_REGISTRATION_STOP:	while(0);			 break;//
		case F_REGISTRATION_PAUSE:	while(0);			 break;//

		case F_TEST_IRQ_EN:	 TestIRQ_En();		 break;//
		case F_TEST_IRQ_DIS: TestIRQ_Dis();		 break;//

		case F_SCAN_CH_SET: SetScanChannel(val); break;//channel setter for AUTOSCAN mode

		case F_MULTI_CH: if(val == ON) {InMultiChMode_f = 1; FPGA.setScanMode(0x0F);}
						 else {InMultiChMode_f = 0; FPGA.setScanMode(0x00);}
						 break;//channel autoinc on/off for multi channel mode mode

		case F_MULTI_CH_GAIN_SET: 	while(0);	 break;//channel autoinc on/off for multi channel mode mode

		default:				if(DebugOutActive) printf("UNNOWN_FUNK %i\n", val); 
								DEBUGMSG(TRUE, (TEXT("UNNOWN_FUNK %u \r\n"),  val));
								Sleep(2);
								break;
		}
	}
	else
	{
		if(DebugOutActive) printf("FPGA emulated\n"); 
		DEBUGMSG(TRUE, (TEXT("FPGA emulated\r\n")));
		Sleep(2);
	}
#else
#endif //WINCE

}

buffer tmp_buff; 

int tmp_x1 = 0;
int tmp_x2 = 0;
DWORD tmp_elev = 0;
DWORD tmp_ch = 0;

void fpgaIO(int IO, int datatype, buffer& buff)
{
//if(DebugOutActive) printf("%s(IO: %i, datatype = %i)\n", __FUNCTION__ , IO, datatype);
int cnt = 0;
		switch(datatype)
		{
		case F_ASCAN_CURS: //Маркерные измерения на А-скане
return; //временно отключил TODO:
			if(IO == F_IN) //принимать номер канала(градус) для расчёта по градусу
			{
			tmp_buff = buff;
			tmp_x1   = tmp_buff.buff_сurs.posX1;
			tmp_x2   = tmp_buff.buff_сurs.posX2;
			//if(DebugOutActive) printf("F_IN posX1 = %i, posX2 = %i\n", tmp_x1 , tmp_x2);
			}
			if(IO == F_OUT) //в ответ на запрос от приложения
			{
				FPGA.getGateMeasureTFirst(0, tmp_elev);

				tmp_ch = tmp_buff.buff_сurs.value; //For debug


				if(tmp_elev) //есть превышение
				{
					DEBUGMSG(TRUE, (TEXT("F_DLL: MeasureTFirst %u \r\n"),  tmp_elev));
				//Заполняем поля локальной Dll-структуры данными о курсоре (о превышении заполнено раньше)
				CursorElevationBuff.buff_сd.dN = tmp_x1; //tmp_x1
				CursorElevationBuff.buff_сd.Kd = tmp_x2; //tmp_x1
				CursorElevationBuff.buff_сd.H  = -(3,26 * tmp_elev * cos(42))/2;
				CursorElevationBuff.buff_сd.L  = tmp_x2*10; 

				CursorElevationBuff.buff_сd.excess = tmp_elev; //CH bit //for test
				}
				else
				{
				CursorElevationBuff.buff_сd.dN = 0; 
				CursorElevationBuff.buff_сd.Kd = 0; 
				CursorElevationBuff.buff_сd.H  = 0;
				CursorElevationBuff.buff_сd.L  = 0; 

				CursorElevationBuff.buff_сd.excess = 0; //
				}

				buff = CursorElevationBuff; //отправляем готовые данные наверх приложению
			} break;

		case F_MOVE_DATA: // Track Sensor Speed, Direction, Distance
			if(IO == F_OUT)
			{
				cnt++;
				buff.buff_md.speed = cnt;
			//FPGA.getTrackParams(buff.buff_md.direction, buff.buff_md.tickCount, buff.buff_md.speed);
			if(DebugOutActive) printf("TS_OUT dir = %i, speed = %i, dist = %i\n", buff.buff_md.direction , buff.buff_md.speed, buff.buff_md.tickCount);
			}
		break;
		// IGOR
		case F_START_PROTOCOL:
			startProtocolThread(buff.buff_prot_header);
			break;
		case F_STOP_PROTOCOL:
			stopProtocolThread();
			break;
		//
		}
}
//if(DebugOutActive) printf("F_OUT posX1 = %i, posX2 = %i\n", tmp_x1 , tmp_x2);


void FPGAinit(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	if(n)
	{
#ifdef WINCE

		DBG_SHOW_FUNC;
		KoeffArr_init();
		FPGA_BUS_Init(); //DM bus timings

#ifdef FPGA_BUS_TESTS

		DEBUGMSG(TRUE, (TEXT("		F_DLL: Testing FPGA busses....\r\n")));

	//--------------------Tests section start----------------------------------------
		DBUS_TEST_result = FPGA_DBUS_TEST();
		//if(DebugOutActive) printf(" FPGA_D_BUS_TEST %s ", (DBUS_TEST_result?"ERR = ":"OK\n"));
		if(DBUS_TEST_result)
		{
			//if(DebugOutActive) printf("%d\n",DBUS_TEST_result); 
			error++;
		}
	        
		ABUS_TEST_result = FPGA_ABUS_TEST();
		//if(DebugOutActive) printf("FPGA_A_BUS_TEST %s ", (ABUS_TEST_result?"ERR = ":"OK\n"));
		if(ABUS_TEST_result)
		{
			//if(DebugOutActive) printf("%d\n",ABUS_TEST_result); 
			error++;
		}
	//--------------------Tests section end------------------------------------------

		if(error > 0)
		{
			//while(0) 
			{
				bus_ok = 0;
				//if(DebugOutActive) printf("FPGA_BUS_ERROR \r\n Initialising cancelled! \r\n");
				DEBUGMSG(TRUE, (TEXT("\r\n\r\n")));
				DEBUGMSG(TRUE, (TEXT("		F_DLL: ERROR\r\n")));
				DEBUGMSG(TRUE, (TEXT("		DBUS_TEST_result = %u \r\n"), DBUS_TEST_result));
				DEBUGMSG(TRUE, (TEXT("		ABUS_TEST_result = %u \r\n"), ABUS_TEST_result));
				DEBUGMSG(TRUE, (TEXT("   !!!!!!FPGA INIT CANCELLED!!!!!!\r\n")));
				DEBUGMSG(TRUE, (TEXT("\r\n\r\n")));
				Sleep(15000);
				return;
			}
		} 
		else
		{
			DEBUGMSG(TRUE, (TEXT("		F_DLL: A_bus & D_bus is OK\r\n")));
		}
#endif// FPGA_BUS_TESTS

		FPGA.systemReset();//restore init values 	

		AcousticScheme_DefaultInit();
		FPGA_Regs_deinit(); 

		//++++++++++++++++++++++++++++++++++++++++
		//------System start----------------------

		FPGA.setCR(0xFF);//ADC_off
//		FPGA.getCR(main_cr);//ADC_off
		//main_cr = FPGA_Read(Main_CR);

		FPGA.setCR(0x0A);//ADC_on and other perif on

		//=========LCD_CONTROLLER_INIT==============
		FPGA.setLcdMode(0);//0-switch to cpu; 1-black screen
		//==========================================

		System_init();
		//++++++++++++++++++++++++++++++++++++++++
		Ascan_init();
		//TEST_A-SCAN////////////////
		FPGA.setApainter(0);//off
		//draw_sine(20,2);
		//test_all_Ascans();
		////////////////////////////

		Gen_init();
		//now gen started

		Acust_init(); 
		Gates_init();

		FPGA.setGenSel(GEN4); //3
		FPGA.setAnalogChSwich(CH4);

		//FPGA.setProbeDelay(activeScheme->probe.delayUs);//5-2000  //FPGA_Write(_ProbeDelay ,1);////>>IN_SET

		//1500 - start delay
		//+2000 = middle of screen rdm 22 - 42* поверхность катания
		//+4500 = end
		FPGA.setSignalADCDelay(1400); 
		FPGA.setSignalCompress(0);//Compress //3 //>>IN_SET
		FPGA.setSignalDetector(0);//Detector = pos+neg 
		HardAScan_Start();

		//PrintAcousticScheme(*activeScheme);

		FPGA.setApainter(0);//FPGA A-scan drawing off

		// сначала инициализировать значениями, затем разрешать прорисовку. Иначе весь экран может залиться одним цветом
		FPGA.setCursorX(0, 100);
		FPGA.setCursorY(0, 100);

		FPGA.setCursorX(1, 200);
		FPGA.setCursorY(1, 200);

		FPGA.setApainter(0);//on

		for (int i = 0; i<8; i++)
		{
			SetChannelParams(i, i*10, 50000, 600);
		}
		/*
		//on CPU side
		int track = 0, tmp_track = 0;
		float time, speed;
		while(1)
		{
			Sleep(100);
			track = FPGA_Read(Track_position);

			time = FPGA_Read(Track_speed);
			if((time>0) && (tmp_track != track))
			{
				time /= 2;
				speed = 1/time;
				speed *= 1000; 
				if(DebugOutActive) printf("speed = %f m\s track = %i \n", speed, track);

				tmp_track = FPGA_Read(Track_position);
			}
		}
		//*/

		//gen 4 фонит с бпеп 2
		//ген 3 канал 3 не виден датчик
		//ген 6 канал 6 не виден датчик
		//ген 7 канал 7 не виден датчик

		//2-2 РС 1 пластина 
		//1-2 /2-1 РС - обе пластины
		//4-4 42 hand 

//////////////////////////////////////////////////////////////////
/*
		 WCHAR MainExePath[255];
		 LPCTSTR DstFolderPath = {L"\\Program Files\\Interface"};

		 LPCTSTR MainExeName = {L"FpgaInit.exe"};
		 //Make path to start main app
			wcscpy(MainExePath, DstFolderPath);
			wcscat(MainExePath, L"\\");
			wcscat(MainExePath, MainExeName);
		 STARTUPINFO cif;
		 ZeroMemory(&cif,sizeof(STARTUPINFO));
		 DEBUGMSG(TRUE,( TEXT("Starting FPGA init app\r\n")));
		 PROCESS_INFORMATION pi;
		 CreateProcess(MainExePath,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&cif,&pi);
//*/
	#endif WINCE
	}
}

void FPGADeinit (void) //выключение fpga и всей связаной периферии
{
	FPGA.setScanMode(0); //channel autoinc off
	FPGA.setGenSel(0); 
    FPGA.setAnalogChSwich(0);
} 