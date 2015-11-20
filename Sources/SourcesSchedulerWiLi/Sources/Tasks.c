/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Tasks.c $
 * $Revision: 1.0 $
 * $Author: Adrián Zacarías Siete $
 * $Date: 17-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    It holds the implementation of the functions that contain the tasks.
*/
/*============================================================================*/
/* COPYRIGHT (C) CONTINENTAL AUTOMOTIVE 2014                                  */
/* AUTOMOTIVE GROUP, Interior Division, Body and Security                     */
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/
/*============================================================================*/
/*                    REUSE HISTORY - taken over from                         */
/*============================================================================*/
/*  DATABASE           |        PROJECT     | FILE VERSION (AND INSTANCE)     */
/*----------------------------------------------------------------------------*/
/*                     |                    |                                 */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: Tasks.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Tasks.h"
/*============================================================================*/
extern volatile T_UBYTE rub_Direction = 0;
extern T_SBYTE rsb_CountIndex = 0;
/*============================================================================*/

extern volatile T_UBYTE rub_FlagValUpAut = 0;
extern volatile T_UBYTE rub_FlagValUpMan = 0;
extern volatile T_UBYTE rub_FValAutUP = 0;
extern volatile T_UBYTE rub_FlagValDownAut = 0;
extern volatile T_UBYTE rub_FlagValDownMan = 0;
extern volatile T_UBYTE rub_FValAutDown = 0;

extern volatile T_UBYTE rub_FlagValAnPi = 0;
extern volatile T_UWORD ruw_CounterWait5seg = 0;
extern volatile T_UBYTE rub_FlagWait5seg = 0;

extern volatile T_UWORD ruw_CountUp = 0;
extern volatile T_UWORD ruw_CountDown = 0;
extern volatile T_UWORD ruw_CountAnPi = 0;

extern volatile T_UBYTE rub_FlagL_UP = 0;
extern volatile T_UBYTE rub_FlagL_DOWN = 0;

