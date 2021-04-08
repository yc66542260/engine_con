/*******************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       MC33812.c
\brief      Serial Communication Interface functions
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     Omar Hernandez/Rodrigo Ruiz
\ThirdParty NACURI 
\version    0.1
\date       20/03/2008
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

/** SPI definitions and functions prototypes */
//#include "SPI.h"

/** MC33810 definitions and functions */
#include "MC33810.h"


void vfMC33812_Init(void) 
{
    INJIN1_DIN0 = LOW;
    INJIN1_DIN0_PIN_DIR = OUTPUT;
    INJIN2_DIN1 = LOW;
    INJIN2_DIN1_PIN_DIR = OUTPUT;
    BATSWIN_DIN2 = LOW;
    BATSWIN_DIN2_PIN_DIR = OUTPUT;
    VLAMPIN_DIN3 = LOW;
    VLAMPIN_DIN3_PIN_DIR = OUTPUT;
    
    RIN3_GIN0 = LOW;
    RIN3_GIN0_PIN_DIR = OUTPUT;
    O2HIN_GIN1 = LOW;
    O2HIN_GIN1_PIN_DIR = OUTPUT;
    IGNIN1_GIN2 = LOW;
    IGNIN1_GIN2_PIN_DIR  = OUTPUT;
    IGNIN2_GIN3 = LOW;
    IGNIN2_GIN3_PIN_DIR = OUTPUT;
}









/**********************/

