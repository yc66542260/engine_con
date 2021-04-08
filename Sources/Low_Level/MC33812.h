/*******************************************************************************/
/**
Copyright (c) 2009 Freescale Semiconductor
Freescale Confidential Proprietary
\file       MC33812.h
\brief      Analog to Digital Converter Interface functions
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     Luis Olea
\version    0.2
\date       August/ 2009
*/
/*******************************************************************************/
/*                                                                             */
/* All software, source code, included documentation, and any implied know-how */
/* are property of Freescale Semiconductor and therefore considered            */ 
/* CONFIDENTIAL INFORMATION.                                                   */
/*                                                                             */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY. */
/*                                                                             */
/* All Confidential Information remains the property of Freescale Semiconductor*/
/* and will not be copied or reproduced without the express written permission */
/* of the Discloser, except for copies that are absolutely necessary in order  */
/* to fulfill the Purpose.                                                     */
/*                                                                             */
/* Services performed by FREESCALE in this matter are performed AS IS and      */
/* without any warranty. CUSTOMER retains the final decision relative to the   */
/* total design and functionality of the end product.                          */
/*                                                                             */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the    */
/* success of this project.                                                    */
/*                                                                             */
/* FREESCALE disclaims all warranties, express, implied or statutory including,*/
/* but not limited to, implied warranty of merchantability or fitness for a    */
/* particular purpose on any hardware, software ore advise supplied to the     */
/* project by FREESCALE, and or any product resulting from FREESCALE services. */
/*                                                                             */
/* In no event shall FREESCALE be liable for incidental or consequential       */
/* damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE    */
/* harmless against any and all claims demands or actions by anyone on account */
/* of any damage,or injury, whether commercial, contractual, or tortuous,      */
/* rising directly or indirectly as a result of the advise or assistance       */
/* supplied CUSTOMER in connectionwith product, services or goods supplied     */
/* under this Agreement.                                                       */
/*                                                                             */
/*******************************************************************************/
#include <hidef.h>
/** Seabreeze Emulator Compilation Options */
#include "Seabreeze_Emulator.h"
/** S12X derivative information */
#include __S12X_DERIVATIVE
/** Variable types and common definitions */
#include "typedefs.h"

#include "Input_Output.h"
#include "SPI.h"

#ifndef __MC33812  
#define __MC33812 1

#define INJIN1_DIN0            PTT_PTT0
#define INJIN1_DIN0_PIN_DIR    DDRT_DDRT0
#define INJIN2_DIN1            PTT_PTT1
#define INJIN2_DIN1_PIN_DIR    DDRT_DDRT1
#define BATSWIN_DIN2           PTB_PTB4
#define BATSWIN_DIN2_PIN_DIR   DDRB_DDRB4
#define VLAMPIN_DIN3           PTE_PTE5
#define VLAMPIN_DIN3_PIN_DIR   DDRE_DDRE5

#define RIN3_GIN0              PTP_PTP2
#define RIN3_GIN0_PIN_DIR      DDRP_DDRP2
#define O2HIN_GIN1             PTA_PTA0
#define O2HIN_GIN1_PIN_DIR     DDRA_DDRA0
#define IGNIN1_GIN2            PTT_PTT2
#define IGNIN1_GIN2_PIN_DIR    DDRT_DDRT2
#define IGNIN2_GIN3            PTT_PTT3
#define IGNIN2_GIN3_PIN_DIR    DDRT_DDRT3




/** MC33810 initialization */
void vfMC33812_Init(void); 


#endif