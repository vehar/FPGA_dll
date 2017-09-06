#ifndef UNIDRIVER_H
#define UNIDRIVER_H


#pragma once

#include <windows.h>
#include "driverHelper.h"
#include "../debug.h"

#define FILE_DEVICE_HAL	0x00000101
#define METHOD_BUFFERED          0
#define FILE_ANY_ACCESS          0

#define TIM_INT_OFFSET           9

//#define CTL_CODE(DeviceType, Function, Method, Access ) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define IOCTL_HAL_REQUEST_SYSINTR CTL_CODE(FILE_DEVICE_HAL, 38, METHOD_BUFFERED, FILE_ANY_ACCESS)


struct DEVICE_INT_DATA {

	DWORD				dwPin;
	DWORD				intWait;
	DWORD				bufSize;
	DWORD				accessSize;

};

class UniDriver  //general memory interface
{
	static HANDLE hDrv;
	static HANDLE Open();	
	
public:

	UniDriver();
	~UniDriver();

  
   static DWORD ReadReg(RWRegData_t* readData);
   static DWORD WriteReg(RWRegData_t* writeData);
   static void  DmaRead(WORD addr, WORD size);
   static HANDLE GetHDrv() {return hDrv;};

   DWORD WriteWORD(USHORT adr, USHORT val);
   DWORD ReadWORD(USHORT adr, USHORT& val);

   void WriteBuf(WORD Addr, USHORT* Buff, int size);
   void ReadBuf(WORD Addr, USHORT* Buff, int size);

	static void		InitIRQ(DWORD dwPin, DWORD bufSize, DWORD accessSize, DWORD dwWait);
	//static DWORD	ReadBufIRQ(RWRegData_t* readData, PBYTE buf, DWORD bufSize);
	static DWORD	ReadBufIRQ(RWRegData_t* readData, PBYTE buf, DWORD bufSize);
	static DWORD	ReadIRQ(RWRegData_t* readData);
	static void		ReleaseIRQ();

};


#endif UNIDRIVER_H