#include "MPC5606B.h"
#include "stdtypedef.h"
#include "Application.h"
#include "HAL.h"
#include "MAL.h"

#define Emain0 1
#if Emain0 == 1

#define ValTMR_0 		0x0000F9FF
#define ValTMR_1 		0x1312CFFF

T_UBYTE rub_CountIndice = 0;

volatile T_UBYTE rub_FlagValUpAut = 0;
volatile T_UBYTE rub_FlagValUpMan = 0;
volatile T_UBYTE rub_FValAutUP = 0;
volatile T_UBYTE rub_FlagValDownAut = 0;
volatile T_UBYTE rub_FlagValDownMan = 0;
volatile T_UBYTE rub_FValAutDown = 0;
volatile T_UBYTE rub_FlagValAnPi = 0;

volatile T_UWORD ruw_CountUp = 0;
volatile T_UWORD ruw_CountDown = 0;
volatile T_UWORD ruw_CountAnPi = 0;

volatile T_UBYTE rub_F400 = 0;
volatile T_UBYTE rub_Fled = 0;
volatile T_UWORD ruw_count400 = 0;


void main (void) {
	
	initModesAndClock();			/* Initialize mode entries and system clock */
	initGPIO();
	initPIT(ValTMR_0, ValTMR_1);
	
	while (1) {
		WiLi_Aplication();
	}
}

#endif



