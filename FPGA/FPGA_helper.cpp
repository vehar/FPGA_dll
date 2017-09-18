#include "FPGA_helper.h"

//TODO: dock it!!!!
int duration(int del)
{
	return del<<3;
}
 
USHORT ToAdcScale (USHORT persent)
{
	return persent*(FPGA_ADC_SIGNAL_MAX_VAL/100);
}

 USHORT ToLcdScaleY (USHORT persent)
{
	return -persent*(LCD_MAX_ASCAN_Y_RANGE/100)+LCD_ASCAN_Y_BIAS;
}

   USHORT ToLcdScaleX (USHORT val)
{
	return val/(MAX_INPUT_VAL/LCD_HEIGHT);
}
 //TODO: частота в uS!

/////////////////////////////////////////////////////////////
void AcousticScheme_DefaultInit(void)
{
activeScheme = &Rdm11Scheme;

activeScheme->inqFreq  = 100;//FreqSync	//100
SyncState = SyncInt;//SyncCtrl
compress_val = 0;//Compress //+1 = +half screen left 5=rdm22 screen 
Channel = 4;//AN_CH_CSR
activeScheme->params.attenEn = 0; //AttenSw
activeScheme->params.gainDb = 10;

 //ACGAIN_DR_1!
 AScanEn = 1;//AScanEn

 activeScheme->signal.adcDelayUs = 1500; //AdcDelay? //30
 activeScheme->probe.delayUs = 4;//ProbeDelay? //4

 activeScheme->gates[0].levelPercent = 70; //currentlu not in percent, only in adc quants TODO: fix
}

#define GEN_CNT 7 //TODO:
void FPGA_Regs_deinit(void)//дополнить деинитом буфферов
{
FPGA.systemReset();//restore another init values 

//FPGA.setAScanColor(0, 0);
FPGA.setAnalogChSwich(0);
}


void Sync_init(void) 
{
//////Sync_init/////////////////////////////////////////////////////
FPGA.MainSyncEn(0);//SyncCtrl_nENABLE - oFF
FPGA.setSyncSource(SyncStop);
FPGA.setSyncFreq(activeScheme->inqFreq);//1000 //>>IN_SET
FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt
FPGA.MainSyncEn(1);//SyncCtrl_nENABLE - on
//////Sync_init/////////////////////////////////////////////////////
}



void Acust_init(void)
{
//////ANALOG_init start///////////////////////////////////////////////////////////

FPGA.setScanMode(1); //channel autoinc on
//резкое падение потребления!!! после этой записи
//FPGA.setSignalADCDelay(activeScheme->signal.adcDelayUs);//need test
//FPGA.setProbeDelay(activeScheme->probe.delayUs);//5-2000  //FPGA_Write(_ProbeDelay ,1);////>>IN_SET

//===================================================================================================
//int AcoustAmplification = 310;

//FPGA.setTgcStartAddr(0);
//FPGA.setTgcData(AcoustAmplification, AcoustAmplification, LCD_WIDTH-1);
//FPGA.setAcoustContGainCode(AcoustAmplification-100);//FPGA_Write(_AcousticContactGain ,10); //>>IN_SET //усиление в зоне ak_zone (fpga)
//FPGA.setTgcState(1);//TgcEn - при 0 - пропадала генерация //FPGA_Write(_TGC_EN_MR ,1); //>>IN_SET	
//===================================================================================================

FPGA.setAnalogChSwich(CH1);//FPGA_Write(_AN_CH_CSR ,0);//AN_CH_CSR //>>IN_SET
/*
FPGA.setFilterEn(0);//>>IN_SET
FPGA.setFilterCompress(1);//>>IN_SET
FPGA.resetFilterCoeffsAddr(); //>>IN_SET
FPGA.setFilterCoeffs(koef_array, 23);//
*/
//////Analog init end/////////////////////////////////////////////////////////////////
}

/*
void Gates_init(void)
{
	FPGA.setGateStart(0, 1000); 
	FPGA.setGateEnd(0, 4500);
	FPGA.setGateLevel(0, 100);// TODO: fix bug with max 500 in FPGA //ToAdcScale(activeScheme->gates[0].levelPercent)
}
*/


