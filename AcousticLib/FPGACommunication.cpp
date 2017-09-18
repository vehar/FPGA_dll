//#include "stdafx.h"

#include "FPGACommunication.h"

#ifdef WINCE

FPGACommunication::FPGACommunication()
{
	DBG_SHOW_FUNC;
	gmi = new UniDriver();//General Memory Interface init
	ExtBusCs1Init();
}

FPGACommunication::~FPGACommunication()
{
	delete gmi;
}

void FPGACommunication::BitWR(WORD reg, char bit, char val)
{
	USHORT tmp = 0;
	gmi->ReadWORD(reg, tmp);

	if(bit>15){ASSERT_FAILED(bit);}

	if(val == 1)
    {
		tmp |= (1 << bit);
		gmi->WriteWORD(reg ,tmp);
	} 
    else if (val == 0)
    {
		tmp &= ~(1 << bit);
		gmi->WriteWORD(reg ,tmp);
	}
	else
	{
		ASSERT_FAILED(val);
	}
}
//========================================RDM11 V.H.=======================================
void FPGACommunication::systemReset()
{
	DBG_SHOW_FUNC_T("F_DLL: "); 

	gmi->WriteWORD(SYSTEM_RESET_CR ,1);
}

void FPGACommunication::setSignalPattern(UINT val)
{
     DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(SITEP_CR , val); 
} 

void FPGACommunication::MainSyncEn(UINT en)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), en));

	USHORT tmpReg = 0;

	gmi->ReadWORD(CONTROL_REG, tmpReg);

	if(en == 0) 
    {
		tmpReg |= (1 << FMC_SYNC_nEN_b);//SyncCtrl_nENABLE - oN
		gmi->WriteWORD(CONTROL_REG ,tmpReg);
	} 
    else if (en == 1)
    {
		tmpReg &= ~(1 << FMC_SYNC_nEN_b);//SyncCtrl_nENABLE - oFF
		gmi->WriteWORD(CONTROL_REG ,tmpReg);
	}
}

void FPGACommunication::setAScanEn(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	BitWR(CONTROL_REG, ASCAN_EN_b, val);
}

void FPGACommunication::setScanMode(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	BitWR(CONTROL_REG, FMC_SCAN_MODE_b, val);
}

void FPGACommunication::setCR(UINT val) //установка только единиц!!!
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	USHORT tmpReg = 0;
	gmi->ReadWORD(CONTROL_REG, tmpReg);
	tmpReg |= val;
	gmi->WriteWORD(CONTROL_REG ,val);
}

void FPGACommunication::getCR(USHORT& val)
{
	gmi->ReadWORD(CONTROL_REG, val);

	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));
}


void FPGACommunication::getTrackParams(char& Direction, int& Position, float& Speed)
{
	DBG_SHOW_FUNC_T("F_DLL: "); 

	USHORT time = 0;

	//gmi->ReadWORD(TrackDirection, Direction);
	Direction = 1; //TODO: fix it
	gmi->ReadWORD(TRACK_POS_DR, (USHORT&)Position);

	gmi->ReadWORD(TRACK_SPEED_DR, time);
	if((time>0) && (Direction))//any direction 
	{
		time /= 2;
		Speed = 1/time;
		Speed *= 1000; 
	}
}

//=========================================================================================
//A-scan drawing
void FPGACommunication::setDrawStartTime(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(DRAW_STARTTIME_DR, val);
}

void FPGACommunication::setDrawEndTime(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(DRAW_ENDTIME_DR, val);
}

void FPGACommunication::setDrawCompress(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(DRAW_COMPRESS_DR, val);
}

void FPGACommunication::setSyncFreq(UINT freqInSamples)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), freqInSamples));

	gmi->WriteWORD(FSYNC_DR, freqInSamples);
}

void FPGACommunication::setSyncSource(USHORT syncSource)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), syncSource));

	BitWR(CONTROL_REG, FMC_SYNC_SRC_b, syncSource);
}

