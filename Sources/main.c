/* The implementation of the 500mS timer is correct */

#include "MPC5606B.h"
#include "IntcInterrupts.h"

#define ValTMR 		0X61A7FF

#define Ten_ms 		0x0000FA01
#define FourH_ms	0x0186A001
#define FiveH_ms 	0x0061A801

#define OUTPUT 		0x0200
#define INPUT 		0x0100
#define LED_DOWN	SIU.GPDO[10].B.PDO
#define LED_UP	 	SIU.GPDO[11].B.PDO
#define RGB_ON		0x00
#define RGB_OFF		0x01
#define _OK			0x01
#define RESET_STM_COUNTER STM.CNT.R = 0

volatile unsigned int flag_Ten_ms = 0;
volatile unsigned int flag_FourH_ms = 0;
volatile unsigned int flag_FiveH_ms = 0;

unsigned int myvar = 0, Ten_ms_passed = 0;
unsigned int i = 0, break_flag = 0;
int j = 0;
unsigned int a = 0, GPDI_value = 0, Automatic = 0;
unsigned int direction = 0, first_time = 0;
unsigned long int MyTimer = 0;

/* main.c - eMIOS OPWM example */
/* Description:  eMIOS example using Modulus Counter and OPWM modes */
/* Rev 1.8 Mar 14 2010 SM - modified initModesAndClock, updated header file */
/* Copyright Freescale Semiconductor, Inc. 2004–2010 All rights reserved. */

void reset_flags(void){
	flag_Ten_ms = 0;
	flag_FourH_ms = 0;
	flag_FiveH_ms = 0;
}
void isr(void){
	if(STM.CH[0].CIR.B.CIF){ 
		flag_Ten_ms = 1;
		STM.CH[0].CIR.B.CIF = 0x01;
	}
	if(STM.CH[1].CIR.B.CIF){ 
		flag_FourH_ms = 1;
		STM.CH[1].CIR.B.CIF = 0x01;
	}
	if(STM.CH[2].CIR.B.CIF){ 
		flag_FiveH_ms = 1;
		STM.CH[2].CIR.B.CIF = 0x01;
	}
}

void initModesAndClock(void) {
	ME.MER.R = 0x0000001D;          	/* Enable DRUN, RUN0, SAFE, RESET modes */
	                              		/* Initialize PLL before turning it on: */
										/* Use 1 of the next 2 lines depending on crystal frequency: */
	CGM.FMPLL_CR.R = 0x02400100;    	/* 8 MHz xtal: Set PLL0 to 64 MHz */   
	
	ME.RUN[0].R = 0x001F0074;       	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	
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
}

void initSTM(void){
	INTC_InstallINTCInterruptHandler(isr,30,1);
	//INTC_InstallINTCInterruptHandler(isr,31,1);
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
	
	STM.CH[2].CCR.B.CEN = 0x00; //Enable channel 2
	STM.CH[2].CMP.R = FiveH_ms; //Compare register 
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
	
	for(i = 0; i<12; i++){ 			/* OUTPUTS - A0 - A11 */
		SIU.PCR[i].R = OUTPUT;		/* OUTPUT = 0x0200 */
		SIU.GPDO[i].B.PDO = 0x00;	
	}
	SIU.GPDO[10].B.PDO = 0x01;	
	SIU.GPDO[11].B.PDO = 0x01;	
	
	SIU.PCR[64].R = INPUT; /* INPUT = 0x0100 */
	SIU.PCR[65].R = INPUT;
	SIU.PCR[66].R = INPUT;
	SIU.PCR[67].R = INPUT;
	
	//SIU.PCR[68].R = OUTPUT;				/* Program the drive enable pin of LED1 (PE4) as output*/
	//SIU.PCR[69].R = 0x0200;				/* Program the drive enable pin of LED2 (PE5) as output*/
	//SIU.PCR[70].R = 0x0200;				/* Program the drive enable pin of LED3 (PE6) as output*/
	//SIU.PCR[71].R = 0x0200;				/* Program the drive enable pin of LED4 (PE7) as output*/
	
}
	void main (void) {
	
	initModesAndClock(); 				/* Initialize mode entries and system clock */
	
	initSTM();
	
	initGPIO();
		
	STM.CNT.R = 0;
	j = 0;
	setWindowLevel( (unsigned int) j );
	while (1) 
	{
		while(!SIU.GPDI[64].B.PDI && !SIU.GPDI[65].B.PDI);
		if(SIU.GPDI[64].B.PDI){
			GPDI_value = 64;
		}else{
			GPDI_value = 65;
		}			
		
		reset_flags();
		RESET_STM_COUNTER;
		Ten_ms_passed = 0;
				
		while(SIU.GPDI[GPDI_value].B.PDI && !flag_Ten_ms);
		if(SIU.GPDI[GPDI_value].B.PDI){
			Ten_ms_passed = _OK;
		}else{
			Ten_ms_passed = 0;
		}
		
		
		if(Ten_ms_passed){
			first_time = 1;
			break_flag = 1;
			while(break_flag){
				if(GPDI_value == 65){
					j--;
					direction = 0x00;
				}else if(GPDI_value == 64){
					j++;
					direction = 0x01;
				}
				
				if(j == 11){
					j = 10;
					break_flag = 0;
				}else if(j == -1){
					j = 0;
					break_flag = 0;
				}else{ 
					if(direction){
						LED_UP = RGB_ON;
					}else{
						LED_DOWN = RGB_ON;
					}
				}
					
				setWindowLevel(j);
				

				
				RESET_STM_COUNTER;
				
				while(!STM.CH[1].CIR.B.CIF && !(!first_time && flag_FiveH_ms && SIU.GPDI[GPDI_value].B.PDI) );
			
				if(!first_time && flag_FiveH_ms && SIU.GPDI[GPDI_value].B.PDI)
					break;
				
				STM.CH[1].CIR.B.CIF = 0x01;
		
				if(first_time){
					flag_FiveH_ms = 0;
					RESET_STM_COUNTER;
					STM.CH[2].CCR.B.CEN = 0x01;
					first_time = 0;
				}else if(!SIU.GPDI[GPDI_value].B.PDI){
					STM.CH[2].CCR.B.CEN = 0x00;
					flag_FiveH_ms = 0;
				}
					
				//if(flag_FiveH_ms && SIU.GPDI[GPDI_value].B.PDI)
				//	break_flag = 0;
			}
			
			if(flag_FiveH_ms){
				while(SIU.GPDI[GPDI_value].B.PDI){
					RESET_STM_COUNTER;
					while(!STM.CH[1].CIR.B.CIF);
					STM.CH[1].CIR.B.CIF = 0x01;
					
					if(SIU.GPDI[65].B.PDI){
						j--;
						direction = 0x00;
					}else if(SIU.GPDI[64].B.PDI){
						j++;
						direction = 0x01;
					}
					
					if(j == 11){
						j = 10;
					}else if(j == -1){
						j = 0;
					}else{ 
						if(direction){
							LED_UP = RGB_ON;
						}else{
							LED_DOWN = RGB_ON;
						}
					}
						
					setWindowLevel(j);
				
				}
			}
			
			LED_UP = RGB_OFF;
			LED_DOWN = RGB_OFF;
			
		} /* End ten ms while*/		
		
	} /* end while(1) */
}/* end main */
