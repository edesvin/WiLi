/*
 * MAL.c
 *
 *  Created on: Nov 2, 2015
 *      Author: Robo
 */

#include "MPC5606B.h"
#include "MAL.h"
#include "stdtypedef.h"

extern T_UBYTE rub_CountIndice;

extern volatile T_UBYTE rub_FlagValUpAut;
extern volatile T_UBYTE rub_FlagValUpMan;
extern volatile T_UBYTE rub_FValAutUP;
extern volatile T_UBYTE rub_FlagValDownAut;
extern volatile T_UBYTE rub_FlagValDownMan;
extern volatile T_UBYTE rub_FValAutDown;

extern volatile T_UBYTE rub_FlagValAnPi;

extern volatile T_UWORD ruw_CountUp;
extern volatile T_UWORD ruw_CountDown;
extern volatile T_UWORD ruw_CountAnPi;

extern volatile T_UBYTE rub_F400;
extern volatile T_UBYTE rub_Fled;
extern volatile T_UWORD ruw_count400;


void isr(void){
	if( PIT.CH[0].TFLG.B.TIF ){
		PIT.CH[0].TFLG.B.TIF = 1;
		/******************************************************************************/
		if(PB_UP && !rub_FlagValAnPi ){
			rub_FValAutDown = 0;
			ruw_CountUp++;
			if(ruw_CountUp >= 10 && ruw_CountUp < 500){
				rub_F400=1;
				rub_FlagValUpAut = 1;
				rub_FlagValUpMan = 0;
				rub_FlagValDownAut = 0;
				rub_FlagValDownMan = 0;
			}
			else if(ruw_CountUp >= 500 && !rub_FValAutUP){
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
				rub_FValAutUP = 1;
			}
			ruw_CountUp = 0;
			rub_FlagValUpMan = 0;
		}
		/******************************************************************************/
		if(PB_DOWN ){
			ruw_CountDown++;
			rub_FValAutUP = 0;
			if(ruw_CountDown >= 10 && ruw_CountDown < 500){
				rub_F400=1;
				rub_FlagValDownAut = 1;
				rub_FlagValDownMan = 0;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
			}
			else if(ruw_CountDown >= 500 && !rub_FValAutDown){
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
				rub_FValAutDown = 1;			
			}
			ruw_CountDown = 0;
			rub_FlagValDownMan = 0;
		}
		/**************************************************************************************************/
		
		if(PB_AnPi && (rub_FlagValUpAut || rub_FlagValUpMan) && !rub_FlagValDownAut && !rub_FlagValDownMan && rub_CountIndice != 10){
			ruw_CountAnPi++;
			rub_FValAutUP = 0;
			if(ruw_CountAnPi >= 10){
				rub_FlagValAnPi = 1;
				ruw_CountAnPi = 0;
				
				rub_FlagValDownAut = 1;
				rub_FlagValDownMan = 0;
				rub_FlagValUpAut = 0;
				rub_FlagValUpMan = 0;
				rub_FValAutDown = 1;
				
			}
		}
		else{
			ruw_CountAnPi = 0;
		}
		/******************************************************************************/
		
		if(PB_UP && PB_DOWN && (!rub_FlagValUpAut && !rub_FlagValDownAut)){
			rub_FlagValDownAut = 0;
			rub_FlagValDownMan = 0;
			rub_FlagValUpAut = 0;
			rub_FlagValUpMan = 0;
			rub_FValAutDown = 0;
			rub_FValAutUP = 0;
		}
		
		/******************************************************************************/
		
		if(rub_F400){
			ruw_count400++;
			if(ruw_count400>=400){
				ruw_count400=0;
				rub_Fled=1;
			}
		}
		
	}
}