void FPGACommunication::setCZoneDelay(USHORT channel, USHORT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("channel = %u val =%u \r\n"), channel, val));

	USHORT tmpReg = 0;
	USHORT Reg = 0;

	switch(channel)
	{
		case 1: Reg = ADC_DELAY_DR_1; break;
		case 2: Reg = ADC_DELAY_DR_2; break;
		case 3: Reg = ADC_DELAY_DR_3; break;
		case 4: Reg = ADC_DELAY_DR_4; break;
		case 5: Reg = ADC_DELAY_DR_5; break;
		case 6: Reg = ADC_DELAY_DR_6; break;
		case 7: Reg = ADC_DELAY_DR_7; break;
		case 8: Reg = ADC_DELAY_DR_8; break;
		default : return;
	}
	gmi->WriteWORD(Reg ,val);
}

void FPGACommunication::setCZoneEnd(USHORT channel, USHORT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("channel = %u val =%u \r\n"), channel, val));

	USHORT tmpReg = 0;
	USHORT Reg = 0;

	switch(channel)
	{
		case 1: Reg = ADC_DURATION_DR_1; break;
		case 2: Reg = ADC_DURATION_DR_2; break;
		case 3: Reg = ADC_DURATION_DR_3; break;
		case 4: Reg = ADC_DURATION_DR_4; break;
		case 5: Reg = ADC_DURATION_DR_5; break;
		case 6: Reg = ADC_DURATION_DR_6; break;
		case 7: Reg = ADC_DURATION_DR_7; break;
		case 8: Reg = ADC_DURATION_DR_8; break;
		default : return;
	}
	gmi->WriteWORD(Reg ,val);
}

void FPGACommunication::setChDacGain(USHORT channel, USHORT gain)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("channel = %u gain =%u \r\n"), channel, gain));
	
	USHORT tmpReg = 0;
	USHORT Reg = 0;

	switch(channel)
	{
		case 1: Reg = DAC_GAIN_DR_1; break;
		case 2: Reg = DAC_GAIN_DR_2; break;
		case 3: Reg = DAC_GAIN_DR_3; break;
		case 4: Reg = DAC_GAIN_DR_4; break;
		case 5: Reg = DAC_GAIN_DR_5; break;
		case 6: Reg = DAC_GAIN_DR_6; break;
		case 7: Reg = DAC_GAIN_DR_7; break;
		case 8: Reg = DAC_GAIN_DR_8; break;
		default : return;
	}
	gmi->WriteWORD(Reg ,gain);
}

//Установка соответствия в последовательности запуска между запуком генератора и выбором аналогового канала для приёма
void FPGACommunication::setGenChAccordance(USHORT seqNum, USHORT channel, USHORT gen)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("seqNum =%u channel =%u gen =%u \r"), seqNum, channel, gen));

	USHORT tmpReg = 0;
	USHORT Reg = 0;
	USHORT val = 0;

	switch(seqNum)
	{
		case 1: Reg = GEN_CH_ACCORD_DR_1; break;
		case 2: Reg = GEN_CH_ACCORD_DR_2; break;
		case 3: Reg = GEN_CH_ACCORD_DR_3; break;
		case 4: Reg = GEN_CH_ACCORD_DR_4; break;
		case 5: Reg = GEN_CH_ACCORD_DR_5; break;
		case 6: Reg = GEN_CH_ACCORD_DR_6; break;
		case 7: Reg = GEN_CH_ACCORD_DR_7; break;
		case 8: Reg = GEN_CH_ACCORD_DR_8; break;
		default : return;
	}
	val = (gen<<8) | (channel<<0); //младшая половина регистра для канала, старшая - для генератора
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("Reg val =%u\r\n"), val));
	gmi->WriteWORD(Reg ,val);
}

#define LO_HALF (0)
#define HI_HALF (1)

