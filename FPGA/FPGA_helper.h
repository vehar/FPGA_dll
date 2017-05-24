#ifndef FPGA_HELPER
#define FPGA_HELPER

#pragma once

#include <windows.h>
#include <WinIoCtl.h>
//#include <tchar.h>
//#include <stdio.h>
//#include <stdlib.h> 
//#include <memory.h> 
//#include <string.h>
//#include "locale.h"
//#include <winbase.h>
//#include <initguid.h> 
//#include <winuser.h> 
//#include <winuserm.h>
//#include <commctrl.h>


#include "FPGA.h"  
#include "../ExtBusFunc/ExtBusFunc.h"  

#include "../AcousticLib/FPGACommunication.h" 
#include "../AcousticLib/AcousticScheme.h"
//#include "AcousticLib/AcousticSchemeProcessor.h"  
//#include "CustThread/CustThread.h" 
//#include "Controls/CustControls.h"

//TODO: dock it!!!!
 //RDM11
#define FPGA_ADC_SIGNAL_MAX_VAL 512 // TODO: fix bug with max 500 in FPGA

#define LCD_MAX_ASCAN_Y_RANGE 11
#define MAX_INPUT_VAL 10000 //num of adc counts (ZONE)

#define LCD_ASCAN_Y_BIAS 200

#define LCD_WIDTH  480
#define LCD_HEIGHT 272


#define VAL_MIRROR_X(val) (-(val)+LCD_HEIGHT-10) //for A/Bscan mirroring


//FPGA_DISPLAY_DIRECT_DRAWING_COLORS
	//Bit    07  06  05 04  03 02  01 00
	//Data   R   R   R  G   G   G  B   B

#define DARK_RED	(0xE0)// EC
#define GREEN		(0x1C)
#define BLUE		(0x03)
#define RED			(DARK_RED+0x10)
#define YELLOW		(RED|GREEN)
#define AQUA		(GREEN|BLUE)
#define VIOLET		(DARK_RED|GREEN)
/*
#define RED (0xF0)
#define YELLOW (0xFC)
#define VIOLET (0x1F)
*/

extern int compress_val;
extern int Channel;
extern bool AScanEn;
extern USHORT koef_array[23];

extern Sync SyncState;
extern FPGACommunication FPGA;


extern void KoeffArr_init(void);
extern void AcousticScheme_DefaultInit(void);
extern void FPGA_Regs_deinit(void);
extern void System_init(void);
extern void Ascan_init(void);

extern void Gen_init(void);
extern void Acust_init(void);
extern void Gates_init(void);
extern void HardAScan_Start(void);
extern void HardAScan_Stop(void);

extern int FPGA_DBUS_TEST();
extern int FPGA_ABUS_TEST();


extern USHORT ToAdcScale (USHORT persent);
extern USHORT ToLcdScaleY (USHORT persent);
extern USHORT ToLcdScaleX (USHORT val);

#endif FPGA_HELPER