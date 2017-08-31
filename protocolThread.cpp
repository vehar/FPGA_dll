#include <windows.h>
#include "winioctl.h"
#include <ctime>

#include "GpioDriver\gpio.h"
#include "UniDriver\UniDriver.h"

#include "protocolThread.hpp"
#include "Protocol\RDMSIProtocol.hpp"
#include "Protocol\RDMSProtocolWriter.hpp"


#include "FPGA\FPGA.h"
#include "FPGA\FPGA_Registers.h"

/* // VH - Is it necessary?
int fpgaDBus() {

	int Ok = 0;
	unsigned short temp;
	int Value1;
	for(int i=0;i<15;i++)
	{
		FPGA_Write(61,(unsigned short)1<<i);
		//FPGA_Write(127,(unsigned short)~1<<i);
		temp = FPGA_Read(61);
		if(temp != 1<<i) Ok |= 1<<i;
	}
	return Ok;
}

int fpgaABus() {

	int Ok = 0;
 unsigned short temp;
	int Value1;
	for(int i=0;i<7;i++)
	{
		FPGA_Read( 1 << i );  
		temp = FPGA_Read(62);
		if(temp != 1<<i) Ok |= 1<<i;
	}
	return Ok;
}
*/


BOOL bufferTest(WORD* buffer, DWORD words, BOOL prevExists, WORD& prev) {

	if (prevExists == TRUE) {

		if ((prev + 1) != buffer[0]) {

			DWORD dif = buffer[0] - prev;
			//printf("Error:\t%d\t==>\t%d\t(%d)\t*\r\n", prev, buffer[0], dif);
			return FALSE;

		}

	}

	for (DWORD i = 0; i < (words - 1); ++i) {

		WORD a = buffer[i];
		WORD b = buffer[i + 1];
		if ((WORD)(a + 1) != b) {

			DWORD dif = b - a;
			//printf("Error:\t%d\t==>\t%d\t(%d)\r\n", a, b, dif);
			return FALSE;
			
		}

	}

	prev = buffer[words - 1];

	return TRUE;

}


// Start protocol thread
DWORD startProtocolThread(const protocolData &pData) {

	// Check if thread exists
	if (protThread == NULL) {

		// Create execution info struct
		execStruct = new execInfo;

		// Allow to start thread function
		execStruct->threadExecuting = true;
		execStruct->pHeader = pData;

		// Thread ID
		// Just for safety because CreateThread may crash if lpIDThread = NULL
		DWORD threadID = 0;

		// Run protocol thread
		protThread = CreateThread(NULL,
								  0,
								  (LPTHREAD_START_ROUTINE)protocolThreadFunc,
								  execStruct,
								  0,
								  &threadID);

		// Thread was not created!!!
		if (protThread == NULL) {

			return -2;

		}

		//SetThreadPriority(protThread, THREAD_PRIORITY_ABOVE_NORMAL);

		return 0;

	}

	return -1;

}

// Stop protocol thread
void stopProtocolThread() {

	// Check if thread exists
	if (protThread != NULL) {

		// Stop thread execution
		execStruct->threadExecuting = false;

		// Wait for thread to stop
		//WaitForSingleObject(protThread, INFINITE);

		// Zero thread handler
		protThread = NULL;

		// Delete execution info struct
		delete execStruct;

	}

}

