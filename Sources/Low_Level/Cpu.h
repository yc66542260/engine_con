/*******************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       CPU.h
\brief      CPU initialization prototypes and definitions
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     Abraham Tezmol
\version    0.4
\date       11/Feb/2008
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

#include "Seabreeze_Emulator.h"

#ifndef _CPU_H
#define _CPU_H


#ifdef __S12XS128
  //for S12XS and S12XE
/*-- Defines -----------------------------------------------------------------*/
/** System configuration definitions */
#define BUS_FREQ        40000000        /** System Bus clock frequency */
#define XTAL_FREQ       8000000         /** Xtal frequency */
#define SYNR_FACTOR     5               /** Target PLL synthesizer multiplying factor */
#define REFFRQ_VALUE    2               /** Configure PLL filter for optimal stability and lock time */
                                        /** REFFRQ = 2; 6MHz < fREF <= 12MHz */
#define VCOFRQ_VALUE    1               /** Configure VCO gain for optimal stability and lock time */
                                        /** VCOFRQ = 1; 48MHz <= fVCO<= 80MHz */

/*-- Macros ------------------------------------------------------------------*/

/** System clock related macros */
/* BUS_FREQ = XTAL_FREQ * (SYNR+1) / ((REFDV+1)(2*POSTDIV))       */
#define POSTDIV_VALUE   (POSTDIV_FACTOR-1)
#define SYNR_VALUE      (SYNR_FACTOR-1)
#define REFDV_VALUE     ((XTAL_FREQ*SYNR_FACTOR/BUS_FREQ)-1) 
 
/** System Clock status definitions */      
#define CLOCK_STATUS_OK                 0
#define CLOCK_STATUS_PLL_LOCK_ERROR     1
#define CLOCK_STATUS_SCM_ERROR          2
#endif
#ifdef __S12P128
  //for S12P device
/*-- Defines -----------------------------------------------------------------*/
/** System configuration definitions */
#define BUS_FREQ        20000000        /** System Bus clock frequency */
#define XTAL_FREQ       8000000         /** Xtal frequency */

/*-- Macros ------------------------------------------------------------------*/

/** System clock related macros */
/* BUS_FREQ = XTAL_FREQ * (SYNR+1) / ((REFDV+1)(2*POSTDIV))       */
#define POSTDIV_VALUE   (POSTDIV_FACTOR-1)
#define SYNR_VALUE      (SYNR_FACTOR-1)
#define REFDV_VALUE     ((XTAL_FREQ*SYNR_FACTOR/BUS_FREQ)-1) 
 
/** System Clock status definitions */      
#define CLOCK_STATUS_OK                 0
#define CLOCK_STATUS_PLL_LOCK_ERROR     1
#define CLOCK_STATUS_SCM_ERROR          2

#endif


/*-- Function Prototypes -----------------------------------------------------*/

/** Othe CPU initalization parameters */
void vfnCpu_init(void);

/** System clock and PLL initialization */
void vfnClock_Init(void);

/** Clock Monitor Interrupt Service Routine */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void Clock_Monitor_Isr(void);
#pragma CODE_SEG DEFAULT

#endif /* _CPU_H */

/*******************************************************************************/