void FPGACommunication::setChCompression(USHORT channel, WORD compress)
{
	WORD RD_RegVal = 0;
	WORD WR_RegVal = 0;
	WORD Reg = 0;
	WORD Haif_f = 0;

	switch(channel)
	{
		case 1: Reg = COMPRESS_DR_1_2; Haif_f = LO_HALF; break;
		case 2: Reg = COMPRESS_DR_1_2; Haif_f = HI_HALF; break;
		case 3: Reg = COMPRESS_DR_3_4; Haif_f = LO_HALF; break;
		case 4: Reg = COMPRESS_DR_3_4; Haif_f = HI_HALF; break;
		case 5: Reg = COMPRESS_DR_5_6; Haif_f = LO_HALF; break;
		case 6: Reg = COMPRESS_DR_5_6; Haif_f = HI_HALF; break;
		case 7: Reg = COMPRESS_DR_7_8; Haif_f = LO_HALF; break;
		case 8: Reg = COMPRESS_DR_7_8; Haif_f = HI_HALF; break;
		default : return;
	}
	gmi->ReadWORD(Reg, RD_RegVal);

	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("CH =%u compress =%u \n\tRD_RegVal =%u \r"), channel, compress, RD_RegVal));
	if(Haif_f == LO_HALF)
	{
		RD_RegVal = RD_RegVal & 0xFF00; //clean Low reg half 
		WR_RegVal = RD_RegVal | (compress & 0xFF); //write masked value back to reg
	}
	else if(Haif_f == HI_HALF)
	{
		RD_RegVal = RD_RegVal & 0x00FF; //clean High reg half 
		WR_RegVal = RD_RegVal | ((compress & 0xFF)<<8); //write masked value back to reg
	}
	
	DEBUGMSG(TRUE, (TEXT("\tWR_RegVal =%u \r\n"), WR_RegVal));

	gmi->WriteWORD(Reg ,WR_RegVal);
}


void FPGACommunication::setCR_DACen(USHORT en)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), en));

	BitWR(CONTROL_REG, FMC_DAC_EN_b, en);
}

void FPGACommunication::setCR_ADCen(USHORT en)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), en));

	BitWR(CONTROL_REG, FMC_ADC_EN_b, en);
}

void FPGACommunication::setDACGain( USHORT val )
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(DAC_GAIN_DR, val);// TODO change reg
}

void FPGACommunication::setAcoustContGainCode( USHORT val )
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(ACGAIN_DR_1, val);
}

void FPGACommunication::setAnalogChSwich(USHORT val )
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(AN_CH_CSR, val);
}

 //------------------------------GENERATOR------------------------------------------
void FPGACommunication::setCR_HWGenPow(USHORT val ) //выбор ВЫХОДА активного генератора
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	BitWR(CONTROL_REG, GEN_HW_EN_b, val);
}

////
void FPGACommunication::setSignalADCDelay(UINT val)
{
	DBG_SHOW_FUNC_T("UNUSED ! F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(ADC_DELAY_DR, val);
}

void FPGACommunication::setAScanWrCS(UINT val)
{
	DBG_SHOW_FUNC_T("UNUSED ! F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(ASCAN_WR_CSR , val); 
}

void FPGACommunication::setAScanBuffSize(UINT val)
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), val));

	gmi->WriteWORD(ASCAN_BUFF_SIZE , val); 
}

 //--------------------------------------------------------------------------------
