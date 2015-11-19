/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Init_Tasks.c $
 * $Revision: 1.0 $
 * $Author: Edgar Escayola Vinagre $
 * $Date: 17-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
		This file contains the global initializations.
		
		It contains the array that has the description of the tasks: 
		function pointers, periods and offsets. Also the software initializations.
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
 * $Log: Init_Tasks.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Init_Tasks.h"
/*============================================================================*/
extern volatile T_UWORD raub_DynConuter[E_TSK_N_OF];
/*============================================================================*/
const S_STAT cas_STAT[E_TSK_N_OF] = {/*	Function Pointer	-	Period 	-	  Offset	*/
		
		{		Task_0,		1	,		0		},
		{		Task_1,		400,		1		},
		
		/*	Add here your new task and a comma before the last one	*/			
		/*	{	Function Pointer	,	Period	,		Offset	}	*/	

};
/*============================================================================*/

void Global_Init(void){

	T_UWORD luw_i;

	for(luw_i = 0; luw_i < E_TSK_N_OF; luw_i++){
		raub_DynConuter[luw_i] = cas_STAT[luw_i].uw_Offset;
	}

}
