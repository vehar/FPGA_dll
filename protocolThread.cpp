#include <windows.h>
#include "winioctl.h"
#include <ctime>
#include <queue>

#include "GpioDriver\gpio.h"
#include "UniDriver\UniDriver.h"
#include "FPGA/FPGA_Registers.h"

#include "protocolThread.hpp"
#include "Protocol\RDMSIProtocol.hpp"
#include "Protocol\RDMSProtocolWriter.hpp"


#include "FPGA\FPGA.h"

#pragma pack(push, 1)
struct RGBA {

	float x;
	float y;
	float R;
	float G;
	float B;
	float A;
};
#pragma pack(pop)

/*
#define BUFFERS        4
#define CHANNELS		8
#define POINTS        200
struct PACKET {

    int num;
    unsigned char *packet[BUFFERS][CHANNELS]; 

	PACKET() {

		for(int i = 0; i < BUFFERS; i++)
			for(int j = 0; j < CHANNELS; j++)
				packet[i][j] = new unsigned char[POINTS];

	}

};
std::queue<PACKET>* packets = NULL;
*/

//protocolData* pData = NULL;

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
DWORD startProtocolThread(protocolData* pData) {

	DBG_SHOW_FUNC_T("F_PROTO: ");
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

	DBG_SHOW_FUNC_T("F_PROTO: "); 

	// Check if thread exists
	if (protThread != NULL) {

		// Stop thread execution
		execStruct->threadExecuting = FALSE;

		// Wait for thread to stop
		WaitForSingleObject(protThread, INFINITE);

		// Zero thread handler
		protThread = NULL;

		// Delete execution info struct
		delete execStruct;

	}

}

	DWORD bufferSize	= 10000;//8 << 8;		// buff		8192
	DWORD wordNum		= 10;//8 << 4;		// words	128