/*
void FPGACommunication::setFilterEn( USHORT val )
{
	gmi->WriteWORD(FILT_EN_CR, val);
}

void FPGACommunication::setFilterCoeffs( USHORT* Buff, int size )
{
	gmi->WriteWORD(FILT_COEFS_RST_CR, 0);
	gmi->WriteBuf(FILT_COEFS_DR, Buff, size);
}

void FPGACommunication::setTgcStartAddr(UINT val)
{
	gmi->WriteWORD(TGC_RAM_START_AR ,val);
}

void FPGACommunication::getSignalData( USHORT *Buff, int size )
{
	DBG_SHOW_FUNC_T("F_DLL: "); DEBUGMSG(TRUE, (TEXT("val %u \r\n"), size));

	ReadBuf(ADC_DATA_DR, Buff, size+1);
}

void FPGACommunication::setProbeDelay( USHORT val )
{
	gmi->WriteWORD(ADC_PEC_DELAY_DR, val);
}

void FPGACommunication::getMajorVersion( USHORT &val )
{
	gmi->ReadWORD(VERSION_DR, val);
}
*/
/*
///////////////////////////////////////////////////////////////////////////
void FPGACommunication::setGateStart( int gateIdx, UINT val )
{
 	DWORD StartAddr = (gateIdx ? Gate2Start_1 : Gate1Start_1);
	WriteDWORD(StartAddr, StartAddr+1, val);
}

void FPGACommunication::setGateEnd( int gateIdx, UINT val )
{
	DWORD StartAddr = (gateIdx ? Gate2End_1 : Gate1End_1);
	WriteDWORD(StartAddr, StartAddr+1, val);
}

void FPGACommunication::setGateLevel( int gateIdx, USHORT val )
{
	DWORD StartAddr = (gateIdx ? Gate2Level : Gate1Level);
	gmi->WriteWORD(StartAddr, val);
}

void FPGACommunication::getGateMeasureTFirst( int gateIdx, DWORD &val )
{
	DWORD StartAddr = (gateIdx ? Gate2TFirst_1 : Gate1TFirst_1);
	ReadDWORD(StartAddr, StartAddr+1, val);
}

void FPGACommunication::getGateMeasureTMax( int gateIdx, DWORD &val )
{
	DWORD StartAddr = (gateIdx ? Gate2TMax_1 : Gate1TMax_1);
	ReadDWORD(StartAddr, StartAddr+1, val);
}

void FPGACommunication::getGateMeasureTCros0( int gateIdx, DWORD &val )
{
	DWORD StartAddr = (gateIdx ? Gate2T0_1 : Gate1T0_1);
	ReadDWORD(StartAddr, StartAddr+1, val);
}


/////////////////////////////////////////////////////////

void FPGACommunication::setAScanColor(UINT color_addr, UINT color_val)
{
//	gmi->WriteWORD(AScanColor ,(color_addr<<8 | (color_val&0xFF)));
}

void FPGACommunication::setFilterCompress( USHORT val)
{
	//gmi->WriteWORD(FILT_COMPRES_DR, val);
}

void FPGACommunication::resetFilterCoeffsAddr()
{
	//gmi->WriteWORD(FILT_COEFS_RST_CR, 1);
}

void FPGACommunication::setTgcState( USHORT val )
{
	//gmi->WriteWORD(TGC_EN_MR, val);
}

void FPGACommunication::setTgcData( DWORD* Buff, int size)//!
{
//	gmi->WriteWORD(TGC_RAM_START_AR, 0);
//	WriteBuf32(TGC_RAM_DR, DacData2, Buff, size);
}

void FPGACommunication::setTgcData( USHORT val_1, USHORT val_2, int range)//! усиление или врч
{
//	gmi->WriteWORD(TGC_RAM_START_AR, 0);
//	WriteBuf32(TGC_RAM_DR, DacData2, val_1, val_2, range);
}
*/

UCHAR FPGACommunication::WriteBuf32(DWORD addr, DWORD addr1, DWORD *val, int size) //!
{
	for(int i=0;i<size;i++)
	{
		gmi->WriteWORD(addr, val[i]& 0xffff);
		gmi->WriteWORD(addr1, (val[i]>>16));
	}
	return 1;
}

UCHAR FPGACommunication::WriteBuf32(DWORD addr, DWORD addr1, UINT val, UINT val_1, int range) //!
{
	for(int i=0;i<range;i++)
	{
		gmi->WriteWORD(addr, val);
		gmi->WriteWORD(addr1, val_1);
	}
	return 1;
}

UCHAR FPGACommunication::ReadBuf(DWORD addr, USHORT *Buff, int range) //!
{
	WORD d1;

	for(int i=0;i<range;i++)
	{
		gmi->ReadWORD(addr, d1);
		Buff[i] = d1;
	}
	return 1;
}
/*
UCHAR FPGACommunication::ReadBuf32(DWORD addr,  int range) //!
{
	WORD d1,d2;

	for(int i=0;i<range;i++)
	{
		gmi->ReadWORD(addr, d1);
		gmi->ReadWORD(addr1, d2);
		val = d2<<16 | d1;
	}
	return 1;
}
*/
UCHAR FPGACommunication::WriteDWORD(DWORD addr, DWORD addr1, DWORD val)
{
	if(!gmi->WriteWORD(addr, val & 0xffff)) return 0;
	if(!gmi->WriteWORD(addr1, (val>>16))) return 0;
	return 1;
}

UCHAR FPGACommunication::ReadDWORD(DWORD addr, DWORD addr1, DWORD &val)
{
	WORD d1,d2;
	if(!gmi->ReadWORD(addr, d1)) return 0;
	if(!gmi->ReadWORD(addr1, d2)) return 0;
	val = d2<<16 | d1;
	return 1;
}

#endif