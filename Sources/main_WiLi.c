#include "MPC5606B.h"
#include "IntcInterrupts.h"

#define HMain0 1
#define HMain1 0

#if HMain0 == 1

#define ValTMR 0X01869FFF 

int dly,lly;

void isr(void){
	if( PIT.CH[2].TFLG.B.TIF ){
		
		PIT.CH[2].TFLG.B.TIF = 1;
		SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;
		
	}
}
void initModesAndClock(void) {
	//ME.MER.R = 0x0000001D;
	//CGM.FMPLL_CR.R = 0x02400100;
	//ME.RUNPC[1].R = 0x00000010; 
	ME.RUN[0].R = 0x001F0074;
	ME.RUNPC[0].R = 0x000000FE;
	ME.MCTL.R = 0x40005AF0;
	ME.MCTL.R = 0x4000A50F; 
}

/******************************************************************************/
void initPIT(uint32_t LDVALOR){
	PIT.PITMCR.B.MDIS = 0;
	PIT.PITMCR.B.FRZ = 1;
	PIT.CH[2].LDVAL.R = LDVALOR;
	PIT.CH[2].TCTRL.B.TEN = 1;
	PIT.CH[2].TCTRL.B.TIE = 1;
}
/******************************************************************************/
void main (void) {
	unsigned int *P1 = (unsigned int *)0x00800000;
	unsigned int *P2 = (unsigned int *)0x00AAAC;
	int i,tmp,status;
	
	
	vuint16_t PORTxIN[4],PORTxOUT[4];
	
	initModesAndClock(); 				/* Initialize mode entries and system clock */
	
	INTC_InstallINTCInterruptHandler(isr,61,1);
	INTC.CPR.R = 0;
	
	//for(i = 0; i <= 3; i++) PORTxIN[i] = SIU.GPDO[i + 68].B.PDO;
	SIU.PCR[68].R = 0x0200;	
	SIU.PCR[69].R = 0x0200;		
	SIU.PCR[70].R = 0x0200;		
	SIU.PCR[71].R = 0x0200;		
	
	SIU.PCR[64].R = 0x0100;
	SIU.PCR[65].R = 0x0100;
	SIU.PCR[66].R = 0x0100;
	SIU.PCR[67].R = 0x0100;
	
	for(i = 68; i <= 70; i++) SIU.GPDO[i].B.PDO = 1;
	initPIT(ValTMR);
/*
	DFLASH.MCR.R = 0x00000010; 
	*P2 = 0xFFFFFF21; 
	*P1 = 0x00000020;
	DFLASH.MCR.R = 0x00000011; 
	do {
		tmp = DFLASH.MCR.R; 
	} while ( !(tmp & 0x00000400) );
	status = DFLASH.MCR.R & 0x0000020;
	DFLASH.MCR.R = 0x00000010; 
	DFLASH.MCR.R = 0x00000000; */
	
	while (1) {
		SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
	}
}

#endif