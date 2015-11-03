/*
 * HAL.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Robo
 */
#include "MPC5606B.h"
#include "HAL.h"
#include "stdtypedef.h"
#include "IntcInterrupts.h"
#include "MAL.h"


void initModesAndClock(void) {
	//ME.MER.R = 0x0000001D;
	//CGM.FMPLL_CR.R = 0x02400100;
	//ME.RUNPC[1].R = 0x00000010; 
	ME.RUN[0].R = 0x001F0074;
	ME.RUNPC[0].R = 0x000000FE;
	ME.MCTL.R = 0x40005AF0;
	ME.MCTL.R = 0x4000A50F; 
	INTC_InstallINTCInterruptHandler(isr,59,1);
	INTC.CPR.R = 0;
}
/******************************************************************************/
void initPIT(uint32_t LDVALOR_0, uint32_t LDVALOR_1){
	PIT.PITMCR.B.MDIS = 0;			//Enable Clock for PIT
	PIT.PITMCR.B.FRZ = 1;			//Detiene el timer en modo debuger
	PIT.CH[0].LDVAL.R = LDVALOR_0;	//Carga el valor con el que se desbordara el timer
	PIT.CH[0].TCTRL.B.TEN = 1;		//Habilita el timer
	PIT.CH[0].TCTRL.B.TIE = 1;		//Habilita interrupcion del timer
	PIT.CH[0].TFLG.B.TIF = 1;
	
	PIT.CH[1].LDVAL.R = LDVALOR_1;	//Carga el valor con el que se desbordara el timer
	PIT.CH[1].TCTRL.B.TEN = 1;		//Habilita el timer
	PIT.CH[1].TCTRL.B.TIE = 0;		//Habilita interrupcion del timer
	PIT.CH[1].TFLG.B.TIF = 1;
}

/******************************************************************************/

void initGPIO(void){
	T_UWORD luw_i;
	for(luw_i = 0; luw_i <=11; luw_i++) SIU.PCR[luw_i].R = 0x0200;
	for(luw_i = 64; luw_i <=67; luw_i++) SIU.PCR[luw_i].R = 0x0100;
	for(luw_i = 0; luw_i <= 9; luw_i++) SIU.GPDO[luw_i].B.PDO = 0;
	SIU.GPDO[10].B.PDO = 1;
	SIU.GPDO[11].B.PDO = 1;	
}