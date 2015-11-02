#include "MPC5606B.h"
#include "stdtypedef.h"
#include "IntcInterrupts.h"

#define Emain0 1
#if Emain0 == 1

#define PB_UP		SIU.GPDI[64].B.PDI
#define PB_DOWN 	SIU.GPDI[66].B.PDI
#define PB_AnPi 	SIU.GPDI[67].B.PDI

#define ValTMR_0 		0x0000F9FF
#define ValTMR_1 		0x1312CFFF

volatile T_UBYTE rub_FlagValUpAut = 0;
volatile T_UBYTE rub_FlagValUpMan = 0;
volatile T_UBYTE FValAutUP = 0;
volatile T_UBYTE rub_FlagValDownAut = 0;
volatile T_UBYTE rub_FlagValDownMan = 0;
volatile T_UBYTE FValAutDown = 0;

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
		

		////////////////////////////////////////////////
		if(PB_UP && !FValAutDown){
			FValAutDown = 0;
			ruw_CountUp++;
			if(ruw_CountUp >= 10 && ruw_CountUp < 500){
				F400=1;
				rub_FlagValUpAut = 1;
				rub_FlagValUpMan = 0;
				rub_FlagValDownAut = 0;
				rub_FlagValDownMan = 0;
			}
			else if(ruw_CountUp >= 500 && !FValAutUP){
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 1;
				rub_FlagValDownAut = 0;
				rub_FlagValDownMan = 0;
				if(ruw_CountUp > 4000) ruw_CountUp = 4000;
			}
			else;
		}
		else{
			if(rub_FlagValUpAut){
				FValAutUP = 1;
			}
			ruw_CountUp = 0;
			rub_FlagValUpMan = 0;
		}
		////////////////////////////////////////////////
		if(PB_DOWN && !FValAutUP){
			ruw_CountDown++;
			FValAutUP = 0;
			if(ruw_CountDown >= 10 && ruw_CountDown < 500){
				F400=1;
				rub_FlagValDownAut = 1;
				rub_FlagValDownMan = 0;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
			}
			else if(ruw_CountDown >= 500 && !FValAutDown){
				rub_FlagValDownAut = 0;
				rub_FlagValDownMan = 1;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
				if(ruw_CountDown > 4000) ruw_CountDown = 4000;
			}
			else;
		}
		else{
			if(rub_FlagValDownAut){ 
				FValAutDown = 1;			
			}
			ruw_CountDown = 0;
			rub_FlagValDownMan = 0;
		}
		////////////////////////////////////////////////
		if(PB_AnPi && (rub_FlagValUpAut || rub_FlagValUpMan) && !rub_FlagValDownAut && !rub_FlagValDownMan){
			ruw_CountAnPi++;
			FValAutUP = 0;
			if(ruw_CountAnPi >= 10){
				rub_FlagValAnPi = 1;
				ruw_CountAnPi = 0;
				
				rub_FlagValDownAut = 1;
				rub_FlagValDownMan = 0;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
				FValAutDown = 1;
				
			}
		}
		else{
			ruw_CountAnPi = 0;
		}
		////////////////////////////////////////////////
		
		if(PB_UP && PB_DOWN ){
			rub_FlagValDownAut = 0;
			rub_FlagValDownMan = 0;
			rub_FlagValUpAut = 0;
			rub_FlagValUpMan = 0;
			FValAutDown = 0;
			FValAutUP = 0;
		}
		
		/////////////////////////////////////////////////
		
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
		
		if(rub_FlagValUpAut || rub_FlagValUpMan){
			if(Fled){
				Fled = 0;
				SIU.GPDO[lub_CountIndice].B.PDO = 1;
				SIU.GPDO[10].B.PDO = 0;
				SIU.GPDO[11].B.PDO = 1;
				lub_CountIndice++;
				if(lub_CountIndice >= 10){
					lub_CountIndice = 10;
					rub_FlagValUpAut = 0;
					FValAutUP = 0;
				}
				

			}
		}
		////////////////////////////////////////////////
		else if(rub_FlagValDownAut || rub_FlagValDownMan){
			if(Fled){
				Fled = 0;
				if(lub_CountIndice <= 1) {
					lub_CountIndice = 1;
					rub_FlagValDownAut = 0;
					FValAutDown = 0;
					SIU.GPDO[11].B.PDO = 1;
				}
				SIU.GPDO[lub_CountIndice-1].B.PDO = 0;
				SIU.GPDO[10].B.PDO = 1;
				SIU.GPDO[11].B.PDO = 0;
				lub_CountIndice--;
				
				if(lub_CountIndice == 0 && rub_FlagValAnPi){
					SIU.GPDO[11].B.PDO = 1;
					PIT.CH[1].TCTRL.B.TEN = 0;
					PIT.CH[1].TCTRL.B.TEN = 1;
					PIT.CH[1].LDVAL.R = ValTMR_1;
					PIT.CH[1].TFLG.B.TIF = 1;
					rub_FlagValAnPi = 0;
					while(!PIT.CH[1].TFLG.B.TIF);
					PIT.CH[1].TFLG.B.TIF = 1;
					Fled = 0;
					rub_FlagValDownAut = 0;
					rub_FlagValDownMan = 0;
					rub_FlagValUpAut = 0;
					rub_FlagValUpMan = 0;
					FValAutDown = 0;
					FValAutUP = 0;
				}
				
			}
		}
		////////////////////////////////////////////////
		else if(!rub_FlagValUpAut && !rub_FlagValUpMan && !rub_FlagValDownAut && !rub_FlagValDownMan){
			Fled = 0;
			SIU.GPDO[10].B.PDO = 1;
			SIU.GPDO[11].B.PDO = 1;
		}
	}
}

#endif



