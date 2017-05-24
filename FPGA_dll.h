#ifndef FPGA_DLL
#define FPGA_DLL

#ifdef FPGA_DLL_EXPORTS
#define FPGA_DLL_API __declspec(dllexport) 
#else
#define FPGA_DLL_API __declspec(dllimport) 
#endif


#include "fpga_func_defs.h"
#include "FPGA_dll_func.h"

#pragma once

extern "C"
{

FPGA_DLL_API void FPGAinit(int n); //obsolete

FPGA_DLL_API void startAScan(int n); //obsolete
FPGA_DLL_API void startBScan(int n); //obsolete

FPGA_DLL_API void SettingsHndl (int n); //obsolete

FPGA_DLL_API void onAScan(int n); //obsolete
FPGA_DLL_API void offAScan(int n); //obsolete

FPGA_DLL_API void onBScan(int n); //obsolete
FPGA_DLL_API void offBScan(int n); //obsolete

FPGA_DLL_API void ToFpgaDllSend(int with_fpga, int funk, int val);

FPGA_DLL_API void fpgaIO(int IO, int datatype, buffer& buff);

}


#endif FPGA_DLL