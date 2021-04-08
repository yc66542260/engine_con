/*******************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       MC33932.c
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


#include "MC33932.h"


UINT8 u8Device_State_Motor_1 = FREE_WHEELING_LOW;
UINT8 u8Device_State_Motor_2 = FREE_WHEELING_LOW;

/*******************************************************************************/
/**
* \brief    MC33932 Initialization
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    void
* \return   void
*/

void vfInit_MC33932(void) 
{
    // Se necesita que se desactiven las otras funcionaldes de los pines?? 

    /* Set as output IN0, IN1, IN2, IN3 */
    H1INA_PIN_DIRECTION = OUTPUT;
    H1INB_PIN_DIRECTION = OUTPUT;
    H2INA_PIN_DIRECTION = OUTPUT;
    H2INB_PIN_DIRECTION = OUTPUT;
    /* Set as ouput enable pins */
    ENDB2_PIN_DIRECTION = OUTPUT;
    ENDB4_PIN_DIRECTION = OUTPUT;
    /* Set Status Flag as input */
    SFA_B_PIN_DIRECTION = INPUT;
    SFB_B_PIN_DIRECTION = INPUT;
    /* Disable devices by default*/
    ENDB2_MOTOR_1 = DISABLED;
    ENDB4_MOTOR_2 = DISABLED;
    /* Set state as freewheeling low*/
    H2INA_MOTOR_1 = LOW;
    H2INB_MOTOR_1 = LOW;
    H1INA_MOTOR_2 = LOW;
    H1INB_MOTOR_2 = LOW;
    u8Device_State_Motor_1 = FREE_WHEELING_LOW; 
    u8Device_State_Motor_2 = FREE_WHEELING_LOW; 
}

/*******************************************************************************/
/**
* \brief    MC33932 Enable
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    Device to be enabled 
            (MOTOR_1 or MOTOR2)
* \return   Mask Error for the device with error status flag
*/

UINT8 u8M33932_Enable(UINT8 u8Device) 
{ 
    UINT8 u8Return_Code = 0;
           
    if(u8Device & MOTOR_1) 
    {
        ENDB2_MOTOR_1 = ENABLED;
        if(!SFA_B_MOTOR_1) 
        {
            u8Return_Code |= MOTOR_1_ERROR;
        }
    }
    if(u8Device & MOTOR_2) 
    {
      
        ENDB4_MOTOR_2 = ENABLED;
        if(!SFB_B_MOTOR_2) 
        {
            u8Return_Code |= MOTOR_2_ERROR;
        }
    }
    
    return u8Return_Code; 
  
}

/*******************************************************************************/
/**
* \brief    MC33932 Disable
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    Device to be disabled 
            (MOTOR_1 or MOTOR2)
* \return   Mask Error for the device with error status flag
*/                    
UINT8 u8M33932_Disable(UINT8 u8Device) 
{ 
    UINT8 u8Return_Code = 0;
           
    if(u8Device & MOTOR_1) 
    {
        ENDB2_MOTOR_1 = DISABLED;
        if(!SFA_B_MOTOR_1) 
        {
            u8Return_Code |= MOTOR_1_ERROR;
        }
    }
    if(u8Device & MOTOR_2) 
    {
      
        ENDB4_MOTOR_2 = DISABLED;
        if(!SFB_B_MOTOR_2) 
        {
            u8Return_Code |= MOTOR_2_ERROR;
        }
    }
    
    return u8Return_Code; 
  
}


/*******************************************************************************/
/**
* \brief    MC33932 set Device state
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    Device to be disabled, state for the device 
            Device = (MOTOR_1 or MOTOR2)
            State = (FORWARD, REVERSE, FREEW HEELING LOW, FRE WHEELING HIGH)
* \return   Mask Error for the device with error status flag
*/