// Protocol thread
DWORD WINAPI protocolThreadFunc(LPVOID lpParam) {

	DBG_SHOW_FUNC_T("F_PROTO: ");

	//ExtBusCs1Init();


	//DWORD cycleFreq		= 100;			// ms

	//FPGA_Write(FSYNC_DR, cycleFreq * 100);		// t = val * 100 [ms]
	//FPGA_Write(TEST_IRQ_CR, wordNum);			// num of words written in time from FSYNC_DR

	//FPGA.setAScanBuffSize(wordNum); TODO
	FPGA_Write(0x50, wordNum);

	// Get pointer to thread param
	execInfo* exec = (execInfo*)lpParam;
	// Poiner to execution flag
	BOOL* execFlag = &exec->threadExecuting;
	// Pointer to header data
	protocolData* hdr = exec->pHeader;

	
 /***/	UniDriver uniDrv;

//

	WORD* buffer = new WORD[bufferSize];

	DWORD cnt = 0;
	DWORD cnt2 = 0;

	
 /***/	uniDrv.InitIRQ(65, bufferSize * 2, wordNum * 2, 1000);

	/*
	RWRegData_t	readAddr;
	readAddr.baseAddr	= GPMC_CS1_BASE;
	readAddr.offset		= 114 << 1;
	readAddr.value		= 0;
	*/

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
 /***/		uniDrv.ReleaseIRQ();
		return -1;
	
	}

	/*
	HINSTANCE lib = LoadLibrary(L"SPI_dll.dll");
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
	*/

	for (DWORD i = 0; i < wordNum; ++i) {
	
		buffer[i] = 0x0000;
	
	}

	//SPIInit();

	/*
	RDMSDefectPacket		defectPacket;
	RDMSVoltageTempPacket	vtPacket;

	RDMSGainPacket			gainPacket;
	gainPacket.startByte	= RDMS_GAIN_PACKET;
	gainPacket.channelNum	= 0x00;
	gainPacket.gain			= 0x00;
	gainPacket.endByte		= RDMS_GAIN_PACKET;

	DWORD readed = 85;
	CHAR data[85];
	*/

	RWRegData_t	readAddr;
	readAddr.baseAddr	= GPMC_CS1_BASE;
	readAddr.offset		= 0x50 << 1;
	readAddr.value		= 0;

	DWORD resultSize = 0;
	WORD prv = 0;
	while (*execFlag == TRUE) {

		/*
		if (hdr->sensitivity.check != 0x00) {

			int* channel = &hdr->sensitivity.channel0;

			for (int i = 0; i < 8; ++i) {

				if ((hdr->sensitivity.check & (1 << i)) != 0x00) {

					gainPacket.channelNum	= i;
					gainPacket.gain			= channel[i];
					gainPacket.calcCheckSum();
					gainPacket.write(data);
					WriteFile(hFile, data, 5, &resultSize, NULL);

				}

			}

			hdr->sensitivity.check = 0x00;

		}
		*/

		/*
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

			printf("Unable to write data!");
			Sleep(5000);
			uniDrv.ReleaseIRQ();
			return -2;

		}

		/*
		hdr->memLimit -= resultSize;

		if (resultSize > hdr->memLimit) {
		
			*execFlag = FALSE;
		
		}
		*/

		/*
		char data[1024];
		uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, wordNum * 2);
		sprintf(data, "IRQ count:\t%d", 0);
		WriteFile(hFile, data, strlen(data), &resultSize, NULL);
		*/

		// Packet defects data
		//uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, wordNum * 2);

//uniDrv.ReadBuf(0x50, buffer, wordNum * 2);

 /***/		uniDrv.ReadBufIRQ(&readAddr, (PBYTE)buffer, wordNum * 2);
	//	WriteFile(hFile, buffer, wordNum * 2, &resultSize, NULL);

		/*
		uniDrv.ReadBuf(63, buffer, 778);
		char* charBuff = (char*)buffer;

		int max = charBuff[347];

		for (int i = 347; i < 484; ++i) {
		
			if (charBuff[i] > max) {
			
				max = charBuff[i];
			
			}
		
		}

		*((float*)hdr) = (max * 1000 - 36300) * 0.001f;
		*/

		/*
		PACKET tempPacket;
		tempPacket.num = 4;
		for (int i = 0; i < tempPacket.num; ++i) {
		
			for (DWORD j = 0; j < 8; ++j) {

					
				tempPacket.packet[i][j][0] = ((char*)buffer)[cnt + 6];
				tempPacket.packet[i][j][1] = (char)255;

				cnt += 16;
				if (cnt > wordNum * 2) {
				
					cnt = 0;
				
				}

			}

		
		}

		((std::queue<PACKET>*)hdr)->push(tempPacket);
		*/

		/*
		if (hdr != NULL) {

			for (int i = 0; i < 32; ++i) {

				float data = (float)((char*)buffer)[cnt + 6];

				for (int j = 0; j < 272; ++j) {

					((RGBA*)hdr)[cnt2 + 479 * j].x = cnt2;
					((RGBA*)hdr)[cnt2 + 479 * j].y = j;

					if (data == j) {

						((RGBA*)hdr)[cnt2 + 479 * j].R = 0;
						((RGBA*)hdr)[cnt2 + 479 * j].G = 1;
						((RGBA*)hdr)[cnt2 + 479 * j].B = 0;
						((RGBA*)hdr)[cnt2 + 479 * j].A = 1;

					} else {

						((RGBA*)hdr)[cnt2 + 479 * j].R = 0;
						((RGBA*)hdr)[cnt2 + 479 * j].G = 0;
						((RGBA*)hdr)[cnt2 + 479 * j].B = 0;
						((RGBA*)hdr)[cnt2 + 479 * j].A = 1;

					}

				}

				cnt += 16;
				if (cnt > wordNum * 2) {

					cnt = 0;

				}

				++cnt2;
				if (cnt2 > 479) {

					cnt2 = 0;

				}

			}

		}
		*/

		//WriteFile(hFile, buffer, wordNum * 2, &resultSize, NULL);

		if (*execFlag != TRUE) {

			break;

		}

		Sleep(500);

	}

	//FreeLibrary(lib);

 /***/	uniDrv.ReleaseIRQ();
	CloseHandle(hFile);

	// We are done
	return 0;

}
