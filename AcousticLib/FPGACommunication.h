#ifndef FPGACOMMUNICATION_H
#define FPGACOMMUNICATION_H

#pragma once

//#include "CustomTypes.h"
#include <windows.h>
#ifdef WINCE
#include "../UniDriver/UniDriver.h"
#include "../GpioDriver/GpioDriver.h"
#include "../ExtBusFunc/ExtBusFunc.h" 
#endif

//#define WINCE

#include "../FPGA/FPGA_Registers.h" //all internal FPGA registers adresses

//Адаптер для Acustlib
#define Gate1Start_1 STROBE_1_STR_DR
#define Gate2Start_1 STROBE_2_STR_DR

#define Gate1End_1 STROBE_1_END_DR
#define Gate2End_1 STROBE_2_END_DR

#define Gate1Level STROBE_1_LVL_DR
#define Gate2Level STROBE_2_LVL_DR

#define Gate1TFirst_1 STROBE_1_1EL_DR
#define Gate2TFirst_1 STROBE_2_1EL_DR

#define Gate1TMax_1 STROBE_1_MAX_DR
#define Gate2TMax_1 STROBE_2_MAX_DR

#define Gate1T0_1 STROBE_1_ZERO_DR
#define Gate1T0_2 Strobe1ZeroCrossHi
#define Gate2T0_1 STROBE_2_ZERO_DR
#define Gate2T0_2 Strobe2ZeroCrossHi
//Адаптер для Acustlib


enum Sync{SyncStop = 0, SyncInt = 1, SyncExt = 2, SyncCont = 3};

#ifdef WINCE

class FPGACommunication
{
   UniDriver *gmi;
public:
	FPGACommunication();
	~FPGACommunication();
	UCHAR ReadDWORD(DWORD addr, DWORD addr1, DWORD &val);
	UCHAR WriteDWORD(DWORD addr, DWORD addr1, DWORD val);
	UCHAR WriteBuf32(DWORD addr, DWORD addr1, DWORD *val, int size);
	
//========================================RDM11 V.H.=======================================
void setADC(USHORT en);
void setDAC(USHORT en);
void setChDacGain(USHORT channel, USHORT gain);

void setSignalPattern(UINT val);
void MainSyncEn(UINT en);
void setAScanDrawMode(UINT val);
void setAScanEnAddr(UINT val);
void setAScanWrCS(UINT val);
void setAScanStartAddrWr(UINT val);
void resetAScanRamCntRd();
void setAScanColor(UINT color_addr, UINT color_val);
void setGenStartAddrWr(UINT val);
void setAScanBuff(UINT val, UINT val2);
void setFilterCompress( USHORT val);
void resetFilterCoeffsAddr();
void setScanMode(UINT val); // b0: 1 -  автоинкремент счётчика каналов; 0 - номер выставляется вручную
void setTgcStartAddr(UINT val);
void systemReset();
void setCursorX( int cursorIdx, UINT val);
void setCursorY( int cursorIdx, UINT val);
void setLcdMode(UINT val);
void setApainter(UINT val);

void setCR(UINT val);
void getCR(USHORT& val);

void getTrackParams(char& Direction, int& Position, float& Speed);
/*
void getTrackDirection(WORD& val);
void getTrackPosition(WORD& val);
void getTrackSpeed(WORD& val);
*/
void getAScanDrawMode(USHORT& val);

void setTgcData( USHORT val_1, USHORT val_2, int range);//!

UCHAR ReadBuf(DWORD addr, USHORT *Buff, int range);
UCHAR WriteBuf32(DWORD addr, DWORD addr1, UINT val, UINT val_1, int size);
//=========================================================================================


	void getMajorVersion(USHORT& val);
	void getSvnVersion(USHORT& val);

	void setSyncFreq(UINT freqInSamples);
	void setSyncSource(USHORT syncSource);

	void setSignalADCDelay(UINT val);
	void resetReadRamCounter();
	void getSignalData(USHORT* Buff, int size);

	void setSignalDataLen(USHORT val);
	void setSignalCompress(USHORT val);
	void setSignalInversion(USHORT val);
	void setSignalDetector(USHORT val);
	void setAnalogChSwich(USHORT val);
	void setSignalIntegration(USHORT val);
	void setAttenuator(USHORT val);

	void setFilterEn(USHORT val);
	void setFilterCoeffs(USHORT* Buff, int size);

	void setGenSel(USHORT val);

	
	void setAcoustContGainCode(USHORT val);
	void getAC_SUM_DR( USHORT& val); //acoustic contact

	void setProbeDelay(USHORT val);
	
	void setDACCh(USHORT val);
	void setDACGain(USHORT val);

	void setTgcState(USHORT val);
	void setTgcData(DWORD* Buff, int size);
	
	void setGateStart(int gateIdx, UINT val);
	void setGateEnd(int gateIdx, UINT val);
	void setGateLevel(int gateIdx, USHORT val);
	void getGateMeasureTFirst(int gateIdx, DWORD &val);
	void getGateMeasureTMax(int gateIdx, DWORD &val);
	void getGateMeasureTCros0( int gateIdx, DWORD &val);
	
};

#endif WINCE

#endif FPGACOMMUNICATION_H