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

/** SPI definitions and functions prototypes */
//#include "SPI.h"

/** MC33810 definitions and functions */
#include "MC33810.h"


UINT8 u8transaction_state;
UINT8 u8transaction_type;
UINT16 u16Return_Data;

//Only valid for emulator
#if HARDWARE == (EMULATOR)

void vfMC33810_Init(void) 
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
    u8transaction_state = WRITE_READY;
    u8transaction_type = NOTRANS;    
}


UINT8 u8MC33810_Request(UINT16 u16Input_Buffer) 
{   
    UINT16 u16ReadValue;
    if(u8transaction_state == WRITE_READY) 
    {    
        u8transaction_state = BUSY_WRITING;
        u8transaction_type = REQUEST;
        u16ReadValue = u8SPI_Transfer(u16Input_Buffer); 
    }

    return u8MC33810_Status() ;
}

UINT8 u8MC33810_Command(UINT16 u16Input_Buffer) 
{
    UINT16 u16ReadValue;
    if(u8transaction_state == WRITE_READY) 
    {    
        u8transaction_state = BUSY_WRITING;
        u8transaction_type = COMMAND;
        u16ReadValue = u8SPI_Transfer(u16Input_Buffer); 
    }

    return u8MC33810_Status() ;    

}


UINT8 u8MC33810_Status(void)
{
    UINT16 u16ReadValue;
    if(u8transaction_type == REQUEST) 
    {
        if(u8transaction_state == BUSY_WRITING) 
        {
            if(u8SPI_Transfer(DUMMY_WRITE_DATA) == SPI_COMM_RX_OK) 
            {
                u8transaction_state = BUSY_READING;
                u16ReadValue = u16SPI_Read_Data();
                
            }
        }
        
        if(u8transaction_state == BUSY_READING) 
        {
            if(u8SPI_Transfer(DUMMY_WRITE_DATA) == SPI_COMM_RX_OK) 
            {
                u16Return_Data = u16SPI_Read_Data();
                u8transaction_state = READ_READY;
                u8transaction_type = NOTRANS;
                
            }
                
        }
    }
    
    if(u8transaction_type == COMMAND) 
    {
        if(u8transaction_state == BUSY_WRITING) 
        {
            if(u8SPI_Transfer(DUMMY_WRITE_DATA) == SPI_COMM_RX_OK) 
            {
                u16ReadValue = u16SPI_Read_Data();
                u8transaction_state = WRITE_READY;
                u8transaction_type = NOTRANS;
            }
        }
    
    
    }
    return u8transaction_state;
}

UINT16 u16MC33810_Read_Data(void) 
{
    if(u8transaction_state == READ_READY) 
    {
      u8transaction_state = WRITE_READY;
      return u16Return_Data; 
    }
    return DUMMY_READ_DATA;    
}





/****************/
UINT8 u8MC33810_Read_Answer(UINT16  *pu16Buffer)    //<----------------------
{

    if(u8MC33810_Status() == READ_READY) 
    {
        *pu16Buffer = u16MC33810_Read_Data();
    }
    
    return u8MC33810_Status();

}

UINT8 u8MC33810_Read_Register_Command(UINT16  *u16Buffer, UINT8 u8Address) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request(READ_REGISTER_COMMAD_MASK | (u8Address << 4));     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }
    
    return u8MC33810_Status();    
}


UINT8 u8MC33810_All_Status_Command(UINT16  *u16Buffer) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request(ALL_STATUS_COMMAD_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    
}


UINT8 u8MC33810_SPI_Check_Command(UINT16  *u16Buffer) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request(SPI_CHECK_COMMAD_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    
}


/**************/
 UINT16 u16Mode_Select_Command = 0;
 UINT16 u16LSD_Fault_Command = LSD_FAULT_COMMAND_MASK|0x0800|0x00FF; //default values
 UINT16 u16Driver_ON_OFF_Command = 0;
 UINT16 u16Spark_Command = SPARK_COMMAND_MASK | 0x0800 | 0x000D; // default values
 UINT16 u16End_Spark_Command =  END_SPARK_FILTER_MASK | END_SPARK_FILTER_4US;
 UINT16 u16Dac_Command = DAC_COMMAND_MASK | DAC_MAXI_CURRENT_14A | DAC_OVERLAP_COMPENSATION_35|DAC_NOMI_CURRENT_5_50;
 UINT16 u16Short_Threshold_Voltage_Command = DAC_COMMAND_MASK | DAC_MAXI_CURRENT_14A | DAC_OVERLAP_COMPENSATION_35|DAC_NOMI_CURRENT_5_50;
 UINT16 u16Short_Duration_Timer_Command = 0;
 UINT16 u16File_Operation_Retry_Shutdown = 0;
 UINT16 au16PWMx_Frequency_Dc_Command[NUMBER_OF_PWM_DEVICES];
        
