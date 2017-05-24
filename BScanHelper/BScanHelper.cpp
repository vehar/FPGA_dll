//#include "stdafx.h"

#include "BScanHelper.h"

void BSkan_errase (void)
{
int x_errase = LCD_WIDTH;
int y_errase = 0;

FPGA.setAScanWrCS(1+64+128+256);

while(x_errase-- > 0)
{
	while (y_errase++ < LCD_HEIGHT){
		FPGA.setCursorX(1, x_errase);
		FPGA.setCursorY(1, y_errase);
		FPGA.setAScanBuff(0, 0);
	}
	y_errase = 0;
}
}

void BSkan_errase_colomn (int x_start)
{
int x_errase = x_start-10;
int y_errase = 0;

FPGA.setAScanWrCS(64+128+256);

while(x_errase++ < x_start)
{
	while (y_errase++ < LCD_HEIGHT){
		FPGA.setCursorX(1, x_errase);
		FPGA.setCursorY(1, y_errase);
		FPGA.setAScanBuff(0, 0);
	}
	y_errase = 0;
}
}

void setGenChSeq(USHORT Addr, USHORT GenSel, USHORT ChSel)
{
	USHORT tmp = 0;
	tmp = (Addr) | (ChSel<<4)| (GenSel<<8);
//	FPGA_Write(GRSQ_CR, tmp);
}
