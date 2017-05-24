#ifndef BSCANHELPER_H
#define BSCANHELPER_H

//BScanHelper.h

#include "..\FPGA\FPGA_helper.h"

void BSkan_errase (void);
void BSkan_errase_colomn (int x_start);
void setGenChSeq(USHORT Addr, USHORT GenSel, USHORT ChSel);


#endif BSCANHELPER_H