/*********************/
UINT8 u8MC33810_Mode_Select_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {        
        return u8MC33810_Request((u16Data & 0x0FFF) | MODE_SELECT_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}

UINT8 u8MC33810_IGN_GP_MODE_SELECT(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16Mode_Select_Command = (u16Mode_Select_Command & ~MODE_SELECT_IGN_GP_MODE_MASK) | (u16Data & MODE_SELECT_IGN_GP_MODE_MASK);
    return u8MC33810_Mode_Select_Command(u16Buffer, u16Mode_Select_Command);
}

UINT8 u8MC33810_V10_MODE_ENABLE(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16Mode_Select_Command = (u16Mode_Select_Command & ~MODE_SELECT_V10_MODE_MASK) | (u16Data & MODE_SELECT_V10_MODE_MASK);
    return u8MC33810_Mode_Select_Command(u16Buffer, u16Mode_Select_Command);
}

UINT8 u8MC33810_OVR_UNDERVTG(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16Mode_Select_Command = (u16Mode_Select_Command & ~MODE_SELECT_OVER_UNDER_VOLTAGE_MASK) | (u16Data & MODE_SELECT_OVER_UNDER_VOLTAGE_MASK);
    return u8MC33810_Mode_Select_Command(u16Buffer, u16Mode_Select_Command);
}

UINT8 u8MC33810_PWM_ENABLE(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16Mode_Select_Command = (u16Mode_Select_Command & ~MODE_SELECT_PWM_ENABLE_MASK) | (u16Data & MODE_SELECT_PWM_ENABLE_MASK);
    return u8MC33810_Mode_Select_Command(u16Buffer, u16Mode_Select_Command);
}





/*****************/


UINT8 u8MC33810_LSD_Fault_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | LSD_FAULT_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_LSD_Flt_Operation(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16LSD_Fault_Command = (u16LSD_Fault_Command & ~LST_FAULT_LSD_FLT_OPERATION_MASK) | (u16Data & LST_FAULT_LSD_FLT_OPERATION_MASK);
    return u8MC33810_LSD_Fault_Command(u16Buffer, u16LSD_Fault_Command);
}


UINT8 u8MC33810_LSD_ON_Open_Load(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16LSD_Fault_Command = (u16LSD_Fault_Command & ~LST_FAULT_ON_OPEN_LOAD_MASK) | (u16Data & LST_FAULT_ON_OPEN_LOAD_MASK);
    return u8MC33810_LSD_Fault_Command(u16Buffer, u16LSD_Fault_Command);
}

UINT8 u8MC33810_LSD_OFF_Open_Load(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16LSD_Fault_Command = (u16LSD_Fault_Command & ~LST_FAULT_OFF_OPEN_LOAD_MASK) | (u16Data & LST_FAULT_OFF_OPEN_LOAD_MASK);
    return u8MC33810_LSD_Fault_Command(u16Buffer, u16LSD_Fault_Command);
}


/**********************/
UINT8 u8MC33810_Driver_ON_OFF_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | DRIVER_ON_OFF_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Driver_ON_OFF_GPGD(UINT16  *u16Buffer,UINT16 u16Data) /* Ignored in Ignition Mode */
{
    u16Driver_ON_OFF_Command = (u16Driver_ON_OFF_Command & ~DRIVER_ON_OFF_GPGD_MASK) | (u16Data & DRIVER_ON_OFF_GPGD_MASK);
    return u8MC33810_Driver_ON_OFF_Command(u16Buffer, u16Driver_ON_OFF_Command);
}


UINT8 u8MC33810_Driver_ON_OFF_OUTX(UINT16  *u16Buffer,UINT16 u16Data) /* Ignored in Ignition Mode */
{
    u16Driver_ON_OFF_Command = (u16Driver_ON_OFF_Command & ~DRIVER_ON_OFF_OUTX_MASK) | (u16Data & DRIVER_ON_OFF_OUTX_MASK);
    return u8MC33810_Driver_ON_OFF_Command(u16Buffer, u16Driver_ON_OFF_Command);
}                                   



/**********************/
UINT8 u8MC33810_Spark_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | SPARK_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}