void KoeffArr_init(void)
{
koef_array	[	0	]	=	-75	;
koef_array	[	1	]	=	-113;
koef_array	[	2	]	=	-127;	
koef_array	[	3	]	=	-113;
koef_array	[	4	]	=	-75	;
koef_array	[	5	]	=	-20	;
koef_array	[	6	]	=	39	;
koef_array	[	7	]	=	90	;
koef_array	[	8	]	=	121	;
koef_array	[	9	]	=	126	;
koef_array	[	10	]	=	103	;
koef_array	[	11	]	=	58	;
koef_array	[	12	]	=	0	;
koef_array	[	13	]	=	-58	;
koef_array	[	14	]	=	-103;
koef_array	[	15	]	=	-126;
koef_array	[	16	]	=	-121;
koef_array	[	17	]	=	-90	;
koef_array	[	18	]	=	-39	;
koef_array	[	19	]	=	20	;
koef_array	[	20	]	=	75	;
koef_array	[	21	]	=	114	;
koef_array	[	22	]	=	128	;
}

//=======================================================================================
unsigned char   Value;

int FPGA_DBUS_TEST()
{
	int Ok = 0;
	unsigned short temp;
	for(int i=0;i<15;i++)
	{
		FPGA_Write(DBUS_TEST_DR ,(unsigned short)1<<i);
		temp = FPGA_Read(DBUS_TEST_DR);
		if(temp != 1<<i) Ok |= 1<<i;
	}
	return Ok;
}

int FPGA_ABUS_TEST()
{
	int Ok = 0;
 unsigned short temp;
	for(int i=0;i<7;i++)
	{
		FPGA_Read( 1 << i );  
		temp = FPGA_Read(ABUS_TEST_DR);
		if(temp != 1<<i) Ok |= 1<<i;
	}
	return Ok;
}


void Plott (WORD val)
{
	DEBUGMSG(TRUE, (TEXT("$%u;"), val));
}

int t = 0;


extern BOOL bufferTest(WORD* buffer, DWORD words, BOOL prevExists, WORD& prev);

