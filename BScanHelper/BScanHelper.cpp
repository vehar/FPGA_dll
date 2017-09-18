//#include "stdafx.h"

#include "BScanHelper.h"

void BSkan_errase (void){}

void BSkan_errase_colomn (int x_start){}

void setGenChSeq(USHORT Addr, USHORT GenSel, USHORT ChSel)
{
	USHORT tmp = 0;
	tmp = (Addr) | (ChSel<<4)| (GenSel<<8);
//	FPGA_Write(GRSQ_CR, tmp);
}