extern volatile T_UBYTE rub_FlagFinalPositionUP = 0;
extern volatile T_UBYTE rub_FlagFinalPositionDown = 0;
/*============================================================================*/
void Task_0(void){
	
//	SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;
	Val_PushB();
	Time5segAnpi();
}
/*============================================================================*/
void Task_1(void){

//	SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
	
	Func_Dir();
	Func_LEDsUpDown();
	
	if(!rub_FlagWait5seg){
		StateMachine();
	}
	else{
		/****/
	}
}
/*============================================================================*/
T_SBYTE Func_UP(T_SBYTE lsb_index){
	rub_FlagFinalPositionDown = 0;
	rub_FlagL_UP = 1;
	if(rub_FlagL_DOWN){
		rub_FlagL_DOWN = 0;
		lsb_index++;
	}
	SIU.GPDO[lsb_index].B.PDO = 1;
	lsb_index++;
	
	if( lsb_index >= 10 ){
		Reset_All_Flags();
		lsb_index = 9;
		rub_FlagL_UP = 0;
		rub_FlagL_DOWN = 0;
		rub_FlagFinalPositionUP = 1;
	}
	return lsb_index;
}
/*============================================================================*/
T_SBYTE Func_DOWN(T_SBYTE lsb_index){
	rub_FlagFinalPositionUP = 0;
	rub_FlagL_DOWN = 1;
	if(rub_FlagL_UP){
		rub_FlagL_UP = 0;
		lsb_index--;
	}
	SIU.GPDO[lsb_index].B.PDO = 0;
	if(lsb_index <= 0 ){
		rub_FlagFinalPositionDown = 1;
		Reset_All_Flags();
		lsb_index = 0;
		rub_FlagL_UP = 0;
		rub_FlagL_DOWN = 0;
	}
	else{
		lsb_index--;
	}
	return lsb_index;
}
/*============================================================================*/
void Func_IDLE(void){
	Led_UP = OFF;
	Led_DOWN = OFF;	
}
/*============================================================================*/
void Func_Dir(void){
	
	if(rub_FlagValUpAut || rub_FlagValUpMan){
		rub_Direction = L_UP;
	}
	else if(rub_FlagValDownAut || rub_FlagValDownMan){
		rub_Direction = L_DOWN;
		if(rub_FlagValAnPi && rsb_CountIndex == 0){
			rub_Direction = L_IDLE;
			rub_FlagValAnPi = 0;
			First_Led = 0;
			ruw_CounterWait5seg = 5000;
		}
	}
	else if(!rub_FlagValUpAut && !rub_FlagValUpMan && !rub_FlagValDownAut && !rub_FlagValDownMan){
		rub_Direction = L_IDLE;
	}
	else{
		rub_Direction = L_IDLE;
	}
}
/*============================================================================*/
void Val_PushB(void){
	
	if(PB_UP && !PB_DOWN  && !rub_FlagValAnPi){
		Val_PB_UP();
	}
	else if(!PB_UP && PB_DOWN ){
		Val_PB_DOWN();
	}
	else if(PB_UP && PB_DOWN && !rub_FlagValAnPi){
		InvalidButtonPress();
	}
	else{	
		NoButtonPress();
	}
	if(!PB_DOWN && PB_AnPi && (rub_FlagValUpAut || rub_FlagValUpMan) && !rub_FlagFinalPositionUP){
		Val_PB_AnPi();
	}
}
/*============================================================================*/
void Reset_All_Flags(void){
	rub_FlagValDownAut = 0;
	rub_FlagValDownMan = 0;
	rub_FlagValUpAut = 0;
	rub_FlagValUpMan = 0;
	rub_FValAutDown = 0;
	rub_FValAutUP = 0;
}
/*============================================================================*/
void Reset_Dir_Flags(void){
	rub_FlagValDownAut = 0;
	rub_FlagValDownMan = 0;
	rub_FlagValUpAut = 0;
	rub_FlagValUpMan = 0;
}
/*============================================================================*/
void Func_LEDsUpDown(void){
	if((rub_FlagValUpAut || rub_FlagValUpMan) && rsb_CountIndex <= 9 && !rub_FlagFinalPositionUP){
		Led_UP = ON;
		Led_DOWN = OFF;
	}
	else if((rub_FlagValDownAut || rub_FlagValDownMan) && rsb_CountIndex >= 0 && !rub_FlagFinalPositionDown){
		Led_UP = OFF;
		Led_DOWN = ON;
	}
	else{
		Led_UP = OFF;
		Led_DOWN = OFF;
	}
}
/*============================================================================*/
void Val_PB_UP(void){
	ruw_CountUp++;
	rub_FValAutDown = 0;
	
	if(ruw_CountUp >= 10 && ruw_CountUp < 500){
		Reset_Dir_Flags();
		rub_FlagValUpAut = 1;
	}
	
	else if(ruw_CountUp >= 500 && !rub_FValAutUP){
		Reset_Dir_Flags();
		rub_FlagValUpMan = 1;
		if(ruw_CountUp > 4000) ruw_CountUp = 4000;
	}
	
	else{
		/**/
	}
}
/*============================================================================*/
void Val_PB_DOWN(void){
	ruw_CountDown++;
	rub_FValAutUP = 0;
	
	if(ruw_CountDown >= 10 && ruw_CountDown < 500){
		Reset_Dir_Flags();
		rub_FlagValDownAut = 1;
	}
	
	else if(ruw_CountDown >= 500 && !rub_FValAutDown){
		Reset_Dir_Flags();
		rub_FlagValDownMan = 1;
		if(ruw_CountDown > 4000) ruw_CountDown = 4000;
	}
	
	else{
		/**/
	}
}
/*============================================================================*/
void Val_PB_AnPi(void){
	ruw_CountAnPi++;
	rub_FValAutUP = 0;
	
	if(ruw_CountAnPi >= 10){
		
		rub_FlagValAnPi = 1;
		ruw_CountAnPi = 0;
		Reset_Dir_Flags();
		rub_FlagValDownAut = 1;
		rub_FValAutDown = 1;
	}
}
/*============================================================================*/
void InvalidButtonPress(void){
	Reset_All_Flags();
	ruw_CountDown = 500;
	ruw_CountUp = 500;
}
/*============================================================================*/
void NoButtonPress(void){
	rub_FlagValUpMan = 0;
	rub_FlagValDownMan = 0;
	ruw_CountUp = 0;
	ruw_CountDown = 0;
	if(rub_FlagValUpAut){
		rub_FValAutUP = 1;
	}
	if(rub_FlagValDownAut){ 
		rub_FValAutDown = 1;			
	}
}
/*============================================================================*/
void StateMachine(void){
	switch (rub_Direction) {
		
		case L_UP:
			rsb_CountIndex = Func_UP(rsb_CountIndex);
			break;
		
		case L_DOWN:
			rsb_CountIndex = Func_DOWN(rsb_CountIndex);				
			break;
			
		default:
			Func_IDLE();
			break;
			
	}
}
/*============================================================================*/
void Reset_VarBarLeds(void){
	rsb_CountIndex = 0;
	rub_FlagL_DOWN = 0;
	rub_FlagL_UP = 0;	
}
/*============================================================================*/
void Time5segAnpi(void){
	if(ruw_CounterWait5seg > 0){
		rub_FlagWait5seg = 1;	
		ruw_CounterWait5seg--;
		rub_FlagValAnPi = 0;
		Reset_All_Flags();
		Reset_VarBarLeds();
		}
	else{
		rub_FlagWait5seg = 0;
	}	
}
