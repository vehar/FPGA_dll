//#include "stdafx.h"
#include <windows.h>
#include "UniDriver.h"
#include "driverHelper.h"
#include "../DM3730_Types.h"
//#include <iostream>
//#include "IntLib.h"
//using namespace std;

HANDLE UniDriver::hDrv = 0;

UniDriver::UniDriver()
{
	hDrv = 0;
	Open();
}

UniDriver::~UniDriver()
{
 if(hDrv)
 {
	 CloseHandle(hDrv);
	 hDrv = 0;
 }
}

HANDLE UniDriver::Open()
{ 
  if(hDrv==NULL)
  {
	hDrv = CreateFile(_T("Uni1:"),
		GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
		0,	// Exclusive access*/
		NULL,	// No security attributes 
		OPEN_EXISTING,	// Required
		0,	// No overlapped I/O on CE
		NULL);	// NULL for com devices		
  }
  return hDrv;
}

DWORD UniDriver::ReadReg(RWRegData_t* readData)
{
	DWORD accepted = 0;
	if(hDrv)
		DeviceIoControl(hDrv, IOCTL_READREG, (LPVOID) readData, sizeof(RWRegData_t),(LPVOID)&readData->value, 4, &accepted, NULL);
	return accepted;
}

DWORD UniDriver::WriteReg(RWRegData_t* writeData)
{
	DWORD accepted = 0;
	if(hDrv)
		DeviceIoControl(hDrv, IOCTL_WRITEREG, (LPVOID) writeData, sizeof(RWRegData_t),NULL, 0, &accepted, NULL);
	return accepted;
}

DWORD UniDriver::WriteWORD(USHORT addr, USHORT val)
{
	volatile u32 Offset = addr*2;
  	RWRegData_t regData = {GPMC_CS1_BASE, 0, 0};
	regData.value = val;
	regData.offset = Offset;
	return WriteReg(&regData);	
}
DWORD UniDriver::ReadWORD(USHORT addr, USHORT& val)
{
	DWORD result;
	volatile u32 Offset = addr*2;
	RWRegData_t regData = {GPMC_CS1_BASE, 0, 0};
	regData.offset = Offset;
	result = ReadReg(&regData);
	val = regData.value;
	return result;
}

void UniDriver::WriteBuf(WORD Addr, USHORT* Buff, int size)
{

}
void UniDriver::ReadBuf(WORD Addr, USHORT* Buff, int size)
{

}

void UniDriver::InitIRQ(DWORD dwPin, DWORD bufSize, DWORD accessSize, DWORD dwWait) {

	DEVICE_INT_DATA intData;
	intData.dwPin		= dwPin;
	intData.intWait		= dwWait;
	intData.bufSize		= bufSize; //0x1FF //800
	intData.accessSize	= accessSize; //0xFF //100

	DeviceIoControl(hDrv, IOCTL_INTINIT, (LPVOID)&intData, sizeof(DEVICE_INT_DATA), NULL, NULL, NULL, NULL);

}

DWORD UniDriver::ReadBufIRQ(RWRegData_t* readData/*, PBYTE buf, DWORD bufSize*/) {

	DWORD readed = 0;

	if(hDrv) {

		//DeviceIoControl(hDrv, IOCTL_INTREADBUF, (LPVOID)readData, sizeof(RWRegData_t), (LPVOID)buf, bufSize, &readed, NULL);
		DeviceIoControl(hDrv, IOCTL_INTREADBUF, (LPVOID)readData, sizeof(RWRegData_t), NULL, NULL, &readed, NULL);
	
	}
	return readed;

}

DWORD UniDriver::ReadIRQ(RWRegData_t* readData) {

	DWORD readed = 0;

	if(hDrv) {

		DeviceIoControl(hDrv, IOCTL_INTREAD, (LPVOID)readData, sizeof(RWRegData_t), NULL, NULL, &readed, NULL);
	
	}
	return readed;

}

void UniDriver::ReleaseIRQ() {

	DeviceIoControl(hDrv, IOCTL_INTDEINIT, NULL, NULL, NULL, NULL, NULL, NULL);

}