float FpgaSpeedTest(DWORD iterations, DWORD wordNum, DWORD IrqPeriod, DWORD bufSize)
{
	UniDriver	uniDrv;
DWORD dwThreadId = 0; 

FPGA_Write(SYSTEM_RESET_CR ,1);
FPGA_Write(FSYNC_DR ,IrqPeriod*100);// t = val*100 [ms]
FPGA_Write(TEST_IRQ_CR ,wordNum);// num of words written in time from FSYNC_DR

FPGA.setSyncSource(INT_SYNC);//Select scan sync source

//FPGA_Write(SYNC_CR, STOP); //Track sensor synk

WORD* buf = new WORD[bufSize];

uniDrv.InitIRQ(65, bufSize, wordNum, 10000); 

printf("Async communication test started...\r\n");
printf("===================================\r\n");
printf("\r\n");
printf("Buffer size:\t\t%d machine words (16 bit)\r\n", bufSize);
printf("Packet size:\t\t%d machine words (16 bit) = %d Bytes\r\n", wordNum, wordNum*2);
printf("Num of iterations:\t%d \r\n", iterations);
printf("Interrupt frequency:\t%d Hz. (%d ms IRQ)\r\n", (1000 / IrqPeriod), IrqPeriod);

RWRegData_t regData;
regData.baseAddr = GPMC_CS1_BASE;
regData.offset = (ADC_DATA_DR)<<1;//TEST_CNT_DR /SPEED_TEST_DR
regData.value = 0;

HANDLE hFile = CreateFile(L"\\YAFFS_PART1\\file.txt",
			  GENERIC_READ | GENERIC_WRITE,
			  NULL,
			  NULL,
			  CREATE_ALWAYS,
			  NULL,
			  NULL);

if (hFile == INVALID_HANDLE_VALUE) {
	
	printf("Unable to create file!");
	Sleep(5000);
	return -2;
	
}

DWORD64 readed = 0;
DWORD writed = 0;
BOOL result = FALSE;
WORD prv = 0;
DWORD cnt = 0;
DWORD dataAmount = 0;
float tick = GetTickCount();

do {

	readed += uniDrv.ReadBufIRQ(&regData, (PBYTE)buf, bufSize * 2);
	result = bufferTest(buf, bufSize, FALSE, prv);
	WriteFile(hFile, buf, bufSize * 2, &writed, NULL);

	if (++cnt > iterations) break;

} while (true);

tick = GetTickCount() - tick;
//Time correction
//tick = tick - (IrqPeriod)*iterations;

dataAmount = (bufSize*2*iterations)/1024;
float Speed = dataAmount/(tick/1000);
 //-------------------------------------------------------------------------------
printf("RW_SPEED_TEST: Total data amount: %d kB\r\n", (dataAmount));
printf("RW_SPEED_TEST: Total elapsed time: %f mS\r\n", (tick));
printf("RW_SPEED_TEST: Approx. speed: %f kB/S\r\n", (Speed));
printf("RW_TEST: END\r\n");

/*
DEBUGMSG(TRUE, (TEXT("RW_SPEED_TEST: Elapsed time: %d mS\r\n"), (tick)));
DEBUGMSG(TRUE, (TEXT("RW_SPEED_TEST: Approx. speed: %d kB/S\r\n"), (Speed)));
DEBUGMSG(TRUE, (TEXT("RW_TEST: END\r\n")));
*/
/*
CloseHandle(hFile);
hFile = CreateFile(L"\\YAFFS_PART1\\file.txt",
			  GENERIC_READ | GENERIC_WRITE,
			  NULL, NULL, OPEN_ALWAYS,
 			  NULL,  NULL);


for (DWORD64 i = 0; i<readed; i+=bufSize*2)
{
ReadFile(hFile, buf, bufSize*2, NULL, NULL);
for(DWORD64 j = 0; j<bufSize; j++){Plott(buf[j]);}
Sleep(0);
}
//*/  
    
/*
CloseHandle(hFile);
hFile = CreateFile(L"\\YAFFS_PART1\\file.txt",
			  GENERIC_READ | GENERIC_WRITE,
			  NULL, NULL, OPEN_ALWAYS,
			  NULL, NULL);

 
	DWORD resultSize = 0;
	result = ReadFile(hFile,
					  buf,
					  2,
					  &resultSize,
					  NULL);
	DWORD count = 0;
	DWORD errorCount = 0;
	WORD prev = buf[0];
*/	

printf("\r\n");
printf("===================================\r\n");
printf("Async communication test done!\r\n");

uniDrv.ReleaseIRQ();
CloseHandle(hFile);

	return Speed;
}

//Test for Kirilov
void FpgaCycleRegTest(DWORD reg) //SCAN_MODE_CR
{
	int readedVal = 0;
	int result = 0;
	int cycleAmount = 10000;

	printf("RW_Test start. cycleAmount = %i, Reg addr %i \r\n", cycleAmount, reg);

for(int t = 0; t < cycleAmount; t++)
{
	for(int i = 0; i <= 15; i++)
	{
		FPGA_Write(reg ,i); 
		printf("WR:%i \t", i);
		readedVal = FPGA_Read(reg); 
		printf("RD:%i \r\n", readedVal);
		if(i != readedVal) 
		{
			result = 0;
			goto EXIT;
		}
		else
		{
			result = 1;
		}
		readedVal = 0;
	}
}
EXIT:
	//Message box Here
	if(result){	MessageBox(NULL, TEXT("test"), TEXT("SUCSESS!"), MB_OK); }
	else{	MessageBox(NULL, TEXT("test"), TEXT("FAIL!"), MB_OK);	}

	printf("RW_Test end!  \r\n");
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//Перенесено из FPGA_dll.cpp


void startAScan(int n) 
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	DBG_SHOW_FUNC;

	if(n)
	{
	#ifdef WINCE

		FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt

	#endif WINCE
	}
}


int InMultiChMode_f = 0; 



