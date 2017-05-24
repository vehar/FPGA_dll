#ifndef ACOUSTICDEFINES_H
#define ACOUSTICDEFINES_H

#pragma once

#ifndef M_PI
#define M_PI					3.14159265f			// PI constant
#endif



static const USHORT ADC_MAX_VALUE		= 1024;					// == 1024 (10 bit ADC)
static const USHORT ADC_MID_VALUE	= (ADC_MAX_VALUE>>1);	// == 512 

#define				DB_MAX_VALUE 		    80
#define				COEFF_FREQ40MHZ			40				// FPGA global clk constant
#define				COEFF_FREQ200MHZ		2				// FPGA pulser clk constant
static const float  COEFF_FREQ80MHZ		= 80.0f;				// base clock

static const int MaxSignalBuffSize		=	1536;
#define AVERAGE_SIGNAL_MAX_VALUE		16
#define DISPLAY_SIGNAL_LEN				480

#define GATES_NUMBER  2
static const int PULSER_BUF_SIZE		=	32;
static const int TGC_BUF_SIZE			=	24;


#define FILTER_DELAY_CNT		90
#define FILTER_COEFF_COUNT		23					// total quantity of coefficients == 45 (23*2-1)

#define ACOUST_CONT_GAIN		35
#define ATTEN_LEVEL				22

//Analog channels
#define CH1				1
#define CH2				2
#define CH3				3
#define CH4				4
#define CH5				5
#define CH6				6
#define CH7				7
#define CH8				8


//separated - combine = S - C


//Generator channels
#define GEN1				1
#define GEN2				2
#define GEN3				3
#define GEN4				4
#define GEN5				5
#define GEN6				6
#define GEN7				7
//#define GEN8				8


#endif ACOUSTICDEFINES_H