UINT8 u8M33932_Set_Device_State(UINT8 u8Device, UINT8 u8State) 
{
    UINT8 u8Return_Code = 0;
           
    if(u8Device & MOTOR_1) 
    {
        /* Select Motor 1 State */  
        switch(u8State) 
        {
            case FORWARD:
                H2INA_MOTOR_1 = HIGH;
                H2INB_MOTOR_1 = LOW;
                break;
            case REVERSE:
                H2INA_MOTOR_1 = LOW;
                H2INB_MOTOR_1 = HIGH;            
                break;
            case FREE_WHEELING_LOW:
                H2INA_MOTOR_1 = LOW;
                H2INB_MOTOR_1 = LOW;
                break;
            case FREE_WHEELING_HIGH:
                H2INA_MOTOR_1 = HIGH;
                H2INB_MOTOR_1 = HIGH;
                break;        
        
        }
        /* Check Motor 1 Status Flag */
        if(!SFA_B_MOTOR_1) 
        {
            u8Return_Code |= MOTOR_1_ERROR;
        } 
        u8Device_State_Motor_1 = u8State; 

    }
    
    if(u8Device & MOTOR_2) 
    {
      
        /* Select Motor 2 State */
        switch(u8State) 
        {
            case FORWARD:
                H1INA_MOTOR_2 = HIGH;
                H1INB_MOTOR_2 = LOW;
                break;
            case REVERSE:
                H1INA_MOTOR_2 = LOW;
                H1INB_MOTOR_2 = HIGH;            
                break;
            case FREE_WHEELING_LOW:
                H1INA_MOTOR_2 = LOW;
                H1INB_MOTOR_2 = LOW;
                break;
            case FREE_WHEELING_HIGH:
                H1INA_MOTOR_2 = HIGH;
                H1INB_MOTOR_2 = HIGH;
                break;        
        
        }
        /* Check Motor 2 Status Flag */
        if(!SFB_B_MOTOR_2) 
        {
            u8Return_Code |= MOTOR_2_ERROR;
        }
        u8Device_State_Motor_2 = u8State;
    }
    
    return u8Return_Code; 



}

/*******************************************************************************/
/**
* \brief    MC33932 read Device state
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    Device = (MOTOR_1 or MOTOR2)            
* \return   Device State (FORWARD, REVERSE, FREEW HEELING LOW, FRE WHEELING HIGH) 
*/

UINT8 u8M33932_Read_Device_State(UINT8 u8Device) 
{
    if(u8Device == MOTOR_1) 
    {
    
       return u8Device_State_Motor_1;
    } 
    else 
    {  
       if(u8Device == MOTOR_2) 
          {
             return u8Device_State_Motor_2;
          }
    }
    return UNKNOWN_STATE;
}

/*******************************************************************************/
/**
* \brief    MC33932 read Device status
* \author   Omar Hernandez/Rodrigo Ruiz 
* \param    Device = (MOTOR_1 or MOTOR2)            
* \return   Device State (Enabled . Disabled, unkown device) 
*/


UINT8 u8M33932_Read_Status(UINT8 u8Device) 
{
    UINT8 u8Return_Code = 0;
    
    
    if((u8Device != MOTOR_1) && (u8Device != MOTOR_2) ) 
    {
        return UNKNOWN_DEVICE;
    }
    
    if(u8Device == MOTOR_1) 
    {
        if( (ENDB2_MOTOR_1 == ENABLED)) 
        {
            u8Return_Code = ENABLED;
        } 
        else 
        {        
           u8Return_Code = DISABLED;
        }
    } 

    if(u8Device == MOTOR_2) 
    {
        if( (ENDB4_MOTOR_2 == ENABLED)) 
        {
            u8Return_Code = ENABLED;
        } 
        else 
        {        
           u8Return_Code = DISABLED;
        }
    }

   
    return u8Return_Code;          

}

/*******************************************************************************/
/**
* \brief    MC33932 testing function, (actually MC33926)
* \author   OH 
* \param    Device = (MOTOR_1 or MOTOR2)            
* \return   Void 
*/

void M33932_test(UINT8 u8Device) 
{
    UINT16 u16buffer=0;
    static UINT16  u16counter = 0;
  
    //H2INA_PIN_DIRECTION = OUTPUT;
    //H2INB_PIN_DIRECTION = OUTPUT;
    DDRS_DDRS2 = OUTPUT;
    DDRS_DDRS3 = OUTPUT;


    if (u16counter == 0) 
    {
      (void)u8M33932_Set_Device_State(u8Device, FORWARD);             
    }

    if (u16counter == 50) 
    {
      (void)u8M33932_Set_Device_State(u8Device, FREE_WHEELING_LOW);        
    }

    if (u16counter == 100) 
    {
      (void)u8M33932_Set_Device_State(u8Device, REVERSE);        
    }

    if (u16counter == 150) 
    {
      (void)u8M33932_Set_Device_State(u8Device, FREE_WHEELING_HIGH);        
    }
    u16counter++;    

    if (u16counter == 200) 
    {
      u16counter = 0;        
    }    
}