// Protocol thread
DWORD WINAPI protocolThreadFunc(LPVOID lpParam) {

	// Get pointer to 
	execInfo* exec = (execInfo*)lpParam;
	// Poiner to execution flag
	BOOL* execFlag = &exec->threadExecuting;
	// Pointer to header data
	protocolData* hdr = &exec->pHeader;

	//
	wchar_t fileName[256];

	RDMSHeader header;
	memcpy(header.headerSymbols, RDMS_HEADER_SYMBOLS, 4);
	header.gpsData.latLongDirection	= 0xFF;
	header.gpsData.latDegrees		= 0xFF;
	header.gpsData.latMinutes		= 0xFF;
	header.gpsData.latSeconds		= 0xFF;
	header.gpsData.longDegrees		= 0xFF;
	header.gpsData.longMinutes		= 0xFF;
	header.gpsData.longSeconds		= 0xFF;
	header.freeMemory				= hdr->memory;
	header.railType					= 0xAA;
	header.operatorNumber			= hdr->operatorN;

	RDMSIPacket* packet = NULL;

	switch(hdr->objectControl) {
	
		case 0:
			packet = new RDMSTrackCoordPacket;
			((RDMSTrackCoordPacket*)packet)->startByte					= RDMS_START_COORD_PACKET;
			((RDMSTrackCoordPacket*)packet)->endByte					= RDMS_START_COORD_PACKET;
			((RDMSTrackCoordPacket*)packet)->gpsData.latLongDirection	= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.latDegrees			= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.latMinutes			= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.latSeconds			= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.longDegrees		= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.longMinutes		= 0xFF;
			((RDMSTrackCoordPacket*)packet)->gpsData.longSeconds		= 0xFF;
			((RDMSTrackCoordPacket*)packet)->pathSensorData.km			= hdr->railRoad.km;
			((RDMSTrackCoordPacket*)packet)->pathSensorData.m			= hdr->railRoad.m;
			((RDMSTrackCoordPacket*)packet)->pathSensorData.sm			= 0x11;
			((RDMSTrackCoordPacket*)packet)->pathSensorData.mm			= 0x22;
			((RDMSTrackCoordPacket*)packet)->railNumber					= hdr->railRoad.road;
			((RDMSTrackCoordPacket*)packet)->thread						= 0xEE;
			((RDMSTrackCoordPacket*)packet)->stageName.nameLength		= wcslen(hdr->railRoad.peregon);
			memcpy(((RDMSTrackCoordPacket*)packet)->stageName.name, hdr->railRoad.peregon, 256);
			((RDMSTrackCoordPacket*)packet)->checkSum					= ((RDMSTrackCoordPacket*)packet)->calcCheckSum();

			header.controlType = RDMS_CONTROL_TYPE_TRACK;
			header.direction = hdr->railRoad.distanceÑount;
			break;

		case 1:
			packet = new RDMSKmStockPacket;

			((RDMSKmStockPacket*)packet)->startByte				= RDMS_KM_STOCK_PACKET;
			((RDMSKmStockPacket*)packet)->endByte				= RDMS_KM_STOCK_PACKET;
			((RDMSKmStockPacket*)packet)->position.km			= hdr->pokm.km;
			((RDMSKmStockPacket*)packet)->position.m			= hdr->pokm.m;
			((RDMSKmStockPacket*)packet)->position.trackNumber	= hdr->pokm.railNum;
			((RDMSKmStockPacket*)packet)->railNum				= hdr->pokm.railNum;
			((RDMSKmStockPacket*)packet)->stageName.nameLength	= wcslen(hdr->pokm.peregon);
			memcpy(((RDMSKmStockPacket*)packet)->stageName.name, hdr->pokm.peregon, 256);
			((RDMSKmStockPacket*)packet)->checkSum				= ((RDMSKmStockPacket*)packet)->calcCheckSum();

			header.controlType	= RDMS_CONTROL_TYPE_KM_STOCK;
			header.direction	= hdr->pokm.distanceÑount;
			break;

		case 2:
			packet = new RDMSRspPacket;

			((RDMSRspPacket*)packet)->startByte				= RDMS_RSP_PACKET;
			((RDMSRspPacket*)packet)->endByte				= RDMS_RSP_PACKET;
			((RDMSRspPacket*)packet)->rackNum				= hdr->railRSP.stelaj;
			((RDMSRspPacket*)packet)->railNum				= hdr->railRSP.railNum;
			((RDMSRspPacket*)packet)->nameData.nameLength	= wcslen(hdr->railRSP.rsp);
			memcpy(((RDMSRspPacket*)packet)->nameData.name, hdr->railRSP.rsp, 256);
			((RDMSRspPacket*)packet)->checkSum				= ((RDMSRspPacket*)packet)->calcCheckSum();

			header.controlType	= RDMS_CONTROL_TYPE_RSP;
			header.direction	= hdr->railRSP.distanceÑount;
			break;

		case 3:
			packet = new RDMSTurnoutPacket;

			((RDMSTurnoutPacket*)packet)->startByte				= RDMS_TURNOUT_PACKET;
			((RDMSTurnoutPacket*)packet)->endByte				= RDMS_TURNOUT_PACKET;
			((RDMSTurnoutPacket*)packet)->position.km			= hdr->strel.km;
			((RDMSTurnoutPacket*)packet)->position.m			= hdr->strel.m;
			((RDMSTurnoutPacket*)packet)->position.trackNumber	= hdr->strel.road;
			((RDMSTurnoutPacket*)packet)->type					= hdr->strel.type;
			((RDMSTurnoutPacket*)packet)->stageName.nameLength	= wcslen(hdr->strel.peregon);
			memcpy(((RDMSTurnoutPacket*)packet)->stageName.name, hdr->strel.peregon, 256);
			((RDMSTurnoutPacket*)packet)->checkSum				= ((RDMSTurnoutPacket*)packet)->calcCheckSum();

			header.controlType = RDMS_CONTROL_TYPE_TURNOUT;
			header.direction = 0;
			break;

		default:
			break;

	}

	// Get local time
    SYSTEMTIME st;
    GetLocalTime(&st);
	// Convert local time to file time
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
	// Calculating number of nanoseconds since 01.01.1970
    ULONGLONG fileTime = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;

	// Save number of seconds since 1970 aka UNIX timestamp
	header.timeData = fileTime / 10000000 - 11644473600LL;
	header.checkSum = header.calcCheckSum();

	// Create file name
	/*
	swprintf(fileName,
			 L"YAFFS_PART1\\%04d-%02d-%02d %02d-%02d-%02d, dist %02d, %lc%03d km %03d m (%ls).rdms",
			 st.wYear,
			 st.wMonth,
			 st.wDay,
			 st.wHour,
			 st.wMinute,
			 st.wSecond,
			 00,
			 L'~',
			 0,
			 0,
			 (hdr->sync == 0) ? L"DP" : L"internal");
	*/

	DWORD bufferSize = 5000;
	DWORD wordNum = 1000; //words
	DWORD IrqPeriod = 50; //ms

	//ExtBusCs1Init();
	FPGA_Write(SYSTEM_RESET_CR ,1);
	FPGA_Write(FSYNC_DR ,IrqPeriod*100);// t = val*100 [ms]
	FPGA_Write(TEST_IRQ_CR ,wordNum);// num of words written in time from FSYNC_DR

	//Select scan sync source
	FPGA_Write(SYNC_CR, INT_SYNC); //Internal synk
	//FPGA_Write(SYNC_CR, STOP); //Track sensor synk
	FPGA_Write(ASCAN_EN_MR, 1);


	UniDriver uniDrv;

	WORD* buffer = new WORD[bufferSize];

	uniDrv.InitIRQ(65, bufferSize, wordNum, 5000);

	RWRegData_t	readAddr;
	readAddr.baseAddr = GPMC_CS1_BASE;
	readAddr.offset = (ADC_DATA_DR)<<1; //old was 114
	readAddr.value = 0;

	uniDrv.ReadBufIRQ(&readAddr);

	hdr->dataFunc((int)(1000 / IrqPeriod), (int)(wordNum >> 8), (int)(bufferSize >> 8));

	while (*execFlag == TRUE) {
		Sleep(0);
	};
/*

	HANDLE hFile = CreateFile(L"YAFFS_PART1\\file.txt",
							  GENERIC_WRITE,
							  NULL,
							  NULL,
							  OPEN_ALWAYS,
							  NULL,
							  NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
	
		printf("Unable to create file!");
		Sleep(5000);
		return -1;
	
	}

	double sizeCounter = 0;

	DWORD resultSize = 0;
	WORD prv = 0;
	while (*execFlag == TRUE) {

		DWORD readed = uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, bufferSize * 2);
		//bufferTest(buffer, bufferSize, FALSE, prv);
		WriteFile(hFile, buffer, readed, &resultSize, NULL);

		if (resultSize != readed) {
		
			printf("Unable to write data!");
			Sleep(5000);
			return -2;
		
		}

		sizeCounter += (readed >> 10);
		hdr->updFunc((int)sizeCounter);

	}
	*/

	uniDrv.ReleaseIRQ();
	//CloseHandle(hFile);

	// Create protocol writer
	//RDMSProtocolWriter* prot = new RDMSProtocolWriter(fileName, RDMS_DEFAULT_BUFFER_SIZE);

	// Write packet to protocol
	//prot->writePacket(*packet, packet->size());
	// Delete packet
	//delete packet;

	/*
	packet = new RDMSDefectPacket;

	//
	HANDLE hGPIO = GPIOOpen();  
	if (!hGPIO) {

		printf("GPIOOpen() error\n");
		return -1;
	};

	GPIOSetMode(hGPIO, 21, GPIO_DIR_INPUT);
	GPIOPullup(hGPIO, 21, 0);
	GPIOClrBit(hGPIO, 21);
	GPIOSetMode(hGPIO, 21, GPIO_DIR_OUTPUT);

	GPIOSetMode(hGPIO, 65, GPIO_DIR_INPUT | GPIO_INT_HIGH_LOW);
	GPIOPullup(hGPIO, 65, 1);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	GPIOInterruptRelease(hGPIO, 65);
	if (!GPIOInterruptInitialize(hGPIO, 65, hEvent)) {

		return -2;

	}
	GPIOInterruptDone(hGPIO, 65);

	DEBUGMSG(TRUE, (TEXT("F_DLL_SPEED_TEST: Write started...\r\n")));

	DWORD tick = GetTickCount();

	for (int i = 0; i < 16384000; ++i) {
	//while (*execFlag == TRUE) {

		if (WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0) {

		((RDMSDefectPacket*)packet)->startByte				= RDMS_DEFECT_PACKET;
		((RDMSDefectPacket*)packet)->pathSensorData.km		= 0xF1;
		((RDMSDefectPacket*)packet)->pathSensorData.m		= 0xF2F3;
		((RDMSDefectPacket*)packet)->pathSensorData.sm		= 0xF4;
		((RDMSDefectPacket*)packet)->pathSensorData.mm		= 0xF5;
		((RDMSDefectPacket*)packet)->defects[0].amplitude	= 0x11;
		((RDMSDefectPacket*)packet)->defects[0].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[1].amplitude	= 0x22;
		((RDMSDefectPacket*)packet)->defects[1].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[2].amplitude	= 0x33;
		((RDMSDefectPacket*)packet)->defects[2].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[3].amplitude	= 0x44;
		((RDMSDefectPacket*)packet)->defects[3].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[4].amplitude	= 0x55;
		((RDMSDefectPacket*)packet)->defects[4].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[5].amplitude	= 0x66;
		((RDMSDefectPacket*)packet)->defects[5].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[6].amplitude	= 0x77;
		((RDMSDefectPacket*)packet)->defects[6].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->defects[7].amplitude	= 0x88;
		((RDMSDefectPacket*)packet)->defects[7].depth		= 0x0000;
		((RDMSDefectPacket*)packet)->endByte				= RDMS_DEFECT_PACKET;
		((RDMSDefectPacket*)packet)->checkSum				= ((RDMSDefectPacket*)packet)->calcCheckSum();
		
		// Write packet to protocol
		prot->writePacket(*packet, packet->size());

			GPIOInterruptDone(hGPIO, 65);

			Sleep(0);

		}

	}

	tick = GetTickCount() - tick;
	WCHAR buffer[256];
	swprintf(buffer, L"F_DLL_SPEED_TEST: Elapsed time: %f\r\n", ((float)tick / 1000.0f));
	DEBUGMSG(TRUE, (buffer));
	swprintf(buffer, L"F_DLL_SPEED_TEST: Approx. speed: %f\r\n", ((float)(524288000.0f / 1024.0f) / ((float)tick / 1000.0f)));
	DEBUGMSG(TRUE, (buffer));

	DEBUGMSG(TRUE, (TEXT("F_DLL_SPEED_TEST: Write done\r\n")));
	//

	// Delete packet
	delete packet;
	*/

	// Destroy protocol
	//delete prot;

	/*
	SetEvent(hEvent);
	GPIOInterruptDisable(hGPIO, 65);
	GPIOInterruptRelease(hGPIO, 65);
	GPIOClose(hGPIO);
	CloseHandle(hEvent);
	*/

	// We are done
	return 0;

}
