#ifndef FPGADLLFUNC_H
#define FPGADLLFUNC_H

#include "FPGA\FPGA_helper.h"

struct сursor
{
	int posX1;
	int posX2;
	int value;
};

struct cursData
{
	int dN;
	int Kd;
	int H;
	int L;
	int excess;  // если не 0 то рисуется красная полоса превышения
};

// IGOR
struct moveData {

	float	speed;			// Скорость  прибора
	int		tickCount;		// Количество тиков
	char	direction;		// Направление движения (+ или -)

};

union buffer
{
	сursor		buff_сurs;
	cursData	buff_сd;
	protocolData*	buff_prot_header;
	//
	moveData	buff_md;
	//
};


///////////////////Settings///////////////////////////////////
//FreqSync+			activeScheme->inqFreq;
 //SyncCtrl
 //Compress+
 //AN_CH_CSR
 //AttenSw
 //ACGAIN_DR_1!
 //GenBuff?
 //AScanEn
 //AdcDelay?
 //ProbeDelay?

//Отображать:
 //пороговая чувствительность - двигается горизонтально (глубина обнаружения дефекта)
 //дH, дL  -условная ширина дефекта и глубина залегания


int DBUS_TEST_result = 0;
int ABUS_TEST_result = 0;
int error = 0;


int FPGA_ABUS_TEST();
int FPGA_DBUS_TEST();

void System_init (void);
void PrintAcousticScheme(AcousticScheme AkSheme); //  Print some of the AcousticScheme structure values
void draw_sine (int cnt, int lim);
int duration(int del);


USHORT koef_array[23];

//AcousticSchemeProcessor asp;

DWORD dwThreadId;
HANDLE hHandle;

int DataRefreshed_Flag = 0;
int KeyState = 0;
int EncState = 0;

int compress_val;
int Channel;
bool AScanEn;
Sync SyncState = SyncCont;


//private 
USHORT main_cr = 0;

int adc_sample_cnt = 0;
int rd_index = 0;

int x1 = 0; 

int x1_ = 0;
int x2_ = 0;
int y1_ = 0;
int y2_ = 0;

//float time = 0;
//float speed = 0;

int det = 0;

DWORD elevation = 0;


//private 
void _dellay(void)
{
	for (int i = 10000; i>0; i--);
}
 
void draw_sine (int cnt = 20, int lim = 5)
{
	//int cnt = 20;
	while(cnt>7)
	{	
		for(double x1 = -40; x1 < lim; x1 += 0.1)
		{
			int y1 = sin(x1)*3*cnt+100;

			FPGA.setAScanBuff(y1+10+cnt/2, y1);
			_dellay();
		}
		cnt --;
		//if(cnt>=100)
		//{
		//	cnt = 1;
			//FPGA_Write(51, 1);//? ram cnr rst
		//}
	}
}


#endif FPGADLLFUNC_H