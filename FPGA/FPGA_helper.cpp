//#include "stdafx.h"

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


void System_init (void) 
{
//////System_init/////////////////////////////////////////////////////
FPGA.MainSyncEn(0);//SyncCtrl_nENABLE - oFF
FPGA.setSyncSource(SyncStop);
FPGA.setSyncFreq(activeScheme->inqFreq);//1000 //>>IN_SET
//FPGA.setSignalCompress(activeScheme->signal.compress);//1
FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt

FPGA.MainSyncEn(1);//SyncCtrl_nENABLE - on
//////System_init/////////////////////////////////////////////////////
}


void Ascan_init (void)
{
	//Bit    07  06  05 04  03 02  01 00
	//Data   R   R   R  G   G   G  B   B

FPGA.setAScanColor(1, GREEN);//R+G //yellow //A_SCAN_BACKGROUND //N.U.
FPGA.setAScanColor(2, DARK_RED);//G+B// aqua //-1 //FOR CURSORs COLOR TO!!!!!!!!!!
FPGA.setAScanColor(3, BLUE);///E3 - blue  //-2
FPGA.setAScanColor(4, AQUA);//Red //E0 //-4
FPGA.setAScanColor(5, RED);//Green //-8
FPGA.setAScanColor(6, YELLOW);//Blue //-16
FPGA.setAScanColor(7, GREEN);//Blue //-32
FPGA.setAScanColor(8, VIOLET);//R+G //yellow  //-64 B-scan!
}

void Gen_init (void) 
{
//////GEN_init//////////////////////////////////////////////////////////////////////////
	FPGA_Write(GEN_CH_CSR, 0);//disable gen
//	FPGA_Write(GEN_DELAY_DR_1, 0);
//	FPGA_Write(GEN_DURATION_DR_Adr, 8);//P-gen = 200ns + //gen clk_40M; 1 point = 25nS; 
//	FPGA_Write(GEN_CH_CSR, 1); //Enable gen
//////GEN_init////////////////////////////////////////////////////////////////////
}


void Acust_init(void)
{
//////ANALOG_init start///////////////////////////////////////////////////////////
FPGA.setSignalInversion(1);

/*!*/ FPGA.setSignalDetector(1);//Detector = off

FPGA.setScanMode(1); //channel autoinc on
//резкое падение потребления!!! после этой записи
FPGA.setSignalADCDelay(activeScheme->signal.adcDelayUs);//need test
FPGA.setProbeDelay(activeScheme->probe.delayUs);//5-2000  //FPGA_Write(_ProbeDelay ,1);////>>IN_SET
FPGA.setSignalCompress(compress_val);//Compress //3 //>>IN_SET

//===================================================================================================
int AcoustAmplification = 710;

FPGA.setTgcStartAddr(0);
FPGA.setTgcData(AcoustAmplification, AcoustAmplification, LCD_WIDTH-1);
FPGA.setAcoustContGainCode(AcoustAmplification-100);//FPGA_Write(_AcousticContactGain ,10); //>>IN_SET //усиление в зоне ak_zone (fpga)
FPGA.setTgcState(1);//TgcEn - при 0 - пропадала генерация //FPGA_Write(_TGC_EN_MR ,1); //>>IN_SET	
//===================================================================================================

FPGA.setAnalogChSwich(CH4);//FPGA_Write(_AN_CH_CSR ,0);//AN_CH_CSR //>>IN_SET

/*!*/ FPGA.setFilterEn(0);//>>IN_SET
FPGA.setFilterCompress(1);//>>IN_SET
FPGA.resetFilterCoeffsAddr(); //>>IN_SET
FPGA.setFilterCoeffs(koef_array, 23);//signal becomes = 1024
/*!*/FPGA.setSignalIntegration(0);//Количество точек, по которым интегрируется сигнал = 2^ IntegratorKoef //0=off
//////Analog init end/////////////////////////////////////////////////////////////////
}

void Gates_init(void)
{
FPGA.setGateStart(0, 1000); 
FPGA.setGateEnd(0, 4500);
FPGA.setGateLevel(0, 100);// TODO: fix bug with max 500 in FPGA //ToAdcScale(activeScheme->gates[0].levelPercent)
}

//управление автономной отрисовкой А-скана на FPGA
void HardAScan_Start(void)
{
	FPGA.setAScanEnAddr(1+2+4);
	FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt
	FPGA.setApainter(1);//on
}

void HardAScan_Stop(void)
{
	FPGA.setAScanEnAddr(1+2+4); 
	FPGA.setSyncSource(1);//SyncCtrl - on //SyncInt
	FPGA.setApainter(0);//off
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
		//FPGA_Write(127,(unsigned short)~1<<i);
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