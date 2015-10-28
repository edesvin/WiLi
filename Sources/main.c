//Version 1.0 setWindowLevel y setGPIO


#include "MPC5606B.h"
#include "IntcInterrupts.h"

#define ValTMR 0X61A7FF

#define Ten_ms 0x0000FA00
#define FourH_ms 0x0186A000
#define FiveH_ms 0x01E84800
#define OUTPUT 0x0200
#define INPUT 0x0100

int dly,lly;

volatile unsigned int flag_STM0 = 0;
volatile unsigned int flag_STM1 = 0;
volatile unsigned int flag_STM2 = 0;
unsigned int myvar = 0;
unsigned int i = 0, j = 0;
unsigned int a = 0;
/* main.c - eMIOS OPWM example */
/* Description:  eMIOS example using Modulus Counter and OPWM modes */
/* Rev 1.8 Mar 14 2010 SM - modified initModesAndClock, updated header file */
/* Copyright Freescale Semiconductor, Inc. 2004–2010 All rights reserved. */

void isr(void){
	if(STM.CH[0].CIR.B.CIF){ 
		flag_STM0 = 1;
		STM.CH[0].CIR.B.CIF = 0x01;
	}
	if(STM.CH[1].CIR.B.CIF){ 
		flag_STM1 = 1;
		STM.CH[1].CIR.B.CIF = 0x01;
	}
	if(STM.CH[2].CIR.B.CIF){ 
		flag_STM2 = 1;
		STM.CH[2].CIR.B.CIF = 0x01;
	}
}

void initModesAndClock(void) {
	ME.MER.R = 0x0000001D;          	/* Enable DRUN, RUN0, SAFE, RESET modes */
	                              		/* Initialize PLL before turning it on: */
										/* Use 1 of the next 2 lines depending on crystal frequency: */
	CGM.FMPLL_CR.R = 0x02400100;    	/* 8 MHz xtal: Set PLL0 to 64 MHz */   
	/*CGM.FMPLL_R = 0x12400100;*/     	/* 40 MHz xtal: Set PLL0 to 64 MHz */   
	ME.RUN[0].R = 0x001F0074;       	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	
	//ME.RUNPC[0].R = 0x00000010; 	  	/* Peri. Cfg. 0 settings: only run in RUN0 mode */
   										/* Use the next lines as needed for MPC56xxB/S: */  	    	
	//ME.PCTL[48].R = 0x0000;         	/* MPC56xxB LINFlex0: select ME.RUNPC[0] */	
	//ME.PCTL[68].R = 0x0000;         	/* MPC56xxB/S SIUL:  select ME.RUNPC[0] */	
	
	ME.RUNPC[1].R = 0x00000010;     	/* Peri. Cfg. 1 settings: only run in RUN0 mode */
	ME.PCTL[32].R = 0x01;       		/* MPC56xxB ADC 0: select ME.RUNPC[1] */
  	ME.PCTL[57].R = 0x01;       		/* MPC56xxB CTUL: select ME.RUNPC[1] */
  	ME.PCTL[48].R = 0x01;           	/* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] */
	ME.PCTL[68].R = 0x01;           	/* MPC56xxB/S SIUL:  select ME.RUNPC[1] */
	ME.PCTL[72].R = 0x01;           	/* MPC56xxB/S EMIOS 0:  select ME.RUNPC[1] */
	                              		/* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0;         	/* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         	/* Enter RUN0 Mode & Inverted Key */  
	while (ME.GS.B.S_MTRANS) {}     	/* Wait for mode transition to complete */    
	                          			/* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {}/* Verify RUN0 is the current mode */
	
	//while (ME.IS.B.I_MTC != 1) {}    /* Wait for mode transition to complete */    
	//ME.IS.R = 0x00000001;           /* Clear Transition flag */ 
}
void initModesAndClock2(void) {

	ME.MER.R = 0x000025FF;
	CGM.FMPLL_CR.R = 0x02400100;
	/* ME_PCTL[all periph] default points to ME_RUN_PC[0] */
	ME.RUNPC[0].R = 0x000000FE; /* Peripheral ON in every mode */
	/* Re-enter mode to latch mode settings */
	ME.MCTL.R = 0x30005AF0; /* Mode & Key */
	ME.MCTL.R = 0x3000A50F; /* Mode & Key inverted */	

}

