/*******************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       MC33932.h
\brief      Serial Communication Interface functions
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     Omar Hernandez
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

/** Common defines and macros */
#include <hidef.h>
/** Seabreeze Emulator Compilation Options */
#include "Seabreeze_Emulator.h"
/** S12X derivative information */
#include __S12X_DERIVATIVE
/** Variable types and common definitions */
#include "typedefs.h"

#include "Input_Output.h"

  

#ifndef __MC33932
#define __MC33932 1
#define    H1INA_MOTOR_2           PTA_PTA5
#define    H1INA_PIN_DIRECTION     DDRA_DDRA5
#define    H1INB_MOTOR_2           PTA_PTA6
#define    H1INB_PIN_DIRECTION     DDRA_DDRA6
#define    H2INA_MOTOR_1           PTS_PTS3 
#define    H2INA_PIN_DIRECTION     DDRS_DDRS3
#define    H2INB_MOTOR_1           PTS_PTS2 
#define    H2INB_PIN_DIRECTION     DDRS_DDRS2

#define    ENDB2_MOTOR_1           PTP_PTP0 
#define    ENDB2_PIN_DIRECTION     DDRP_DDRP0
#define    ENDB4_MOTOR_2           PTP_PTP1
#define    ENDB4_PIN_DIRECTION     DDRP_DDRP1

#define    SFA_B_MOTOR_1           PTB_PTB2 
#define    SFA_B_PIN_DIRECTION     DDRB_DDRB2
#define    SFB_B_MOTOR_2           PTB_PTB3
#define    SFB_B_PIN_DIRECTION     DDRB_DDRB3


#define    MOTOR_1          0x01
#define    MOTOR_2          0x02
#define    UNKNOWN_DEVICE   0xFF

#define    MOTOR_1_ERROR    0x01
#define    MOTOR_2_ERROR    0x02

enum{
    FORWARD,
    REVERSE,
    FREE_WHEELING_LOW,
    FREE_WHEELING_HIGH,
    UNKNOWN_STATE
};

/*Prototypes*/
void vfInit_MC33932(void);
UINT8 u8M33932_Enable(UINT8 u8Device);
UINT8 u8M33932_Disable(UINT8 u8Device);
UINT8 u8M33932_Set_Device_State(UINT8 u8Device, UINT8 u8State);
UINT8 u8M33932_Read_Device_State(UINT8 u8Device);
UINT8 u8M33932_Read_Status(UINT8 u8Device);
#endif
