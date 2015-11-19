/*============================================================================*/
/*                        I BS SOFTWARE GROUP                                 */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: HAL.h $
 * $Revision: 1.0 $
 * $Author: Adrián Zacarías Siete $
 * $Date: 17-11-2015 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
		This file contains the prototypes of the HAL.
		
		It contains the initialization of clocks, timers, GPIOs and interrupts
		used in the main application.
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
 * $Log: HAL.h  $
  ============================================================================*/
#ifndef HAL_H_
#define HAL_H_
/* Includes */
/*============================================================================*/
#include "MPC5606B.h"
#include "stdtypedef.h"
#include "IntcInterrupts.h"
#include "Kernel.h"
/* Defines */
/*============================================================================*/
#define ValTMR_0 		0x0000F9FF
/* Exported functions prototypes */
/*============================================================================*/
void initModesAndClock(void);
void Init_ISR(void);
void Init_PIT(void);
void Init_PIT_CH0(T_ULONG LDVALOR_0);
void init_GPIO(void);
extern void All_Init(void);

#endif /* HAL_H_ */