UINT8 u8MC33810_Spark_MaxDwell_Timer(UINT16  *u16Buffer,UINT16 u16Data) 
{
    u16Spark_Command = (u16Spark_Command & ~SPARK_MAXDWELL_TIMER_MASK) | (u16Data & SPARK_MAXDWELL_TIMER_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}


UINT8 u8MC33810_Spark_MaxDwell_Enable(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_MAXDWELL_ENABLE_MASK) | (u16Data & SPARK_MAXDWELL_ENABLE_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}


UINT8 u8MC33810_Overlaping_Dwell_Enable(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_OVERLAPING_DWELL_ENABLE_MASK) | (u16Data & SPARK_OVERLAPING_DWELL_ENABLE_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}


UINT8 u8MC33810_Gain_Select(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_GAIN_SELECT_MASK) | (u16Data & SPARK_GAIN_SELECT_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}

UINT8 u8MC33810_Soft_Shutdown_Enable(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_SOFT_SHUTDOWN_MASK) | (u16Data & SPARK_SOFT_SHUTDOWN_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}

UINT8 u8MC33810_Open_Second_Clamp(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_SECOND_CLAMP_MASK) | (u16Data & SPARK_SECOND_CLAMP_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}

UINT8 u8MC33810_Open_Secondary_OSFLT(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_OPEN_SECONDARY_OSFLT_MASK) | (u16Data & SPARK_OPEN_SECONDARY_OSFLT_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}

UINT8 u8MC33810_End_Spark_Threshold(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Spark_Command = (u16Spark_Command & ~SPARK_END_SPARK_THRESHOLD_MASK) | (u16Data & SPARK_END_SPARK_THRESHOLD_MASK);
    return u8MC33810_Spark_Command(u16Buffer, u16Spark_Command);
}

/**********************/

UINT8 u8MC33810_End_Spark_Filter(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | END_SPARK_FILTER_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Set_End_Spark_Filter(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16End_Spark_Command = (u16End_Spark_Command & ~SPARK_END_SPARK_THRESHOLD_MASK) | (u16Data & SPARK_END_SPARK_THRESHOLD_MASK);
    return u8MC33810_End_Spark_Filter(u16Buffer, u16End_Spark_Command);
}


/***/
UINT8 u8MC33810_DAC_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | DAC_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Maximum_DAC_Threshold(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Dac_Command = (u16Dac_Command & ~DAC_MAXI_THRESHOLD_MASK) | (u16Data & DAC_MAXI_THRESHOLD_MASK);
    return u8MC33810_DAC_Command(u16Buffer, u16Dac_Command);
}

UINT8 u8MC33810_Overlap_Setting(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Dac_Command = (u16Dac_Command & ~DAC_OVERLAP_SETTIG_MASK ) | (u16Data & DAC_OVERLAP_SETTIG_MASK);
    return u8MC33810_DAC_Command(u16Buffer, u16Dac_Command);
}

UINT8 u8MC33810_DAC_Nominal_Current_Threshold(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16Dac_Command = (u16Dac_Command & ~DAC_NOMI_THRESHOLD_MASK ) | (u16Data & DAC_NOMI_THRESHOLD_MASK);
    return u8MC33810_DAC_Command(u16Buffer, u16Dac_Command);
}


/**/

UINT8 u8MC33810_Short_Threshold_Voltage_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | GPGD_SHORT_THRESHOLD_VOLTAGE_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Short_Threshold_Voltage(UINT16  *u16Buffer,UINT16 u16Data , UINT16 u16FBX_Select) 
{                                                                       

    if(u16FBX_Select == GPGD_SHORT_THRESHOLD_VOLTAGE_FB3_MASK ) 
    {  
        u16Short_Threshold_Voltage_Command = (u16Short_Threshold_Voltage_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 9);
    }
    if(u16FBX_Select == GPGD_SHORT_THRESHOLD_VOLTAGE_FB2_MASK ) 
    {  
        u16Short_Threshold_Voltage_Command = (u16Short_Threshold_Voltage_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 6);
    }

    if(u16FBX_Select == GPGD_SHORT_THRESHOLD_VOLTAGE_FB1_MASK ) 
    {  
        u16Short_Threshold_Voltage_Command = (u16Short_Threshold_Voltage_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 3);
    }
    if(u16FBX_Select == GPGD_SHORT_THRESHOLD_VOLTAGE_FB0_MASK ) 
    {  
        u16Short_Threshold_Voltage_Command = (u16Short_Threshold_Voltage_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 0);
    }
    
    return u8MC33810_Short_Threshold_Voltage_Command(u16Buffer, u16Short_Threshold_Voltage_Command);
}


/***************/
/**/

UINT8 u8MC33810_Short_Duration_Timer_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | GPGD_SHORT_DURATION_TIMER_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Short_Duration_Timer(UINT16  *u16Buffer,UINT16 u16Data , UINT16 u16FBX_Select) 
{                                                                       

    if(u16FBX_Select == GPGD_SHORT_DURATION_TIMER_FB3_MASK ) 
    {  
        u16Short_Duration_Timer_Command = (u16Short_Duration_Timer_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 9);
    }
    if(u16FBX_Select == GPGD_SHORT_DURATION_TIMER_FB2_MASK ) 
    {  
        u16Short_Duration_Timer_Command = (u16Short_Duration_Timer_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 6);
    }

    if(u16FBX_Select == GPGD_SHORT_DURATION_TIMER_FB1_MASK ) 
    {  
        u16Short_Duration_Timer_Command = (u16Short_Duration_Timer_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 3);
    }
    if(u16FBX_Select == GPGD_SHORT_DURATION_TIMER_FB0_MASK ) 
    {  
        u16Short_Duration_Timer_Command = (u16Short_Duration_Timer_Command & ~u16FBX_Select ) | ((u16Data & 0x000F) << 0);
    }
    
    return u8MC33810_Short_Duration_Timer_Command(u16Buffer, u16Short_Duration_Timer_Command);
}

/**/

UINT8 u8MC33810_Fault_Operation_Select_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | GPGD_FAULT_OPERATION_SELECT_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_Fault_Operation_Retry_Shutdown(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16File_Operation_Retry_Shutdown = (u16File_Operation_Retry_Shutdown & ~GPGD_FAULT_OPERATION_RETRY_SHUTDOWN_MASK ) | (u16Data & GPGD_FAULT_OPERATION_RETRY_SHUTDOWN_MASK);
    return u8MC33810_Fault_Operation_Select_Command(u16Buffer, u16File_Operation_Retry_Shutdown);
}


UINT8 u8MC33810_Fault_Operation_Shutdow_on_MAXI(UINT16  *u16Buffer,UINT16 u16Data) 
{                                                                       
    u16File_Operation_Retry_Shutdown = (u16File_Operation_Retry_Shutdown & ~GPGD_FAULT_OPERATION_SHUTDOWN_ON_MAXI_MASK ) | (u16Data & GPGD_FAULT_OPERATION_SHUTDOWN_ON_MAXI_MASK);
    return u8MC33810_Fault_Operation_Select_Command(u16Buffer, u16File_Operation_Retry_Shutdown);
}



/**/

UINT8 u8MC33810_PWMX_Frequency_DC_Command(UINT16  *u16Buffer,UINT16 u16Data) 
{
    UINT8 u8temporal_status = 0;
    u8temporal_status = u8MC33810_Status(); 
    if(u8temporal_status == WRITE_READY) 
    {
        return u8MC33810_Request((u16Data & 0x0FFF) | PWMX_FREQUENCY_DC_COMMAND_MASK);     
    } 
    if(u8temporal_status == READ_READY) 
    {
        *u16Buffer = u16MC33810_Read_Data();
    }

    return u8MC33810_Status();    

}


UINT8 u8MC33810_PWMX_FREQUENCY(UINT16  *u16Buffer,UINT16 u16PWM_Address, UINT16 u16Data) 
{                                                                       
    au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10] = (au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10] & (~PWMX_ADDRESS_MASK & ~PWMX_FREQUENCY_MASK ) ) 
                                                        | ( u16PWM_Address & PWMX_ADDRESS_MASK )
                                                        | (u16Data & PWMX_FREQUENCY_MASK);
    return u8MC33810_Fault_Operation_Select_Command(u16Buffer, au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10]);
}