//device has 7 stages: 70>(1), 70<(2), 55>(3), 0(4), 55<(5), 42>(6), 42<(7)
void SetupGenChAccordance(void)
{
	DBG_SHOW_FUNC;

	FPGA.setGenChAccordance(1,7,6); //70>
	FPGA.setGenChAccordance(2,6,7); //70<
	FPGA.setGenChAccordance(3,4,4); //55>
	FPGA.setGenChAccordance(4,5,5); //0
	FPGA.setGenChAccordance(5,3,3); //55<
	FPGA.setGenChAccordance(6,1,1); //42>
	FPGA.setGenChAccordance(7,2,2); //42<
}

//=================================================================================================


int Gain_tmp = 0;

/*
void SetChannelParams(WORD channel, WORD delay, WORD CZone, WORD Gain)
{
	FPGA_Write(ADC_DELAY_DR ,delay);
	FPGA_Write(ADC_CZONEDUR_DR ,CZone);
	FPGA_Write(DAC_GAIN_DR ,Gain);

	FPGA_Write(AN_CH_CSR ,channel);
}
*/
	
//channel GenAnalog map
//									Generator, Analog reciever
static int GenAnalogFPGA_channels[8][2] = {{5, 5}, // RS-eho
										   {5, 5}, // RS-ztm
										   {1, 1}, // 42-forward
										   {2, 2}, // 42-backward 
										   {6, 7}, // 70-forward 4ch
										   {4, 4}, // 55-right
										   {7, 6}, // 70-backward
										   {3, 3}  // 55-left
													};
enum RAIL_TYPE{P43, P50, S49, P65, UIC60, P75}Rail;

typedef struct CZone_t
{
	WORD Delay;
	WORD Duration;
};

//channel zone-rail type map
//						 channel, RailType   P43	 P50     S49     P65    UIC60    P75
 CZone_t ZoneRailFPGA_channels[8][6]; /*=       {{0, 41},{0, 45},{0, 45},{0, 54},{0, 52},{0, 58}, // 1
										   {0, 41},{0, 45},{0, 45},{0, 54},{0, 52},{0, 58}, // 2
										   {18, 107},{18, 117},{18, 117},{18, 142},{18, 135},{18, 152}, // 3
										   {18, 107},{18, 117},{18, 117},{18, 142},{18, 135},{18, 152}, // 4
										   {5, 75},{5, 75},{5, 75},{5, 75},{5, 75},{5, 75}, // 5
										   {15, 50},{15, 50},{15, 50},{15, 50},{15, 50},{15, 50}, // 6
										   {5, 75},{5, 75},{5, 75},{5, 75},{5, 75},{5, 75}, // 7
										   {15, 50},{15, 50},{15, 50},{15, 50},{15, 50},{15, 50}, // 8
													};*/

void CZoneArrInit(void)
{
CZone_t CZ;
char ch;

/*
CZ.Delay = 0;
CZ.Duration = 41; ZoneRailFPGA_channels[ch][P43] = CZone;
CZ.Duration = 45; ZoneRailFPGA_channels[ch][P50] = CZone;
CZ.Duration = 45; ZoneRailFPGA_channels[ch][S49] = CZone;
CZ.Duration = 54; ZoneRailFPGA_channels[ch][P65] = CZone;
CZ.Duration = 52; ZoneRailFPGA_channels[ch][UIC60] = CZone;
CZ.Duration = 58; ZoneRailFPGA_channels[ch][P75] = CZone;
*/
ch = 1; //
ZoneRailFPGA_channels[ch-1][P65].Delay = 0;
ZoneRailFPGA_channels[ch-1][P65].Duration = 64;

ch = 2; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 0;
ZoneRailFPGA_channels[ch-1][P65].Duration = 64;

ch = 3; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 18;
ZoneRailFPGA_channels[ch-1][P65].Duration = 142;

ch = 4; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 18;
ZoneRailFPGA_channels[ch-1][P65].Duration = 142;

ch = 5; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 5;
ZoneRailFPGA_channels[ch-1][P65].Duration = 75;

ch = 6; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 15;
ZoneRailFPGA_channels[ch-1][P65].Duration = 50;

ch = 7; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 5;
ZoneRailFPGA_channels[ch-1][P65].Duration = 75;

ch = 8; 
ZoneRailFPGA_channels[ch-1][P65].Delay = 15;
ZoneRailFPGA_channels[ch-1][P65].Duration = 50;
}

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