void initSTM(void){
	INTC_InstallINTCInterruptHandler(isr,30,1);
	INTC_InstallINTCInterruptHandler(isr,31,1);
	INTC_InstallINTCInterruptHandler(isr,32,1);
	INTC.CPR.R = 0;
	
	STM.CR.B.CPS = 0x00; //Preescaler value : 1
	STM.CR.B.FRZ = 0x00; //Disable debug mode for timer.
	STM.CR.B.TEN = 0x01; //Enable timer.
	
	STM.CH[0].CCR.B.CEN = 0x01; //Enable channel 0
	STM.CH[0].CMP.R = Ten_ms; //Compare register
	/* STM.CH[0].CIR.B.CIF - Interrupt flag */
	
	STM.CH[1].CCR.B.CEN = 0x01; //Enable channel 1
	STM.CH[1].CMP.R = FourH_ms; //Compare register 
	/* STM.CH[1].CIR.B.CIF - Interrupt flag */
	
	STM.CH[2].CCR.B.CEN = 0x01; //Enable channel 2
	STM.CH[2].CMP.R = FiveH_ms; //Compare register 
}

void initPIT(void){
	PIT.PITMCR.B.MDIS = 0x00; /* enable PIT module */
	PIT.PITMCR.B.FRZ = 0x00;
	
	PIT.CH[2].LDVAL.R = ValTMR; /* load PIT counter */
	PIT.CH[2].TCTRL.B.TEN = 0x01; /* enable channel */
		
}

//level from 0 to 10;
void setWindowLevel(unsigned int level){
	for(i = 0; i<level; i++){
		SIU.GPDO[i].B.PDO = 0x01;	
	}
	for(i = level; i<10; i++){
		SIU.GPDO[i].B.PDO = 0x00;	
	}
}

void initGPIO(void){
	
	for(i = 0; i<12; i++){ 		/* OUTPUTS - A0 - A11 */
		SIU.PCR[i].R = OUTPUT;	
		SIU.GPDO[i].B.PDO = 0x00;	
	}
	
	SIU.PCR[64].R = INPUT; /* INPUTS */
	SIU.PCR[65].R = INPUT;
	SIU.PCR[66].R = INPUT;
	SIU.PCR[67].R = INPUT;
	
}
void main (void) {
	
	initModesAndClock(); 				/* Initialize mode entries and system clock */
	
	initSTM();
	//initPIT();

	SIU.PCR[68].R = 0x0200;				/* Program the drive enable pin of LED1 (PE4) as output*/
	//SIU.PCR[69].R = 0x0200;				/* Program the drive enable pin of LED2 (PE5) as output*/
	//SIU.PCR[70].R = 0x0200;				/* Program the drive enable pin of LED3 (PE6) as output*/
	//SIU.PCR[71].R = 0x0200;				/* Program the drive enable pin of LED4 (PE7) as output*/
	
	initGPIO();
	
	//ADC_0.MCR.B.PWDN = 0;
	
	STM.CNT.R = 0;

	STM.CR.B.TEN = 0x01;
	STM.CR.B.FRZ = 0x00;
	STM.CR.B.CPS = 0x00;

	STM.CH[0].CCR.B.CEN = 0x01;
	STM.CH[0].CMP.R = 320000000;

	
	
	
	
	STM.CNT.R = 0;


	
	while (1) 
	{
		//while(SIU.GPDI[64].B.PDI);
		//SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;
		
		for(j = 0; j<=10; j++){
			setWindowLevel(j);
			for(a = 0; a<1000000; a++);
		}
		/*myvar = SIU.GPDI[64].B.PDI;
		if( myvar ){
			SIU.GPDO[68].B.PDO = 0;
		}
		else{
			SIU.GPDO[68].B.PDO = 1;
		}
*/
		
	} /* end while(1) */
}/* end main */
