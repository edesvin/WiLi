/*
 * Application.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Robo
 */
#include "MPC5606B.h"
#include "Application.h"
#include "stdtypedef.h"

extern T_UBYTE rub_CountIndice;

extern volatile T_UBYTE rub_FlagValUpAut;
extern volatile T_UBYTE rub_FlagValUpMan;
extern volatile T_UBYTE rub_FValAutUP;
extern volatile T_UBYTE rub_FlagValDownAut;
extern volatile T_UBYTE rub_FlagValDownMan;
extern volatile T_UBYTE rub_FValAutDown;

extern volatile T_UBYTE rub_FlagValAnPi;


extern volatile T_UBYTE rub_Fled;

void WiLi_Aplication(void){

	if((rub_FlagValUpAut || rub_FlagValUpMan) && !rub_FlagValDownMan && !rub_FlagValDownAut){
		if(rub_Fled){
			rub_Fled = 0;
			SIU.GPDO[rub_CountIndice].B.PDO = 1;
			SIU.GPDO[10].B.PDO = 0;
			SIU.GPDO[11].B.PDO = 1;
			rub_CountIndice++;
			if(rub_CountIndice >= 10){
				SIU.GPDO[10].B.PDO = 1;
				SIU.GPDO[11].B.PDO = 1;
				rub_CountIndice = 10;
				rub_FlagValUpAut = 0;
				rub_FValAutUP = 0;
			}
		}
	}
/******************************************************************************/
	else if((rub_FlagValDownAut || rub_FlagValDownMan) && !rub_FlagValUpAut && !rub_FlagValUpMan){
		if(rub_Fled){
			rub_Fled = 0;
			if(rub_CountIndice <= 1) {
				rub_CountIndice = 1;
				rub_FlagValDownAut = 0;
				rub_FValAutDown = 0;
				SIU.GPDO[11].B.PDO = 1;
			}
			SIU.GPDO[rub_CountIndice-1].B.PDO = 0;
			SIU.GPDO[10].B.PDO = 1;
			SIU.GPDO[11].B.PDO = 0;
			rub_CountIndice--;
			
			if(rub_CountIndice == 0){
				SIU.GPDO[10].B.PDO = 1;
				SIU.GPDO[11].B.PDO = 1;
			}
			if(rub_CountIndice == 0 && rub_FlagValAnPi){
				SIU.GPDO[11].B.PDO = 1;
				PIT.CH[1].TCTRL.B.TEN = 0;
				PIT.CH[1].TCTRL.B.TEN = 1;
				PIT.CH[1].LDVAL.R = 0x1312CFFF;
				PIT.CH[1].TFLG.B.TIF = 1;
				rub_FlagValAnPi = 0;
				while(!PIT.CH[1].TFLG.B.TIF);
				PIT.CH[1].TFLG.B.TIF = 1;
				rub_Fled = 0;
				rub_FlagValDownAut = 0;
				rub_FlagValDownMan = 0;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
				rub_FValAutDown = 0;
				rub_FValAutUP = 0;
			}
			
		}
	}
	/******************************************************************************/
	else if(!rub_FlagValUpAut && !rub_FlagValUpMan && !rub_FlagValDownAut && !rub_FlagValDownMan){
		rub_Fled = 0;
		SIU.GPDO[10].B.PDO = 1;
		SIU.GPDO[11].B.PDO = 1;
	}
}

