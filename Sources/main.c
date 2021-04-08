/*******************************************************************************/
/**
  \mainpage
  
  \n Copyright (c) 2008 Freescale Semiconductor
  \n Freescale Confidential Proprietary
  
  \brief        Small Engine Control System
  \author       Freescale Semiconductor
  \project      BRIC
  \version      0.2
  \date         November/2008   
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

/** Seabreeze Emulator Compilation Options */
#include "Seabreeze_Emulator.h"
/** S12X derivative information */
#include __S12X_DERIVATIVE
#pragma LINK_INFO DERIVATIVE __S12X_LINK_DERIVATIVE

/** Common defines and macros */
#include <hidef.h>
/** Variable types and common definitions */
#include "typedefs.h"
/** ECU configuration function prototypes */
#include "ECU Configuration.h" 
/** Scheduler function prototypes definitions */
#include "Scheduler.h"
/** User management function prototypes definitions */
#include "User Management.h"
/** Data management definitions and function prototypes */
#include "Data Management.h"
/** Engine management definitions and function prototypes */
#include "Engine Management.h"


/*-- Global Variables ---------------------------------------------------------*/
/* Global state machine status for LED flashing control  */
UINT8 gu8Index;

/*-- Main Code ----------------------------------------------------------------*/
void main(void)
{
/* Global Sleep Mode enable flag */
UINT8 gu8SleepModeEnabled;

    /* Initialize auxiliary variables */
    gu8Index = 0;                   
    gu8SleepModeEnabled = 0;
    
    /* Prior Initialization, disable interrupts */
    DISABLE_INTERRUPTS();           
    
    /* Initial configuration for ECU hardware resources */
    vfnECU_Init();
    
    /* Initialization of resources for user's, data and engine management */       
    User_Management_Init();    
    Data_Management_Init();
    Engine_Management_Init(); 
    
    /* Start execution of task scheduler */
    vfnScheduler_Start(); 
              
    /* Initialization complete, enable interrupts */
    ENABLE_INTERRUPTS();            
      
    /*-- While Power-savings mode is NOT requested --*/
    while (gu8SleepModeEnabled == 0)
    {
        /* Execute Multi-thread round robin task scheduler */
        vfnTask_Scheduler();
    }
    
    while(1);
}
/*******************************************************************************/
