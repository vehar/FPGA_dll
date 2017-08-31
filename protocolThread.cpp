#include <windows.h>
#include "winioctl.h"
#include <ctime>

#include "GpioDriver\gpio.h"
#include "UniDriver\UniDriver.h"
#include "FPGA/FPGA_Registers.h"

#include "protocolThread.hpp"
#include "Protocol\RDMSIProtocol.hpp"
#include "Protocol\RDMSProtocolWriter.hpp"


#include "FPGA\FPGA.h"

protocolData* pData = NULL;

//
struct OnBoardParams     
{    
	float StmTemperature;
	float AxelTemp;
	float Akkum_C;
	float Akkum_V;
	float Board_1_8V;
	float Board_3_3V;
	float Board_5V;
	float Board_140V; 
}; 

typedef struct struct_GpsTime
{
	char sec;
	char min;
	char hour;
}GpsTimeType;

typedef struct struct_GpsDate
{
	char year;
	char month;
	char day;
}GpsDateType;

typedef struct struct_GpsInfo
{	
	char state;
	float lat;
	float lon;
	int course;
	int speed;
	GpsTimeType time;
	GpsDateType date;
} GpsInfoType;

union buffIO
{
	OnBoardParams volt;
	GpsInfoType GpsInfo;
};

typedef void (__stdcall *SPIINITFUNC)();
typedef void (__stdcall *SPIIOFUNC)(int, int, buffIO&);
//