UINT8 u8MC33810_PWMX_DUTY_CYCLE(UINT16  *u16Buffer,UINT16 u16PWM_Address, UINT16 u16Data) 
{                                                                       
    au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10] = (au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10] & (~PWMX_ADDRESS_MASK & ~PWMX_FREQUENCY_MASK ) ) 
                                                        | ( u16PWM_Address & PWMX_ADDRESS_MASK )
                                                        | (u16Data & PWMX_FREQUENCY_MASK);
    return u8MC33810_Fault_Operation_Select_Command(u16Buffer, au16PWMx_Frequency_Dc_Command[u16PWM_Address >> 10]);
}



UINT8 M33810_test(void) 
{
    UINT16 u16buffer=0;
    static UINT8  u8what_am_I_doing = 0;
    UINT8 u8dummyread =0;
  //  UINT16 u16buffer2=0;
  
  switch(u8what_am_I_doing) 
  {
    
    case 0:
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_SPI_Check_Command(&u16buffer);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
      break;
    case 1:
        
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
         u8dummyread = u8MC33810_All_Status_Command(&u16buffer);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
      /////////////////////////////////////////////
      break;
    case 2:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_IGN_GP_MODE_SELECT(&u16buffer,  MODE_SELECT_GENERAL_PURPOSE_MODE_GD0
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD1 
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD2
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD3);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
      break;
    case 3:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_05);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }      
            
        
     break;
