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
int AScanShiftVal = 110; //����� �-����� �� OY
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

int AScan_not_freesed = 1;

buffer CursorElevationBuff;


#endif WINCE


void SettingsHndl (int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;
	if(n)
	{
	#ifdef WINCE

		/*
		int s_�ompress = 0;
		int s_AdcDelay = 1300;
		int s_Amplification = 700;
		int s_StrobeStart = 1000;
		int s_StrobeEnd  = 4500;
		int s_StrobeLevel  = 400;

		//FPGA.setSignalCompress(s_�ompress);
		FPGA.setSignalADCDelay(s_AdcDelay);

		for(int i = 0; i< LCD_WIDTH-1; i++)
		{
			FPGA.setDACGain(s_Amplification);
		}

		FPGA.setGateStart(0, s_StrobeStart);
		FPGA.setGateEnd(0, s_StrobeEnd);
		//FPGA.setGateLevel(0, s_StrobeLevel);
*/
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
//FPGA.setSignalCompress(2);//Compress //3 //>>IN_SET
FPGA.setSignalDetector(1);//Detector = pos+neg 
FPGA.setDACGain(654);

FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt

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

		AScanShiftVal = 10; //�������� ���� ������

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


//=================================================================================================


int Gain_tmp = 0;
void SetChannelParams(WORD channel, WORD delay, WORD CZone, WORD Gain)
{
	FPGA_Write(ADC_DELAY_DR ,delay);
	FPGA_Write(ADC_CZONEDUR_DR ,CZone);
	FPGA_Write(DAC_GAIN_DR ,Gain);

	FPGA_Write(AN_CH_CSR ,channel);
}
	
//channel map
//									Generator, Analog reciever
static int GenAnalogFPGA_channels[8][2] = {{5, 5}, // RS-eho
										   {5, 5}, // RS-ztm
										   {1, 1}, // 42-forward
										   {2, 2}, // 42-backward 
										   {6, 7}, // 70-forward
										   {4, 4}, // 55-right
										   {7, 6}, // 70-backward
										   {3, 3}  // 55-left
													};
USHORT currentChannel = 0;

WORD InterfToPhyChDecode(WORD ch)
{
	return GenAnalogFPGA_channels[ch][1];
}

WORD InterfToPhyDACGainDecode(int val)
{
	//10mv = 1dB
	//10 DAC LSB = 1dB
	//Gain phy range = [-34..+62]dB. BUT DAC bias val = +40mV
	//so 34*10+40mV = 380
	return (val*10) + 380;
}

void SetScanChannel(int num) //channel setter for AUTOSCAN mode
{	
	int chPhy = InterfToPhyChDecode(num);
	
	DBG_SHOW_FUNC;
	DEBUGMSG(TRUE, (TEXT("F_DLL: ch idx: %u TO-> chPhy: %u \r\n"), num, chPhy));

	currentChannel = num; //globaly

	FPGA.setAnalogChSwich(chPhy);	//BUG was HERE!!!

	if(InMultiChMode_f) //
	{
	SetChannelParams(chPhy, 0, 0xFFFF, Gain_tmp);//FIX:
	
	if(chPhy == 5){FPGA.setChCompression(chPhy, 12);} //test //��� �� � ��� 12 ��� ��������� 40
	else {FPGA.setChCompression(chPhy, 40);}

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
		
		case F_SIGNAL_COMPRESS: {FPGA.setChCompression(InterfToPhyChDecode(currentChannel), val);} /*FPGA.setSignalCompress(val);*/			break;

		case F_ADC_DELAY:		FPGA.setSignalADCDelay(val);			break;

		case F_GAIN:			{Gain_tmp = val; //TODO: delete as obsolete
								FPGA.setChDacGain(InterfToPhyChDecode(currentChannel), /*InterfToPhyDACGainDecode(val)*/ val);	
								}break; //��������

		case F_ZONE_START:	  	FPGA.setGateStart(0, val);				break; //
		case F_ZONE_END:		FPGA.setGateEnd(0, val); 				break; //

		case F_SYNCH_SOURCE:	FPGA.setSyncFreq(val);				break;

		case F_CH_SEL:	    	FPGA.setAnalogChSwich(val);			break; 
		
		case F_LCD_MODE:		FPGA.setLcdMode(val);				break;//0-switch to cpu; 1-black screen 
		
		//case F_GET_TRACK_PARAMS:		FPGA.getTrackParams(val);break;//!!!!!!!

		case F_ASCAN_START:		startAScan(val);				break;//start thread
		case F_ASCAN_ON:		onAScan(val);				break;
		case F_ASCAN_OFF:		offAScan(val);				break; 
				
		case F_A_FREEZE_ON:		AScan_not_freesed = 0; FPGA.setSyncSource(0); AScanFileInit(); AScanFileWrite(0);break;//����-���� ��� (A-scan)
		case F_A_FREEZE_OFF:	AScan_not_freesed = 1; FPGA.setSyncSource(1); ReadDisplayAScan(); break;//����-���� ���� (A-scan)

		//signal
		case F_DETECTOR_SET:	FPGA.setSignalDetector(val); break; //3 = Detector = pos+neg  
		//case F_INTEGRATOR_SET:	FPGA.setSignalIntegration(val); break;//���������� �����, �� ������� ������������� ������ = 2^ IntegratorKoef //0=off

		case F_TGC_ON:		FPGA.setTgcState(1);			 break;//
		case F_TGC_OFF:		FPGA.setTgcState(0);			 break;//

		case F_SIGNAL_TEST:	FPGA.setSignalPattern(val);		 break;//

		case F_REGISTRATION_START:	while(0);			 break;//
		case F_REGISTRATION_STOP:	while(0);			 break;//
		case F_REGISTRATION_PAUSE:	while(0);			 break;//

		case F_TEST_IRQ_EN:	 TestIRQ_En();		 break;//
		case F_TEST_IRQ_DIS: TestIRQ_Dis();		 break;//

		case F_SCAN_CH_SET: SetScanChannel(val); break;//channel setter for AUTOSCAN (Multichannel) mode 

		case F_MULTI_CH: if(val == ON) // ��� ������ � ������ � ����� ���������������
						 {
							 InMultiChMode_f = 1; 
							 FPGA.setScanMode(1); 
							 FPGA.setAScanEn(1);//Ascan on
							 FPGA.setHWGenPow(ON);
						 }
						 else 
						 {
							 InMultiChMode_f = 0; 
							 FPGA.setScanMode(0); 
							 FPGA.setAScanEn(0);//Ascan off 
							 FPGA.setHWGenPow(OFF);
						 }
						 break;//channel autoinc on/off for multi channel mode mode

		case F_SINGLE_CH: if(val == ON) // ��� ������ � ������ � ����� ��������������
						 {
							 InMultiChMode_f = 0; 
							 FPGA.setScanMode(1); 
							 FPGA.setAScanEn(1);//Ascan on
							 FPGA.setHWGenPow(ON); 
						 }
						 else 
						 {
							 FPGA.setScanMode(0); 
							 FPGA.setAScanEn(0);//Ascan off 
							 FPGA.setHWGenPow(OFF);
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
		case F_ASCAN_CURS: //��������� ��������� �� �-�����
return; //�������� �������� TODO:
			if(IO == F_IN) //��������� ����� ������(������) ��� ������� �� �������
			{
			tmp_buff = buff;
			tmp_x1   = tmp_buff.buff_�urs.posX1;
			tmp_x2   = tmp_buff.buff_�urs.posX2;
			//if(DebugOutActive) printf("F_IN posX1 = %i, posX2 = %i\n", tmp_x1 , tmp_x2);
			}
			if(IO == F_OUT) //� ����� �� ������ �� ����������
			{
				FPGA.getGateMeasureTFirst(0, tmp_elev);

				tmp_ch = tmp_buff.buff_�urs.value; //For debug


				if(tmp_elev) //���� ����������
				{
					DEBUGMSG(TRUE, (TEXT("F_DLL: MeasureTFirst %u \r\n"),  tmp_elev));
				//��������� ���� ��������� Dll-��������� ������� � ������� (� ���������� ��������� ������)
				CursorElevationBuff.buff_�d.dN = tmp_x1; //tmp_x1
				CursorElevationBuff.buff_�d.Kd = tmp_x2; //tmp_x1
				CursorElevationBuff.buff_�d.H  = -(3,26 * tmp_elev * cos(42))/2;
				CursorElevationBuff.buff_�d.L  = tmp_x2*10; 

				CursorElevationBuff.buff_�d.excess = tmp_elev; //CH bit //for test
				}
				else
				{
				CursorElevationBuff.buff_�d.dN = 0; 
				CursorElevationBuff.buff_�d.Kd = 0; 
				CursorElevationBuff.buff_�d.H  = 0;
				CursorElevationBuff.buff_�d.L  = 0; 

				CursorElevationBuff.buff_�d.excess = 0; //
				}

				buff = CursorElevationBuff; //���������� ������� ������ ������ ����������
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
		if(DBUS_TEST_result){error++;}
	        
		ABUS_TEST_result = FPGA_ABUS_TEST();
		if(ABUS_TEST_result){error++;}
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

		FPGA.setCR(0xFF);//ADC_off
		FPGA.setCR(0x0A);//ADC_on and other perif on
		FPGA.setLcdMode(0);//LCD_CONTROLLER_INIT 0-switch to cpu; 1-black screen
		Sync_init();
		Gen_init();
		Acust_init(); 
		Gates_init();

		FPGA.setGenSel(GEN4); //3
		FPGA.setAnalogChSwich(CH4);

		//FPGA.setProbeDelay(activeScheme->probe.delayUs);//5-2000  //FPGA_Write(_ProbeDelay ,1);////>>IN_SET

		//1500 - start delay
		//+2000 = middle of screen rdm 22 - 42* ����������� �������
		//+4500 = end
		FPGA.setSignalADCDelay(1400); 
		//FPGA.setSignalCompress(0);//Compress //3 //>>IN_SET
		FPGA.setSignalDetector(0);//Detector = pos+neg 

		//PrintAcousticScheme(*activeScheme);

		FPGA.setCursorX(0, 100);
		FPGA.setCursorY(0, 100);

		FPGA.setCursorX(1, 200);
		FPGA.setCursorY(1, 200);

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

		//gen 4 ����� � ���� 2
		//��� 3 ����� 3 �� ����� ������
		//��� 6 ����� 6 �� ����� ������
		//��� 7 ����� 7 �� ����� ������

		//2-2 �� 1 �������� 
		//1-2 /2-1 �� - ��� ��������
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

void FPGADeinit (void) //���������� fpga � ���� �������� ���������
{
	FPGA.setScanMode(0); //channel autoinc off
	FPGA.setHWGenPow(0); 
    FPGA.setAnalogChSwich(0);
} 