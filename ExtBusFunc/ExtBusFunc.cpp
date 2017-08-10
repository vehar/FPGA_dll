//#include "stdafx.h"


#include <windows.h>
#include "..\UniDriver\UniDriver.h"

UniDriver bus; //general memory interface

void SetDataValue(DWORD val, DWORD addr)
{
	RWRegData_t regData = {GPMC_DATA_BASE, 0, 0};
	regData.value = val;
	regData.offset = addr;
	bus.WriteReg(&regData);	 
}

DWORD GetDataValue(DWORD addr)
{
	RWRegData_t regData = {GPMC_DATA_BASE, 0, 0};
	regData.offset = addr;
	return bus.ReadReg(&regData);	 
}


void SetCs1Value(DWORD val, DWORD addr)
{ 
	RWRegData_t regData = {GPMC_CS1_BASE, 0, 0}; 
	regData.value = val; 
	regData.offset = addr;
	bus.WriteReg(&regData);	
}

DWORD GetCs1Value(DWORD addr)
{
	RWRegData_t regData = {GPMC_CS1_BASE, 0, 0};
	regData.offset = addr;
	return bus.ReadReg(&regData);	//may be return regData.value; !!!!
}


DWORD GetRegVal(int i) 
{
    RWRegData_t regData = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[i].GPMC_CONFIG2), 0};
    bus.ReadReg(&regData);
	return regData.value;
}

void ExtBusCs1Init(void) 
{ 
	//----!!!!----Look from page 2202 in "AM/DM37x Multimedia Device Silicon Revision 1.x Version R -  Technical Reference Manual" for bit/reg description

	//Example of GPMC settings from WinCE src
	/*
		C:\WINCE600\PLATFORM\LOGIC_ARM_A8\SRC\INC\bsp_cfg.h (1 hit)
		// LPD 2.0 Bring up - TODO We have moved
		// The following are copied from NAND
		#define BSP_GPMC_CF_CONFIG1    0x1210    //  device size 16 bit,  device type NOR 00631200
		#define BSP_GPMC_CF_CONFIG2    0x00131000
		#define BSP_GPMC_CF_CONFIG3    0x001F1F01
		#define BSP_GPMC_CF_CONFIG4    0x10030E03
		#define BSP_GPMC_CF_CONFIG5    0x010F1411
		#define BSP_GPMC_CF_CONFIG6    0x80030600
		#define BSP_GPMC_CF_CONFIG7    ((0x1 << 6)|(0xF << 8)|(0x18 & 0x3F)) / CS enable, 0x18000000 /

		Used at
		C:\WINCE600\PLATFORM\LOGIC_ARM_A8\SRC\OAL\OALLIB\obj\ARMV4I\retail\platform.cod (1 hit)
		Line 3850: ; 1688 :     OUTREG32(&pGpmc->GPMC_CONFIG1_3, BSP_GPMC_CF_CONFIG1);
	*/

	DBG_SHOW_FUNC;
	
    RWRegData_t CS_params = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG1), 0};
	bus.ReadReg(&CS_params); //default 4096
	CS_params.value = 0;
	CS_params.value |= 1<<12;//16 bit
	CS_params.value |= 1<<4;//latency x2, если убрать будет в 2-ва раза быстрее
	//CS_params.value |= 1<<11;// NAND Flash like devices, stream mode
    bus.WriteReg(&CS_params); 

	RWRegData_t Read_tmp1 = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG1), 0};
	bus.ReadReg(&Read_tmp1);
	DEBUGMSG(TRUE,( TEXT("	GPMC_CONFIGDATA[1].GPMC_CONFIG1 val =  0x%x \r\n"),Read_tmp1.value ));

	////////////////////////////////////////////
	RWRegData_t CS_timing = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG2), 0};
	bus.ReadReg(&CS_timing); //default 526337
	//CS_timing.value = 0;//
	CS_timing.value |= 0x1<<0;// CS i assertion time from start cycle time 
	CS_timing.value |= 0x1F<<16;// CS i de-assertion time from start cycle time for Write accesses 
	CS_timing.value |= 0x1F<<8;// CS i de-assertion time from start cycle time for Read accesses 
    bus.WriteReg(&CS_timing); 


	RWRegData_t Read_tmp2 = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG2), 0};
	bus.ReadReg(&Read_tmp2);
	DEBUGMSG(TRUE,( TEXT("	GPMC_CONFIGDATA[1].GPMC_CONFIG2 val =  0x%x \r\n"),Read_tmp2.value ));

	////////////////////////////////////////////
	RWRegData_t WE_OE_timing = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG4), 0};
	bus.ReadReg(&WE_OE_timing);//default 134285313
	//WE_OE_timing.value = 0;
	//WE_OE_timing.value |= 0x5<<24;//nWE de-assertion time from start cycle time
	WE_OE_timing.value |= 0x5<<16;//nWE assertion time from start cycle time
	WE_OE_timing.value |= 0x5<<0;//nOE assertion time from start cycle time
    bus.WriteReg(&WE_OE_timing); 


	RWRegData_t Read_tmp4 = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG4), 0};
	bus.ReadReg(&Read_tmp4);
	DEBUGMSG(TRUE,( TEXT("	GPMC_CONFIGDATA[1].GPMC_CONFIG4 val =  0x%x \r\n"),Read_tmp4.value ));

	//////////////////////////////////////////// RdAccessTime and CycleTime timing parameters configuration
	/*RWRegData_t AccTime_timing = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_CONFIGDATA[1].GPMC_CONFIG5), 0};
	bus.ReadReg(&AccTime_timing);//default 526858
	//AccTime_timing.value = 0;
	//AccTime_timing.value |= 0x1<<19;
	AccTime_timing.value |= 0x3<<8;
	AccTime_timing.value |= 0x3<<0;
    bus.WriteReg(&AccTime_timing); */

	////////////////////////////////////////////
	//RWRegData_t Timeout = {GPMC_BASE_ADDR, offsetof(GPMC_REGS_t, GPMC_TIMEOUT_CONTROL), 0};
	//bus.ReadReg(&Timeout);//default 
	//WE_OE_timing.value = 0;

    //bus.WriteReg(&Timeout); 
}