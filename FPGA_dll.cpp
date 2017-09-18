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
#include "FPGA\FPGA_helper.h"
#endif

int bus_ok = 1;
int DebugOutActive = 0; //if(DebugOutActive) printf

void FPGADeinit(void); //

#ifdef WINCE

FPGACommunication FPGA;
buffer CursorElevationBuff;
int AScan_not_freesed = 1;

#endif WINCE

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


void onAScan(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE
		startAScan(1);// DELETE IT!

		FPGA.setScanMode(0); //Set manual ch control
		FPGA.setAScanEn(1);//on

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

		FPGA.setScanMode(1); //back to auto ch inc
		FPGA.setAScanEn(0);//off

	#endif WINCE
	}
}


void ToFpgaDllSend(int with_fpga, int funk, int val)
{	
	int with_fpga_1 = with_fpga;
	int funk_1 = funk;
	int val_1 = val;

	if(DebugOutActive) printf("(UNDER_CE: %i, From: %s, funk = %i, val = %i )\n", with_fpga, TEXT(__FUNCTION__) ,  funk, val);
	DBG_SHOW_FUNC;
	DEBUGMSG(TRUE, (TEXT("		with_fpga = %u, funk = %u, val = %u \r\n"),  with_fpga, funk, val));//TODO: rewrite dbg string

	//return;

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
		
		case F_SIGNAL_COMPRESS: {
									FPGA.setDrawCompress(val); //для ОДНОканального
									FPGA.setChCompression(InterfToPhyChDecode(currentChannel), val);
								} 		
								break;

		case F_ADC_DELAY:		{
									//FPGA.setSignalADCDelay(val);	
									FPGA.setDrawStartTime(val); //для ОДНОканального
								}break;

		case F_GAIN:			{
									Gain_tmp = val; 
									FPGA.setChDacGain(InterfToPhyChDecode(currentChannel), /*InterfToPhyDACGainDecode(val)*/ val);	
								}break; //Усиление

		//case F_ZONE_START:	  	FPGA.setGateStart(0, val);				break; //
		//case F_ZONE_END:		FPGA.setGateEnd(0, val); 				break; //

		case F_SYNCH_SOURCE:	FPGA.setSyncFreq(val);				break;

		case F_CH_SEL:	    	FPGA.setAnalogChSwich(val);			break; 
		
		//case F_GET_TRACK_PARAMS:		FPGA.getTrackParams(val);break;//!!!!!!!

		case F_ASCAN_START:		startAScan(val);				break;//start thread
		case F_ASCAN_ON:		onAScan(val);				break;
		case F_ASCAN_OFF:		offAScan(val);				break; 
				
		//case F_A_FREEZE_ON:		AScan_not_freesed = 0; FPGA.setSyncSource(0); AScanFileInit(); AScanFileWrite(0);break;//СТОП-КАДР ВКЛ (A-scan)
		//case F_A_FREEZE_OFF:	AScan_not_freesed = 1; FPGA.setSyncSource(1); ReadDisplayAScan(); break;//СТОП-КАДР ВЫКЛ (A-scan)

		//signal
		//case F_TGC_ON:		FPGA.setTgcState(1);			 break;//
		//case F_TGC_OFF:		FPGA.setTgcState(0);			 break;//

		case F_SIGNAL_TEST:	FPGA.setSignalPattern(val);		 break;//

		case F_REGISTRATION_START:	while(0);			 break;//
		case F_REGISTRATION_STOP:	while(0);			 break;//
		case F_REGISTRATION_PAUSE:	while(0);			 break;//

		//case F_TEST_IRQ_EN:	 TestIRQ_En();		 break;//
		//case F_TEST_IRQ_DIS: TestIRQ_Dis();		 break;//

		case F_SCAN_CH_SET: SetScanChannel(val); break;//channel setter for AUTOSCAN (Multichannel) mode 

		case F_MULTI_CH: if(val == ON) // при заходе и выходе с формы многоканального
						 {
							 InMultiChMode_f = 1;  
							 FPGA.setScanMode(1); 
							 FPGA.setAScanEn(1);//Ascan on
							 FPGA.setCR_HWGenPow(ON);
						 }
						 else 
						 {
							 InMultiChMode_f = 0; 
							 FPGA.setScanMode(0); 
							 FPGA.setAScanEn(0);//Ascan off 
							 FPGA.setCR_HWGenPow(OFF);
						 }
						 break;//channel autoinc on/off for multi channel mode mode

		case F_SINGLE_CH: if(val == ON) // при заходе и выходе с формы ОДНОканального
						 {
							 InMultiChMode_f = 0;   

							FPGA.setCR((1<<FMC_SCAN_MODE_b) | (1<<ASCAN_EN_b) | (1<<GEN_HW_EN_b) | (1<<FMC_DAC_EN_b)/*patch*/ );

							startProtocolThread(NULL); //BUG
						 }
						 else 
						 {
							 FPGA.setScanMode(0); 
							 FPGA.setAScanEn(0);//Ascan off 
							 FPGA.setCR_HWGenPow(OFF);

							 stopProtocolThread();
						 }
						 break;	

		default:				if(DebugOutActive) {printf("UNNOWN_FUNK %i\n", val);} 
								DEBUGMSG(TRUE, (TEXT("UNNOWN_FUNK %u \r\n"),  val));
								Sleep(2);
								break;
		}
	}
	else // Without FPGA or FPGA test fail
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
				//FPGA.getGateMeasureTFirst(0, tmp_elev);

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
