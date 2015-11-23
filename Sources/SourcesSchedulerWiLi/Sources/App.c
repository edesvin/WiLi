/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: App.c $
 * $Revision: 1.1 $
 * $Author: 	Adrián Zacarías Siete $
				Edgar Escayola Vinagre $
 * $Date: 20-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \App.c
    This file contains all the functions implemented in the tasks used by the 
    scheduler.
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
/*                     |          WiLi      |           1.1                   */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: App.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "App.h"
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
/*==============================================================================
* Function: Func_UP
*
* Description: Function which generates the UP movement.
*
==============================================================================*/
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
/*==============================================================================
* Function: Func_DOWN
*
* Description: Function which generates the DOWN movement.
*
==============================================================================*/
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
/*==============================================================================
* Function: Func_IDLE
*
* Description: This function stops any signal showing movement.
*
==============================================================================*/
void Func_IDLE(void){
	LED_UP = OFF;
	LED_DOWN = OFF;	
}
/*==============================================================================
* Function: Func_Dir
*
* Description: Function which sets the direction of the movement according to 
* the direction flags.
*
==============================================================================*/
void Func_Dir(void){
	
	if(rub_FlagValUpAut || rub_FlagValUpMan){
		rub_Direction = L_UP;
	}
	else if(rub_FlagValDownAut || rub_FlagValDownMan){
		rub_Direction = L_DOWN;
		if(rub_FlagValAnPi && rsb_CountIndex == 0){
			rub_Direction = L_IDLE;
			rub_FlagValAnPi = 0;
			FIRST_LED = 0;
			ruw_CounterWait5seg = FIVE_SECOND;
		}
	}
	else if(!rub_FlagValUpAut && !rub_FlagValUpMan && !rub_FlagValDownAut && !rub_FlagValDownMan){
		rub_Direction = L_IDLE;
	}
	else{
		rub_Direction = L_IDLE;
	}
}
/*==============================================================================
* Function: Val_PushB
*
* Description: This function validates the status of each push button. It is 
* called by a task that runs every millisecond. It checks the status of the 
* push buttons in order to consider it a valid press, and then it could run 
* the functions that will change the status of the application. 
*
==============================================================================*/
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
	if(!PB_DOWN && PB_AN_PI && (rub_FlagValUpAut || rub_FlagValUpMan) && !rub_FlagFinalPositionUP){
		Val_PB_AnPi();
	}
}
/*==============================================================================
* Function: Reset_All_Flags
*
* Description: Function that resets all the flags that have to do with movement.
*
==============================================================================*/
void Reset_All_Flags(void){

	Reset_Dir_Flags();
	rub_FValAutDown = 0;
	rub_FValAutUP = 0;
}
/*==============================================================================
* Function: Reset_Dir_Flags
*
* Description: Function that resets all the flags that have to do with the 
* direction movement.
*
==============================================================================*/
void Reset_Dir_Flags(void){
	rub_FlagValDownAut = 0;
	rub_FlagValDownMan = 0;
	rub_FlagValUpAut = 0;
	rub_FlagValUpMan = 0;
}
/*==============================================================================
* Function: Func_LEDsUpDown
*
* Description: Function that sets the LED indicators that show the direction 
* of the movement.
*
==============================================================================*/
void Func_LEDsUpDown(void){
	if((rub_FlagValUpAut || rub_FlagValUpMan) && rsb_CountIndex <= 9 && !rub_FlagFinalPositionUP){
		LED_UP = ON;
		LED_DOWN = OFF;
	}
	else if((rub_FlagValDownAut || rub_FlagValDownMan) && rsb_CountIndex >= 0 && !rub_FlagFinalPositionDown){
		LED_UP = OFF;
		LED_DOWN = ON;
	}
	else{
		LED_UP = OFF;
		LED_DOWN = OFF;
	}
}
/*==============================================================================
* Function: Val_PB_UP
*
* Description: Function that sets and evaluates de counters in order to know 
* for how long the push button UP has been pressed.
*
==============================================================================*/
void Val_PB_UP(void){
	ruw_CountUp++;
	rub_FValAutDown = 0;
	
	if(ruw_CountUp >= VALID_VALUE && ruw_CountUp < MANUAL_VALUE){
		Reset_Dir_Flags();
		rub_FlagValUpAut = 1;
	}
	
	else if(ruw_CountUp >= MANUAL_VALUE && !rub_FValAutUP){
		Reset_Dir_Flags();
		rub_FlagValUpMan = 1;
		if(ruw_CountUp > VAL_MAX_COUNTER_PUSH) ruw_CountUp = VAL_MAX_COUNTER_PUSH;
	}
	
	else{
		/**/
	}
}
/*==============================================================================
* Function: Val_PB_DOWN
*
* Description: Function that sets and evaluates de counters in order to know 
* for how long the push button DOWN has been pressed.
*
==============================================================================*/
void Val_PB_DOWN(void){
	ruw_CountDown++;
	rub_FValAutUP = 0;
	
	if(ruw_CountDown >= VALID_VALUE && ruw_CountDown < MANUAL_VALUE){
		Reset_Dir_Flags();
		rub_FlagValDownAut = 1;
	}
	
	else if(ruw_CountDown >= MANUAL_VALUE && !rub_FValAutDown){
		Reset_Dir_Flags();
		rub_FlagValDownMan = 1;
		if(ruw_CountDown > VAL_MAX_COUNTER_PUSH) ruw_CountDown = VAL_MAX_COUNTER_PUSH;
	}
	
	else{
		/**/
	}
}
/*==============================================================================
* Function: Val_PB_AnPi
*
* Description: Function that sets and evaluates de counters in order to know
* for how long the push button of the signal Anti Pinch has been pressed.
*
==============================================================================*/
void Val_PB_AnPi(void){
	ruw_CountAnPi++;
	rub_FValAutUP = 0;
	
	if(ruw_CountAnPi >= VALID_VALUE){
		
		rub_FlagValAnPi = 1;
		ruw_CountAnPi = 0;
		Reset_Dir_Flags();
		rub_FlagValDownAut = 1;
		rub_FValAutDown = 1;
	}
}
/*==============================================================================
* Function: InvalidButtonPress
*
* Description: This function clears the flags and sets the values necessary to
* declare an invalid button press.
*
==============================================================================*/
void InvalidButtonPress(void){
	Reset_All_Flags();
	ruw_CountDown = MANUAL_VALUE;
	ruw_CountUp = MANUAL_VALUE;
}
/*==============================================================================
* Function: NoButtonPress
*
* Description: This function sets the flags to indicate that a button is not
* being pressed.
*
==============================================================================*/
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
/*==============================================================================
* Function: StateMachine
*
* Description: Function that holds the main state machine of the application. 
* It chooses in which state it should be.
*
==============================================================================*/
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
/*==============================================================================
* Function: Reset_VarBarLeds
*
* Description: Function that clears flags and counters generated by the push 
* buttons.
*
==============================================================================*/
void Reset_VarBarLeds(void){
	rsb_CountIndex = 0;
	rub_FlagL_DOWN = 0;
	rub_FlagL_UP = 0;	
}
/*==============================================================================
* Function: Time5segAnpi
*
* Description: Function that decrements the value of the variable used to 
* represent the 5 seconds delay. It also sets the flags so that the code will
* freeze after a Pinch.
*
==============================================================================*/
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

