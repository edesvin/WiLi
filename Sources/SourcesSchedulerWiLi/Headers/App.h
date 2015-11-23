/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: App.h $
 * $Revision: 1.0 $
 * $Author: 	Adrián Zacarías Siete $
 *		Edgar Escayola Vinagre $
 * $Date: 20-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Prototypes of all the functions implemented in the tasks used by the 
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
/*                     |        WiLi        |           1.1                   */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: App.h  $
  ============================================================================*/
#ifndef APP_H_
#define APP_H_
/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "stdtypedef.h"
#include "Tasks.h"
/*============================================================================*/
#define L_IDLE 	0
#define L_UP 	1
#define L_DOWN 	2

#define PB_UP		SIU.GPDI[64].B.PDI
#define PB_DOWN 	SIU.GPDI[66].B.PDI
#define PB_AnPi 	SIU.GPDI[67].B.PDI

#define Led_UP		SIU.GPDO[11].B.PDO
#define Led_DOWN	SIU.GPDO[10].B.PDO

#define First_Led	SIU.GPDO[0].B.PDO

#define ON 0
#define OFF 1
/*============================================================================*/
T_SBYTE Func_UP(T_SBYTE lsb_index);		
T_SBYTE Func_DOWN(T_SBYTE lsb_index);	
void Func_IDLE(void);

void StateMachine(void);

void Val_PushB(void);
void Val_PB_UP(void);
void Val_PB_DOWN(void);
void Val_PB_AnPi(void);
void InvalidButtonPress(void);
void NoButtonPress(void);

void Time5segAnpi(void);
void Func_Dir(void);
void Reset_All_Flags(void);
void Reset_Dir_Flags(void);
void Reset_VarBarLeds(void);

void Func_LEDsUpDown(void);

#endif /* APP_H_ */