////////////////////////////////////////////////////////

    case 4:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_V10_MODE_ENABLE(&u16buffer,  MODE_SELECT_V10_MODE_ON);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
    break;
    case 5:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_05);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
  
    case 6:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_OVR_UNDERVTG(&u16buffer,  MODE_SELECT_OVER_UNDER_VOLTAGE_ON);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
    break;
    case 7:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_05);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////  
    case 8:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_PWM_ENABLE(&u16buffer,  MODE_SELECT_PWM3_ENABLE
                                           |MODE_SELECT_PWM2_ENABLE
                                           |MODE_SELECT_PWM1_ENABLE
                                           |MODE_SELECT_PWM0_ENABLE
                                            );
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 9:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_05);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 10:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_LSD_Flt_Operation(&u16buffer,  LST_FAULT_TIMER_ONLY);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 11:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_06);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 12:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_LSD_ON_Open_Load(&u16buffer,  LST_FAULT_OUT3_ON_OPEN_LOAD
                                                               |LST_FAULT_OUT2_ON_OPEN_LOAD
                                                               |LST_FAULT_OUT1_ON_OPEN_LOAD
                                                               |LST_FAULT_OUT0_ON_OPEN_LOAD);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 13:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_06);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 14:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_LSD_OFF_Open_Load(&u16buffer,  LST_FAULT_OUT3_OFF_OPEN_LOAD
                                                               |LST_FAULT_OUT3_OFF_OPEN_LOAD
                                                               |LST_FAULT_OUT3_OFF_OPEN_LOAD
                                                               |LST_FAULT_OUT3_OFF_OPEN_LOAD);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 15:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_06);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////  OH  04/01/08
    case 16:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Driver_ON_OFF_GPGD(&u16buffer,  DRIVER_ON_OFF_GPGD0_ON
                                                               |DRIVER_ON_OFF_GPGD1_ON
                                                               |DRIVER_ON_OFF_GPGD2_ON
                                                               |DRIVER_ON_OFF_GPGD3_ON);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 17:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_07);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 18:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Driver_ON_OFF_OUTX(&u16buffer,  DRIVER_ON_OFF_OUT0_ON
                                                             //  |DRIVER_ON_OFF_OUT1_ON
                                                             //  |DRIVER_ON_OFF_OUT2_ON
                                                               |DRIVER_ON_OFF_OUT3_ON);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 19:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_07);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 20:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Spark_MaxDwell_Timer(&u16buffer, SPARK_MAXDWELL_2MS);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 21:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 22:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Spark_MaxDwell_Enable(&u16buffer, SPARK_MAXDWELL_ENABLE);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 23:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 24:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Overlaping_Dwell_Enable(&u16buffer, SPARK_OVERLAPING_DWELL_DISABLE);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 25:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 26:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Gain_Select(&u16buffer, SPARK_GAIN_SELECT_OF_2);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 27:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 28:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Soft_Shutdown_Enable(&u16buffer, SPARK_SOFT_SHUTDOWN_ENABLE);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 29:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 30:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Open_Second_Clamp(&u16buffer, SPARK_SECOND_CLAMP_DISABLE);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 31:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 32:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Open_Secondary_OSFLT(&u16buffer, SPARK_OPEN_SECONDARY_OSFLT_50US);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 33:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 34:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_End_Spark_Threshold(&u16buffer, SPARK_END_SPARK_THRESHOLD_5_5);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 35:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_08);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 36:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Set_End_Spark_Filter(&u16buffer, END_SPARK_FILTER_4US);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 37:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_05);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 38:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Maximum_DAC_Threshold(&u16buffer, DAC_MAXI_CURRENT_9A);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 39:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_10);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 40:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Overlap_Setting(&u16buffer, DAC_OVERLAP_COMPENSATION_35);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 41:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_10);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 42:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_DAC_Nominal_Current_Threshold(&u16buffer, DAC_NOMI_CURRENT_3_00);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 43:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_10);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 44:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Short_Threshold_Voltage(&u16buffer, GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_2_0V
                                                                     ,GPGD_SHORT_THRESHOLD_VOLTAGE_FB0_MASK);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 45:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_11);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 46:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Short_Duration_Timer(&u16buffer, GPGD_FBX_FOULT_TIMER_SELECT_240US
                                                                     ,GPGD_SHORT_DURATION_TIMER_FB2_MASK);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 47:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_12);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 48:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Fault_Operation_Retry_Shutdown(&u16buffer, GPGD_FAULT_OPERATION_RETRY_GD0);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 49:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_13);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 50:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Fault_Operation_Shutdow_on_MAXI(&u16buffer, GPGD_FAULT_OPERATION_GD3_SHUTDOWN_ENABLE);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 51:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_13);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 52:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_PWMX_FREQUENCY(&u16buffer, PWMX_ADDRESS_MASK, PWM3_ADDRESS);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 53:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_14);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////
    case 54:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_PWMX_DUTY_CYCLE(&u16buffer, PWMX_ADDRESS_MASK, PWM_FREQUENCY_640HZ);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
          }
      
  
      }
     break;
    case 55:
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Read_Register_Command(&u16buffer,INTERNAL_REGISTER_14);
      }else {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
              u8what_am_I_doing++;
           }
      
  
      }      
            
        
  
     break;
