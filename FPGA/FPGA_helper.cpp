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

FPGA.setAScanColor(0, 0);
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


void Gen_init (void) 
{
	FPGA.setHWGenPow(0);//disable gen
}


void Acust_init(void)
{
//////ANALOG_init start///////////////////////////////////////////////////////////

FPGA.setScanMode(1); //channel autoinc on
//резкое падение потребления!!! после этой записи
//FPGA.setSignalADCDelay(activeScheme->signal.adcDelayUs);//need test
//FPGA.setProbeDelay(activeScheme->probe.delayUs);//5-2000  //FPGA_Write(_ProbeDelay ,1);////>>IN_SET

//===================================================================================================
int AcoustAmplification = 310;

//FPGA.setTgcStartAddr(0);
//FPGA.setTgcData(AcoustAmplification, AcoustAmplification, LCD_WIDTH-1);
//FPGA.setAcoustContGainCode(AcoustAmplification-100);//FPGA_Write(_AcousticContactGain ,10); //>>IN_SET //усиление в зоне ak_zone (fpga)
//FPGA.setTgcState(1);//TgcEn - при 0 - пропадала генерация //FPGA_Write(_TGC_EN_MR ,1); //>>IN_SET	
//===================================================================================================

FPGA.setAnalogChSwich(CH4);//FPGA_Write(_AN_CH_CSR ,0);//AN_CH_CSR //>>IN_SET
/*
FPGA.setFilterEn(0);//>>IN_SET
FPGA.setFilterCompress(1);//>>IN_SET
FPGA.resetFilterCoeffsAddr(); //>>IN_SET
FPGA.setFilterCoeffs(koef_array, 23);//
*/
//////Analog init end/////////////////////////////////////////////////////////////////
}

void Gates_init(void)
{
	FPGA.setGateStart(0, 1000); 
	FPGA.setGateEnd(0, 4500);
	FPGA.setGateLevel(0, 100);// TODO: fix bug with max 500 in FPGA //ToAdcScale(activeScheme->gates[0].levelPercent)
}


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
//--------------------------------------------------------------------------------
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