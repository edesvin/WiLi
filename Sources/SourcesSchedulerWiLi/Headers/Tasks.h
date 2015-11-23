/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Tasks.h $
 * $Revision: 1.5 $
 * $Author: 	Adrián Zacarías Siete $
 *		Edgar Escayola Vinagre $
 * $Date: 17-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Prototypes of the functions that contain the tasks.
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
/*                     |          WiLi      |           1.5                   */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: Tasks.h  $
  ============================================================================*/
#ifndef TASKS_H_
#define TASKS_H_
/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "stdtypedef.h"
#include "App.h"
/* Exported functions prototypes */
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
extern void Task_0(void);
extern void Task_1(void);
/*Add here the prototype of your new task*/

#endif /* TASKS_H_ */
