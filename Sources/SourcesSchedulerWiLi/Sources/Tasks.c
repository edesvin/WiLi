/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Tasks.c $
 * $Revision: 1.5 $
 * $Author: 	Adrián Zacarías Siete $
 *		Edgar Escayola Vinagre $
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
/*                     |         WiLi       |         1.8                      */
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
extern volatile T_UBYTE rub_FlagWait5seg;
/*==============================================================================
* Function: Task_0
*
* Description: This task is executed every millisecond. Its main function is 
* to validate the presses of the push buttons. They should be longer than 10 msec.
* It also validates when the push buttons are pressed in the range of 10 to 500 msec
* in order to determine whether it should go to the automatic or manual mode.
*
==============================================================================*/
void Task_0(void){
	
	Val_PushB();
	Time5segAnpi();
	
}
/*==============================================================================
* Function: Task_1
*
* Description: This task is executed every 400 milliseconds. Its main function is 
* to call the state machine that handles the movements in order to determine the 
* next state of the LED bar and the direction indicators.
*
==============================================================================*/
void Task_1(void){
	
	Func_Dir();
	Func_LEDsUpDown();
	
	if(!rub_FlagWait5seg){
		StateMachine();
	}
	else{
		/****/
	}
}
