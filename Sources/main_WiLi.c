#include "MPC5606B.h"
#include "stdtypedef.h"
#include "IntcInterrupts.h"

#define Emain0 1
#if Emain0 == 1

#define PB_UP		SIU.GPDI[64].B.PDI
#define PB_DOWN 	SIU.GPDI[65].B.PDI
#define PB_AnPi 	SIU.GPDI[66].B.PDI

#define ValTMR_0 		0x0000F9FF
#define ValTMR_1 		0x01869FFF

volatile T_UBYTE rub_FlagValUpAut = 0;
volatile T_UBYTE rub_FlagValUpMan = 0;
volatile T_UBYTE rub_FlagValDownAut = 0;
volatile T_UBYTE rub_FlagValDownMan = 0;
volatile T_UBYTE rub_FlagValAnPi = 0;

volatile T_UWORD ruw_CountUp = 0;
volatile T_UWORD ruw_CountDown = 0;
volatile T_UWORD ruw_CountAnPi = 0;

volatile T_UBYTE F400 = 0;
volatile T_UBYTE Fled = 0;
volatile T_UWORD count400 = 0;


void isr(void){
	if( PIT.CH[0].TFLG.B.TIF ){
		PIT.CH[0].TFLG.B.TIF = 1;
		
		if(PB_UP){
			ruw_CountUp++;
			if(ruw_CountUp >= 10 && ruw_CountUp < 500){
				F400=1;
				rub_FlagValUpAut = 1;
			}
			else if(ruw_CountUp >= 500){
				rub_FlagValUpMan = 1;
				if(ruw_CountUp > 4000) ruw_CountUp = 4000;
			}
			

		}
		else{
			ruw_CountUp = 0;
		}
		
		if(F400){
			count400++;
			if(count400>=400){
				count400=0;
				Fled=1;
			}
		}
		
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
void main (void) {
	
	T_UWORD luw_i;
	T_UBYTE lub_CountIndice = 0;
	
	initModesAndClock(); 								/* Initialize mode entries and system clock */
	
	INTC_InstallINTCInterruptHandler(isr,59,1);
	INTC.CPR.R = 0;
	
	for(luw_i = 0; luw_i <=11; luw_i++) SIU.PCR[luw_i].R = 0x0200;
	for(luw_i = 64; luw_i <=67; luw_i++) SIU.PCR[luw_i].R = 0x0100;
	for(luw_i = 0; luw_i <= 9; luw_i++) SIU.GPDO[luw_i].B.PDO = 0;
	SIU.GPDO[10].B.PDO = 1;
	SIU.GPDO[11].B.PDO = 1;
	
	SIU.PCR[68].R = 0x0200;		

	initPIT(ValTMR_0, ValTMR_1);
	
	while (1) {
		
		if(rub_FlagValUpAut){
			PIT.CH[1].TCTRL.B.TEN = 0;
			PIT.CH[1].TCTRL.B.TEN = 1;
			rub_FlagValUpAut = 0;
			while(lub_CountIndice <= 9){
				if(!rub_FlagValUpMan){
					while(!Fled);
					Fled=0;
					//PIT.CH[1].TFLG.B.TIF = 1;
					SIU.GPDO[lub_CountIndice].B.PDO = 1;
					lub_CountIndice++;
				}
			}
		}
	}
}

#endif