/////////////////////////////////////////

 /* if(u8MC33810_SPI_Check_Command(&u16buffer) == READ_READY ) 
  {
  
  }
 */ 
  
  
  
  } /*swtich*/
return u8dummyread;  
}

//UINT8 u8MC33810_Read_Answer(UINT16  *pu16Buffer)    //<----------------------
//UINT8 u8MC33810_Status(void)
//u8MC33810_IGN_GP_MODE_SELECT(UINT16  *u16Buffer,UINT16 u16Data)
//u8MC33810_V10_MODE_ENABLE(UINT16  *u16Buffer,UINT16 u16Data)
//UINT8 u8MC33810_OVR_UNDERVTG(UINT16  *u16Buffer,UINT16 u16Data) 
//UINT8 u8MC33810_PWM_ENABLE(UINT16  *u16Buffer,UINT16 u16Data) 



//UINT8 u8MC33810_LSD_Flt_Operation(UINT16  *u16Buffer,UINT16 u16Data) 


//UINT8 u8MC33810_LSD_ON_Open_Load(UINT16  *u16Buffer,UINT16 u16Data) 

//UINT8 u8MC33810_LSD_OFF_Open_Load(UINT16  *u16Buffer,UINT16 u16Data) 

/*
void vfMC33810_Init_Test(void) 
{
    UINT16 u16buffer=0;
    UINT8  u8dummyread =0;
    
     
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_IGN_GP_MODE_SELECT(&u16buffer,  MODE_SELECT_GENERAL_PURPOSE_MODE_GD0
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD1 
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD2
                                                   | MODE_SELECT_GENERAL_PURPOSE_MODE_GD3);
      }
      else
      {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
          }
      
  
      }

        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_PWM_ENABLE(&u16buffer, MODE_SELECT_PWM_NONE);
      }
      else
      {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
          }
      
  
      }
    

        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Driver_ON_OFF_GPGD(&u16buffer, DRIVER_ON_OFF_GPGD_OFF_ALL);
      }
      else 
      {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
          }
      
  
      }

        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_Driver_ON_OFF_OUTX(&u16buffer, DRIVER_ON_OFF_OUT_OFF_ALL);
      }
      else 
      {
  
          if(u8MC33810_Status() == READ_READY)
          {                                                                                          
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
          }
      
  
      }
        
        u16buffer=0;
      if(u8MC33810_Status() == WRITE_READY) 
      {  
          u8dummyread = u8MC33810_SPI_Check_Command(&u16buffer);
      }
      else 
      {
  
          if(u8MC33810_Status() == READ_READY)
          {
              u8dummyread = u8MC33810_Read_Answer(&u16buffer);
          }
      
  
      }       
}

*/
#endif