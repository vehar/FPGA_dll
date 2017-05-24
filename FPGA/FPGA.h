#ifndef FPGA_H
#define FPGA_H

#pragma once
#include "../DM3730_Types.h"

/*
typedef char           u8;
typedef unsigned short u16;
typedef unsigned int   u32;
*/

#define FPGA_DCLKPIO     130
#define FPGA_DATA0PIO    135
#define FPGA_NSTATUSPIO  134
#define FPGA_CDONEPIO    133
#define FPGA_NCONFIGPIO  131

void FPGA_BUS_Init(); //Inits clock and ext_cs0

void FPGA_Write(DWORD Addr,unsigned short Value);
void FPGA_Write (DWORD Addr, unsigned short Value, DWORD Count) ;
u16 FPGA_Read(DWORD Addr);
void FPGA_Write_Buf(DWORD Addr, unsigned short *Buf, unsigned short Nr);
bool FPGA_Write_Prg_Buf(unsigned short *Buf, unsigned short Nr);
void FPGA_Write_Buf (DWORD fAddr,  DWORD sAddr, unsigned int *Buf, unsigned short Nr);
void FPGA_Read_Buf(DWORD Addr, unsigned short *Buf, unsigned short Nr);
void FPGA_Write_Buf_signed(DWORD Addr, short *Buf, unsigned short Nr);
void FPGA_Prg_Init();
void FPGA_DCLK(u8 val);
u8 FPGA_GETNSTATUS();
u8 FPGA_GETCDONE();
u8 FPGA_GETDATA0(); //for debug
void FPGA_NCONFIG(u8 x);
void FPGA_DATA0(u8 x);

void ExtBusCs1Init();

#endif FPGA_H