#include "..\protocolThread.hpp"
#include "..\Protocol\RDMSIProtocol.hpp"
#include "..\Protocol\RDMSProtocolWriter.hpp"

extern void stopProtocolThread();
extern DWORD startProtocolThread(protocolData* pData);



#define NS_25 (25)// nano seconds
#define LCD_WIDTH (480) //pix

void SetScanChannel(int num) //channel setter for AUTOSCAN mode
{	
	int chPhy = InterfToPhyChDecode(num);
	int compression = 0;
	WORD scanByteDataAmount = 0;

	CZone_t CZoneCH;

	DBG_SHOW_FUNC;
	DEBUGMSG(TRUE, (TEXT("F_DLL: ch idx: %u TO-> chPhy: %u \r\n"), num, chPhy));

	CZoneCH = ZoneRailFPGA_channels[num][P65];
	DEBUGMSG(TRUE, (TEXT("-----> CZoneCH.Delay: %u CZoneCH.Duration: %u \r\n"), CZoneCH.Delay, CZoneCH.Duration));
// stopProtocolThread();

	currentChannel = num; //globaly

	if(chPhy == 5){	compression = 11;} //test //для РС и ЗТМ 12 для остальных 40
	else { compression = 39; }

	FPGA.setAnalogChSwich(chPhy);	//BUG was HERE!!!
	FPGA.setDACGain(Gain_tmp);

	scanByteDataAmount = (CZoneCH.Duration*1000)/((compression+1)*NS_25);
	
	wordNum = scanByteDataAmount/2/2;
	 
	FPGA.setAScanBuffSize(scanByteDataAmount); //according to CZone lenght (different for all channels)

	FPGA.setDrawStartTime(CZoneCH.Delay*NS_25);
	FPGA.setDrawCompress(compression);
	//FPGA.setDrawEndTime((CZoneCH.Delay + (compression+1)*LCD_WIDTH)*NS_25);

	FPGA.setCZoneDelay(chPhy, CZoneCH.Delay*1000/NS_25);
	FPGA.setChCompression(chPhy, compression);
	FPGA.setCZoneEnd(chPhy, (CZoneCH.Delay + CZoneCH.Duration)*1000/NS_25);

	stopProtocolThread();
	startProtocolThread(NULL);
}

/////////////-----DEBUG_TESTS-------------------
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
/////////////-----DEBUG_TESTS-------------------


void FPGAinit(int n)
{
	if(DebugOutActive) printf("%s(%i)\n", __FUNCTION__ , n);
	if(n)
	{
#ifdef WINCE

		DBG_SHOW_FUNC;
		FPGA_BUS_Init(); //DM bus timings

#ifdef FPGA_BUS_TESTS

		DEBUGMSG(TRUE, (TEXT("		F_DLL: Testing FPGA busses....\r\n")));

	 //-------------------Tests section start----------------------------------------
		DBUS_TEST_result = FPGA_DBUS_TEST();
		if(DBUS_TEST_result){error++;}
	        
		ABUS_TEST_result = FPGA_ABUS_TEST();
		if(ABUS_TEST_result){error++;}
	 //-------------------Tests section end------------------------------------------

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
		Sync_init();

		FPGA.setCR_HWGenPow(0);//disable gen
		FPGA.setScanMode(1); //channel autoinc on

		//FPGA.setAnalogChSwich(CH4);

		//1500 - start delay
		//+2000 = middle of screen rdm 22 - 42* поверхность катания
		//+4500 = end
		FPGA.setSignalADCDelay(1400); 

		//PrintAcousticScheme(*activeScheme);

		FPGA.setCR_DACen(ON); 
		CZoneArrInit();

		SetupGenChAccordance();

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
	FPGA.setCR_HWGenPow(0); 
    FPGA.setAnalogChSwich(0);
} 