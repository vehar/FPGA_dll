// FPGA_app.cpp : Defines the entry point for the console application.
//


//#undef WINCE
//#define DEBUG 
#define FPGA_BUS_TESTS

#include <windows.h>
#include <commctrl.h>
#include <winbase.h>
#include <WinIoCtl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "protocolThread.hpp"
#include "Freeze/Freeze.h" 

#include "debug.h" 

#ifdef WINCE

#include "FPGA_dll.h"
#include "FPGA_dll_func.h"

#include "FPGA_func_defs.h"
//#include "FPGA_registers.h"

#include "MathHelpers.h"
#include "BScanHelper\BScanHelper.h"

#endif

int DebugOutActive = 0; //if(DebugOutActive) printf
int with_fpga = 1;

FPGACommunication FPGA;

#define DBG_TARGET "F_APP"


extern void ToFpgaDllSend(int with_fpga, int funk, int val);
extern void fpgaIO(int IO, int datatype, buffer& buff);

DWORD iterations  = 1;
DWORD wordNum = 4000; //words
DWORD IrqPeriod = 1; //ms
DWORD bufSize = wordNum*4;

int _tmain(int argc, _TCHAR* argv[])
{
DEBUGMSG(TRUE,( TEXT("\r\n\r\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n")));
DBG_SHOW_DATE_T(DBG_TARGET);
DBG_SHOW_FUNC_T(DBG_TARGET);


FpgaCycleRegTest(CONTROL_REG);

//FpgaSpeedTest(iterations, wordNum, IrqPeriod, bufSize);
//ToFpgaDllSend(with_fpga, F_FPGA_INIT, 0);

//test();

Sleep(1000);
	return 0;
}

