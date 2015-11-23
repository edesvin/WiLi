/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Tasks.c $
 * $Revision: 1.5 $
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
extern volatile T_UBYTE rub_FlagWait5seg;
/*==============================================================================
* Function: Task_0
*
* Description: Tarea que se ejecuta cada milisegundo, su funcion principal es 
* validar los pulsos en los push button de mas de 10 ms, asi como los pulsos 
* en el rango de 10 y 500 ms para entrar al modo automatico y los pulsos mayores
* a 500 ms para entrar al modo manual. 
*
==============================================================================*/
void Task_0(void){
	
//	SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;
	Val_PushB();
	Time5segAnpi();
}
/*==============================================================================
* Function: Task_1
*
* Description:  Tarea que se ejecuta cada 400 milisegundos, su funcion principal
* es mandar llamar a la maquina de estados y determinar el proximo estado de
* la barra de leds asi como el estado de los leds indicadores de direccion.
*
==============================================================================*/
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