/*
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
DWORD startProtocolThread() {

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

	ExtBusCs1Init();

	DWORD bufferSize	= 8 << 10;		// buff		8192
	DWORD wordNum		= 8 << 4;		// words	128
	DWORD cycleFreq		= 1;			// ms

	FPGA_Write(SYSTEM_RESET_CR, 1);
	FPGA_Write(FSYNC_DR, cycleFreq * 100);		// t = val * 100 [ms]
	FPGA_Write(TEST_IRQ_CR, wordNum);			// num of words written in time from FSYNC_DR

	// Get pointer to thread param
	execInfo* exec = (execInfo*)lpParam;
	// Poiner to execution flag
	BOOL* execFlag = &exec->threadExecuting;
	// Pointer to header data
  
	protocolData* hdr = &exec->pHeader;

	/*
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
	DWORD cycleFreq = 50; //ms

	//ExtBusCs1Init();
	FPGA_Write(2, 1);
	FPGA_Write(6, cycleFreq * 100);// t = val*100 [ms]
	FPGA_Write(115, wordNum);// num of words written in time from FSYNC_DR

	UniDriver uniDrv;

	WORD* buffer = new WORD[bufferSize];

	uniDrv.InitIRQ(65, bufferSize, wordNum, 5000);

	RWRegData_t	readAddr;
	readAddr.baseAddr = GPMC_CS1_BASE;
	readAddr.offset = 114<<1;
	readAddr.value = 0;

	uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, bufferSize * 2);


	uniDrv.InitIRQ(65, bufferSize * 2, wordNum * 2, 50);

	while (*execFlag == TRUE) {
		Sleep(0);
	};

	HANDLE hFile = CreateFile(L"YAFFS_PART1\\Protocols\\file.txt", 
							  GENERIC_WRITE,
							  NULL,
							  NULL,
							  OPEN_ALWAYS,
							  NULL,
							  NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
	
		printf("Unable to create file!");
		Sleep(5000);
		uniDrv.ReleaseIRQ();
		return -1;
	
	}

	HINSTANCE lib = LoadLibrary(L"dllSpiServer.dll");
	if(!lib) {

		printf("DLL not found!!!");
		Sleep(5000);
		uniDrv.ReleaseIRQ();
		return -3;

	}

	SPIIOFUNC SPIIO = (SPIIOFUNC)GetProcAddressA(lib, "SpiIo");
	if (!SPIIO) {

		printf("Func not found!!!");
		Sleep(5000);
		uniDrv.ReleaseIRQ();
		return -4;

	}

	SPIINITFUNC SPIInit = (SPIINITFUNC)GetProcAddressA(lib, "SPIinit");
	if (!SPIInit) {

		printf("Func not found!!!");
		Sleep(5000);
		uniDrv.ReleaseIRQ();
		return -5;

	}

	for (DWORD i = 0; i < wordNum; ++i) {
	
		buffer[i] = 0x0000;
	
	}

	SPIInit();

	RDMSDefectPacket		defectPacket;
	RDMSVoltageTempPacket	vtPacket;

	RDMSGainPacket			gainPacket;
	gainPacket.startByte	= RDMS_GAIN_PACKET;
	gainPacket.channelNum	= 0x00;
	gainPacket.gain			= 0x00;
	gainPacket.endByte		= RDMS_GAIN_PACKET;

	DWORD readed = 85;
	//CHAR data[85];

	RDMSDefectPacket packet;

	CHAR data[47];

	DWORD resultSize = 0;
	WORD prv = 0;
	while (*execFlag == TRUE) {


		/*
		if (hdr->sensitivity.check != 0x00) {

			int* channel = &hdr->sensitivity.channel0;

		DWORD readed = uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, bufferSize * 2);

		// Packet start byte
		packet.startByte = RDMS_DEFECT_PACKET;

		// Packet path sensor data
		packet.pathSensorData.km		= buffer[0] & 0xFF00 >> 8;
		packet.pathSensorData.m			= buffer[1];
		packet.pathSensorData.sm		= buffer[2] & 0xFF00 >> 8;
		packet.pathSensorData.mm		= buffer[2] & 0x00FF;

		// GPS coordinates data
		packet.gpsData.state			= 0xFF;
		packet.gpsData.lat				= 43.1435f;
		packet.gpsData.lon				= 45.2353f;
		packet.gpsData.speed			= 0x02;
		packet.gpsData.course			= 0x00;
		packet.gpsData.sec				= 33;
		packet.gpsData.min				= 8;
		packet.gpsData.hour				= 15;
		packet.gpsData.year				= 2017;
		packet.gpsData.month			= 6;
		packet.gpsData.day				= 14;
		/*
		packet.gpsData.latLongDirection	= 0x00;
		packet.gpsData.latDegrees		= 0x00;
		packet.gpsData.latMinutes		= 0x00;
		packet.gpsData.latSeconds		= 0x00;
		packet.gpsData.longDegrees		= 0x00;
		packet.gpsData.longMinutes		= 0x00;
		packet.gpsData.longSeconds		= 0x00;
		*/

		// Packet defects data
		packet.defects[0].amplitude		= buffer[3] & 0xFF00 >> 8;
		packet.defects[0].depth			= buffer[4];
		packet.defects[1].amplitude		= buffer[5] & 0xFF00 >> 8;
		packet.defects[1].depth			= buffer[6];
		packet.defects[2].amplitude		= buffer[7] & 0xFF00 >> 8;
		packet.defects[2].depth			= buffer[8];
		packet.defects[3].amplitude		= buffer[9] & 0xFF00 >> 8;
		packet.defects[3].depth			= buffer[10];
		packet.defects[4].amplitude		= buffer[11] & 0xFF00 >> 8;
		packet.defects[4].depth			= buffer[12];
		packet.defects[5].amplitude		= buffer[13] & 0xFF00 >> 8;
		packet.defects[5].depth			= buffer[14];
		packet.defects[6].amplitude		= buffer[15] & 0xFF00 >> 8;
		packet.defects[6].depth			= buffer[16];
		packet.defects[7].amplitude		= buffer[17] & 0xFF00 >> 8;
		packet.defects[7].depth			= buffer[18];

		// Packet end byte
		packet.endByte = RDMS_DEFECT_PACKET;

		// Packet checksum
		packet.calcCheckSum();

		// Write packet to buffer
		packet.write(data);

		WriteFile(hFile, data, 47, &resultSize, NULL);

		if (resultSize != readed) {

			printf("Unable to write data!");
			Sleep(5000);
			return -2;

		}


			for (int i = 0; i < 8; ++i) {


	}

	uniDrv.ReleaseIRQ();
	CloseHandle(hFile);

				}

			}

			hdr->sensitivity.check = 0x00;

		}
		//

		buffIO bf;
		ZeroMemory(&bf, sizeof(buffIO));

		// Try to get some GPS!
		SPIIO(1, 2, bf);

		vtPacket.startByte			= RDMS_VOLTAGE_TEMP_PACKET;
		vtPacket.stmTemperature		= bf.volt.StmTemperature;
		vtPacket.axelTemperature	= bf.volt.AxelTemp;
		vtPacket.accumCurrent		= bf.volt.Akkum_C;
		vtPacket.accumVoltage		= bf.volt.Akkum_V;
		vtPacket.board_1_8V			= bf.volt.Board_1_8V;
		vtPacket.board_3_3V			= bf.volt.Board_3_3V;
		vtPacket.board_5V			= bf.volt.Board_5V;
		vtPacket.board_140V			= bf.volt.Board_140V;
		vtPacket.calcCheckSum();
		vtPacket.endByte			= RDMS_VOLTAGE_TEMP_PACKET;
		// Write packet to buffer
		vtPacket.write(data);

		ZeroMemory(&bf, sizeof(buffIO));
		SPIIO(1, 1, bf);

		// Packet start byte
		defectPacket.startByte				= RDMS_DEFECT_PACKET;
		// Packet path sensor data
		defectPacket.pathSensorData.km		= buffer[0] & 0xFF00 >> 8;
		defectPacket.pathSensorData.m		= buffer[1];
		defectPacket.pathSensorData.sm		= buffer[2] & 0xFF00 >> 8;
		defectPacket.pathSensorData.mm		= buffer[2] & 0x00FF;
		// GPS coordinates data
		defectPacket.gpsData.state			= bf.GpsInfo.state;
		defectPacket.gpsData.lat			= bf.GpsInfo.lat;
		defectPacket.gpsData.lon			= bf.GpsInfo.lon;
		defectPacket.gpsData.speed			= bf.GpsInfo.speed & 0x000000FF;
		defectPacket.gpsData.course			= bf.GpsInfo.course & 0x0000FFFF;
		defectPacket.gpsData.sec			= bf.GpsInfo.time.sec;
		defectPacket.gpsData.min			= bf.GpsInfo.time.min;
		defectPacket.gpsData.hour			= bf.GpsInfo.time.hour;
		defectPacket.gpsData.year			= bf.GpsInfo.date.year;
		defectPacket.gpsData.month			= bf.GpsInfo.date.month;
		defectPacket.gpsData.day			= bf.GpsInfo.date.day;

		// Packet defects data
		uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, wordNum * 2);

		for (int i = 0; i < 8; ++i) {

			defectPacket.defects[i].depth		= buffer[10 + 10 * i] & 0xFFFF;
			defectPacket.defects[i].amplitude	= 0x00;

		}

		// Packet end byte
		defectPacket.endByte				= RDMS_DEFECT_PACKET;
		// Packet checksum
		defectPacket.calcCheckSum();
		// Write packet to buffer
		defectPacket.write(data + 35);

		WriteFile(hFile, data, 85, &resultSize, NULL);

		if (resultSize != readed) {

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

		}

		hdr->memLimit -= resultSize;

		if (resultSize > hdr->memLimit) {
		
			*execFlag = FALSE;
		
		}
		*/

		char data[1024];
		uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, wordNum * 2);
		sprintf(data, "IRQ count:\t%d", 0);
		WriteFile(hFile, data, strlen(data), &resultSize, NULL);

		Sleep(0);

	}

	FreeLibrary(lib);

	uniDrv.ReleaseIRQ();
	CloseHandle(hFile);

	// We are done
	return 0;

}
