/*******************************************************************************/
/**
Copyright (c) 2009 Freescale Semiconductor
Freescale Confidential Proprietary
\file       MC33810.h
\brief      Analog to Digital Converter Interface functions
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     
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

#ifndef __MC33810  
#define __MC33810 1

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


#define DUMMY_WRITE_DATA 0x0000
#define DUMMY_READ_DATA 0x0000
enum{
WRITE_READY,
BUSY_WRITING,
BUSY_READING,
READ_READY
};

enum{
NOTRANS,
REQUEST,
COMMAND,
};

#define NUMBER_OF_PWM_DEVICES   0x04

#define READ_REGISTER_COMMAD_MASK 0x0A00

#define INTERNAL_REGISTER_00   0x00
#define INTERNAL_REGISTER_01   0x01
#define INTERNAL_REGISTER_02   0x02
#define INTERNAL_REGISTER_03   0x03
#define INTERNAL_REGISTER_04   0x04
#define INTERNAL_REGISTER_05   0x05
#define INTERNAL_REGISTER_06   0x06
#define INTERNAL_REGISTER_07   0x07
#define INTERNAL_REGISTER_08   0x08
#define INTERNAL_REGISTER_09   0x09
#define INTERNAL_REGISTER_10   0x0A
#define INTERNAL_REGISTER_11   0x0B
#define INTERNAL_REGISTER_12   0x0C
#define INTERNAL_REGISTER_13   0x0D
#define INTERNAL_REGISTER_14   0x0E
#define INTERNAL_REGISTER_15   0x0F


#define ALL_STATUS_COMMAD_MASK    0x0A00
#define SPI_CHECK_COMMAD_MASK     0x0F00


/*****************************************/
#define MODE_SELECT_COMMAND_MASK  0x1000
/* IGNITON/GPGD GDx = 0 Igniton mode
                GDx = 1 General Purpose */
#define MODE_SELECT_IGN_GP_MODE_MASK          0x0F00       
#define MODE_SELECT_GENERAL_PURPOSE_MODE_NONE 0x0000
#define MODE_SELECT_GENERAL_PURPOSE_MODE_GD0  0x0100
#define MODE_SELECT_GENERAL_PURPOSE_MODE_GD1  0x0200
#define MODE_SELECT_GENERAL_PURPOSE_MODE_GD2  0x0400
#define MODE_SELECT_GENERAL_PURPOSE_MODE_GD3  0x0800

#define MODE_SELECT_V10_MODE_MASK      0x0080
#define MODE_SELECT_V10_MODE_ON        0x0080
#define MODE_SELECT_V10_MODE_OFF       0x0000

/* OVR/UnderVtg = 1 all outputs remain OFF when Vpwr returns to normal
                = 0 all outputs return their previous state */
#define MODE_SELECT_OVER_UNDER_VOLTAGE_MASK   0x0040                
#define MODE_SELECT_OVER_UNDER_VOLTAGE_ON     0x0040
#define MODE_SELECT_OVER_UNDER_VOLTAGE_OFF    0x0000

/* PWMx = 1 pwm enabled
   PWMx = 0 pwm disabled 
   For PWM function enable GDx as General Purpose , frequency and duty cycle */
#define MODE_SELECT_PWM_ENABLE_MASK           0x000F
#define MODE_SELECT_PWM_NONE                  0x0000
#define MODE_SELECT_PWM3_ENABLE               0x0008
#define MODE_SELECT_PWM2_ENABLE               0x0004
#define MODE_SELECT_PWM1_ENABLE               0x0002
#define MODE_SELECT_PWM0_ENABLE               0x0001

/************************************************/

#define LSD_FAULT_COMMAND_MASK            0x2000

/* Determines the type os short circuit protection bit11=Shut down retry
                                                   bit10=Timer Limit
                                                   bit9=Fault Timer*/
#define LST_FAULT_LSD_FLT_OPERATION_MASK  0x0E00
#define LST_FAULT_TIMER_ONLY              0x0800 /* Timer only, output will retry
                                                    on OUT0-OUT3 = 60 us ON, ~10ms OFF */
#define LST_FAULT_TEMP_ONLY               0x0C00 /* Temperature limit only. Output will 
                                                    retry on temperature hysteresis */
#define LST_FAULT_TIMER_TEMP              0x0E00 /* Temperature limit and Timer. 
                                                    OUT0-OUT3 = 60 us ON, ~10ms OFF*/
#define LST_FALT_TIMER_ONLY_NO_RETRY      0x0000 /* Timer only, Output will not retry on period
                                                    OUT0-OUT3 = 60 us ON,OFF*/                                        
#define LST_FAULT_TEMP_ONLY_NO_RETRY      0x0400 /* Temperature limit only. Output will not
                                                    retry on temperature hysteresis */
#define LST_FAULT_TIMER_TEMP_NO_RETRY     0x0E00 /* Temperature limit and Timer.No retry on period 
                                                    OUT0-OUT3 = 60 us ON,OFF*/
/* ON Open Load pull-down current source enable bits 
                0 = disabled    1 = enabled */

#define LST_FAULT_ON_OPEN_LOAD_MASK       0x00F0
#define LST_FAULT_NONE_ON_OPEN_LOAD       0x0000
#define LST_FAULT_OUT3_ON_OPEN_LOAD       0x0080
#define LST_FAULT_OUT2_ON_OPEN_LOAD       0x0040
#define LST_FAULT_OUT1_ON_OPEN_LOAD       0x0020
#define LST_FAULT_OUT0_ON_OPEN_LOAD       0x0010

/* Off Open Load pull-down current source enable bits 
                 0 = disabled    1 = enabled */
#define LST_FAULT_OFF_OPEN_LOAD_MASK       0x000F
#define LST_FAULT_NONE_OFF_OPEN_LOAD       0x0000
#define LST_FAULT_OUT3_OFF_OPEN_LOAD       0x0080
#define LST_FAULT_OUT2_OFF_OPEN_LOAD       0x0040
#define LST_FAULT_OUT1_OFF_OPEN_LOAD       0x0020
#define LST_FAULT_OUT0_OFF_OPEN_LOAD       0x0010

/************************************************/

#define DRIVER_ON_OFF_COMMAND_MASK            0x3000

#define DRIVER_ON_OFF_GPGD_MASK               0x00F0
#define DRIVER_ON_OFF_GPGD_OFF_ALL            0x0000
#define DRIVER_ON_OFF_GPGD0_ON                0x0010
#define DRIVER_ON_OFF_GPGD1_ON                0x0020
#define DRIVER_ON_OFF_GPGD2_ON                0x0040
#define DRIVER_ON_OFF_GPGD3_ON                0x0080

#define DRIVER_ON_OFF_OUTX_MASK               0x000F
#define DRIVER_ON_OFF_OUT_OFF_ALL             0x0000
#define DRIVER_ON_OFF_OUT0_ON                 0x0001
#define DRIVER_ON_OFF_OUT1_ON                 0x0002
#define DRIVER_ON_OFF_OUT2_ON                 0x0004
#define DRIVER_ON_OFF_OUT3_ON                 0x0008

/***********************/
#define SPARK_COMMAND_MASK            0x3000

/*
Max Dwell timer:
                000 = 2ms
                001 = 4ms
                010 = 8ms
                011 = 16ms
                100 = 32ms (default)
                101,110,111 = 64ms
*/
#define SPARK_MAXDWELL_TIMER_MASK     0x0E00
#define SPARK_MAXDWELL_2MS            0x0000
#define SPARK_MAXDWELL_4MS            0x0200
#define SPARK_MAXDWELL_8MS            0x0400
#define SPARK_MAXDWELL_16MS           0x0600
#define SPARK_MAXDWELL_32MS           0x0800
#define SPARK_MAXDWELL_64MS           0x0A00

/* */
#define SPARK_MAXDWELL_ENABLE_MASK    0x0100
#define SPARK_MAXDWELL_DISABLE        0x0000
#define SPARK_MAXDWELL_ENABLE         0x0100

/* */
#define SPARK_OVERLAPING_DWELL_ENABLE_MASK    0x0080
#define SPARK_OVERLAPING_DWELL_DISABLE        0x0000
#define SPARK_OVERLAPING_DWELL_ENABLE         0x0080

/*
 1 (gain of 2) when using a 20m current sense resistor
 0 (gain of 1) when using a 40m current sense resistor (default)
 */
#define SPARK_GAIN_SELECT_MASK                0x0040
#define SPARK_GAIN_SELECT_OF_1                0x0000
#define SPARK_GAIN_SELECT_OF_2                0x0040

/* */
#define SPARK_SOFT_SHUTDOWN_MASK              0x0020
#define SPARK_SOFT_SHUTDOWN_DISABLE           0x0000
#define SPARK_SOFT_SHUTDOWN_ENABLE            0x0020

#define SPARK_SECOND_CLAMP_MASK               0x0010
#define SPARK_SECOND_CLAMP_DISABLE            0x0000
#define SPARK_SECOND_CLAMP_ENABLE             0x0010


#define SPARK_OPEN_SECONDARY_OSFLT_MASK       0x000C
#define SPARK_OPEN_SECONDARY_OSFLT_10US       0x0000
#define SPARK_OPEN_SECONDARY_OSFLT_20US       0x0004
#define SPARK_OPEN_SECONDARY_OSFLT_50US       0x0008
#define SPARK_OPEN_SECONDARY_OSFLT_100US      0x000C /*default*/


/*
End Spark Threshold:
                    00 = VPWR + 2.75
                    01 = VPWR + 5.5
                    10 = VPWR + 8.2
                    11 = VPWR + 11.0
*/
#define SPARK_END_SPARK_THRESHOLD_MASK       0x0003
#define SPARK_END_SPARK_THRESHOLD_2_75       0x0000
#define SPARK_END_SPARK_THRESHOLD_5_5        0x0001 /*default*/
#define SPARK_END_SPARK_THRESHOLD_8_2        0x0002
#define SPARK_END_SPARK_THRESHOLD_11_0       0x0003 


/*****************/
#define END_SPARK_FILTER_MASK                0x5000    
#define END_SPARK_FILTER_0US                 0x0000
#define END_SPARK_FILTER_4US                 0x0001
#define END_SPARK_FILTER_16US                0x0002
#define END_SPARK_FILTER_32US                0x0003


#define DAC_COMMAND_MASK                0x6000
#define DAC_MAXI_THRESHOLD_MASK         0x0F00
#define DAC_MAXI_CURRENT_6A             0x0000
#define DAC_MAXI_CURRENT_7A             0x0100
#define DAC_MAXI_CURRENT_8A             0x0200
#define DAC_MAXI_CURRENT_9A             0x0300
#define DAC_MAXI_CURRENT_10A            0x0400
#define DAC_MAXI_CURRENT_11A            0x0500
#define DAC_MAXI_CURRENT_12A            0x0600
#define DAC_MAXI_CURRENT_13A            0x0700
#define DAC_MAXI_CURRENT_14A            0x0800
#define DAC_MAXI_CURRENT_15A            0x0900
#define DAC_MAXI_CURRENT_16A            0x0A00
#define DAC_MAXI_CURRENT_17A            0x0B00
#define DAC_MAXI_CURRENT_18A            0x0C00
#define DAC_MAXI_CURRENT_19A            0x0D00
#define DAC_MAXI_CURRENT_20A            0x0E00
#define DAC_MAXI_CURRENT_21A            0x0F00


/*
000 = 0%
001 = 7%
010 = 15%
011 = 24%
100 = 35%
101 = 47%
110 = 63%
111 = 80%
*/
#define DAC_OVERLAP_SETTIG_MASK         0x00E0 
#define DAC_OVERLAP_COMPENSATION_0      0x0000
#define DAC_OVERLAP_COMPENSATION_7      0x0020
#define DAC_OVERLAP_COMPENSATION_15     0x0040
#define DAC_OVERLAP_COMPENSATION_24     0x0060
#define DAC_OVERLAP_COMPENSATION_35     0x0080 //default
#define DAC_OVERLAP_COMPENSATION_47     0x00A0
#define DAC_OVERLAP_COMPENSATION_63     0x00C0
#define DAC_OVERLAP_COMPENSATION_80     0x00E0


#define DAC_NOMI_THRESHOLD_MASK         0x001F
#define DAC_NOMI_CURRENT_3_00           0x0000
#define DAC_NOMI_CURRENT_3_25           0x0001
#define DAC_NOMI_CURRENT_3_50           0x0002
#define DAC_NOMI_CURRENT_3_75           0x0003
#define DAC_NOMI_CURRENT_4_00           0x0004
#define DAC_NOMI_CURRENT_4_25           0x0005
#define DAC_NOMI_CURRENT_4_50           0x0006
#define DAC_NOMI_CURRENT_4_75           0x0007
#define DAC_NOMI_CURRENT_5_00           0x0008
#define DAC_NOMI_CURRENT_5_25           0x0009
#define DAC_NOMI_CURRENT_5_50           0x000A
#define DAC_NOMI_CURRENT_5_75           0x000B
#define DAC_NOMI_CURRENT_6_00           0x000C
#define DAC_NOMI_CURRENT_6_25           0x000D
#define DAC_NOMI_CURRENT_6_50           0x000E
#define DAC_NOMI_CURRENT_6_75           0x000F
#define DAC_NOMI_CURRENT_7_00           0x0010
#define DAC_NOMI_CURRENT_7_25           0x0011
#define DAC_NOMI_CURRENT_7_50           0x0012
#define DAC_NOMI_CURRENT_7_75           0x0013
#define DAC_NOMI_CURRENT_8_00           0x0014
#define DAC_NOMI_CURRENT_8_25           0x0015
#define DAC_NOMI_CURRENT_8_50           0x0016
#define DAC_NOMI_CURRENT_8_75           0x0017
#define DAC_NOMI_CURRENT_9_00           0x0018
#define DAC_NOMI_CURRENT_9_25           0x0019
#define DAC_NOMI_CURRENT_9_50           0x001A
#define DAC_NOMI_CURRENT_9_75           0x001B
#define DAC_NOMI_CURRENT_10_00          0x001C
#define DAC_NOMI_CURRENT_10_25          0x001D
#define DAC_NOMI_CURRENT_10_50          0x001E
#define DAC_NOMI_CURRENT_10_75          0x001F



/************************
FBx Fault Threshold Select
    000 = 0.5V
    001 = 1.0V
    010 = 1.5V
    011 = 2.0V (default)
    100 = 2.5V
    101 = 3.0V
    110,111 = No change
*/
#define GPGD_SHORT_THRESHOLD_VOLTAGE_COMMAND_MASK         0x7000
#define GPGD_SHORT_THRESHOLD_VOLTAGE_FB3_MASK             0x0E00
#define GPGD_SHORT_THRESHOLD_VOLTAGE_FB2_MASK             0x01C0
#define GPGD_SHORT_THRESHOLD_VOLTAGE_FB1_MASK             0x0068
#define GPGD_SHORT_THRESHOLD_VOLTAGE_FB0_MASK             0x0007

#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_0_5V               0x0000 
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_1_0V               0x0001   
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_1_5V               0x0002
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_2_0V               0x0003  /*Default*/     
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_2_5V               0x0004
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_3_0V               0x0005
#define GPGD_VDS_FBX_FOULT_THRESHOLD_SELECT_NO_CHAGE           0x0007
      

/************************
FBx Short fault timer select
    000 = 30us
    001 = 60us
    010 = 120us
    011 = 240us
    100 = 480us
    101 = 960us
    110,111 = No change
*/

#define GPGD_SHORT_DURATION_TIMER_COMMAND_MASK         0x8000
#define GPGD_SHORT_DURATION_TIMER_FB3_MASK             0x0E00
#define GPGD_SHORT_DURATION_TIMER_FB2_MASK             0x01C0
#define GPGD_SHORT_DURATION_TIMER_FB1_MASK             0x0068
#define GPGD_SHORT_DURATION_TIMER_FB0_MASK             0x0007

#define GPGD_FBX_FOULT_TIMER_SELECT_30US               0x0000 
#define GPGD_FBX_FOULT_TIMER_SELECT_60US               0x0001   
#define GPGD_FBX_FOULT_TIMER_SELECT_120US              0x0002
#define GPGD_FBX_FOULT_TIMER_SELECT_240US              0x0003  /*Default*/     
#define GPGD_FBX_FOULT_TIMER_SELECT_480US              0x0004
#define GPGD_FBX_FOULT_TIMER_SELECT_920US              0x0005
#define GPGD_FBX_FOULT_TIMER_SELECT_NO_CHAGE           0x0007



/* On Fault Operation
   GDx = 1 retry
   GDx = 0 shutdown*/
#define GPGD_FAULT_OPERATION_SELECT_COMMAND_MASK         0x9000
#define GPGD_FAULT_OPERATION_RETRY_SHUTDOWN_MASK         0x0F00

#define GPGD_FAULT_OPERATION_RETRY_GD3                   0x0800
#define GPGD_FAULT_OPERATION_SHUDOWN_GD3                 0x0000
#define GPGD_FAULT_OPERATION_RETRY_GD2                   0x0400
#define GPGD_FAULT_OPERATION_SHUDOWN_GD2                 0x0000
#define GPGD_FAULT_OPERATION_RETRY_GD1                   0x0200
#define GPGD_FAULT_OPERATION_SHUDOWN_GD1                 0x0000
#define GPGD_FAULT_OPERATION_RETRY_GD0                   0x0100
#define GPGD_FAULT_OPERATION_SHUDOWN_GD0                 0x0000

#define GPGD_FAULT_OPERATION_SHUTDOWN_ON_MAXI_MASK       0x000F

#define GPGD_FAULT_OPERATION_GD3_SHUTDOWN_ENABLE         0x0008
#define GPGD_FAULT_OPERATION_GD3_SHUTDOWN_DISABLE        0x0000
#define GPGD_FAULT_OPERATION_GD2_SHUTDOWN_ENABLE         0x0004
#define GPGD_FAULT_OPERATION_GD2_SHUTDOWN_DISABLE        0x0000
#define GPGD_FAULT_OPERATION_GD1_SHUTDOWN_ENABLE         0x0002
#define GPGD_FAULT_OPERATION_GD1_SHUTDOWN_DISABLE        0x0000
#define GPGD_FAULT_OPERATION_GD0_SHUTDOWN_ENABLE         0x0001
#define GPGD_FAULT_OPERATION_GD0_SHUTDOWN_DISABLE        0x0000


#define PWMX_FREQUENCY_DC_COMMAND_MASK                   0xA000
#define PWMX_ADDRESS_MASK                                0x0C00

#define PWM3_ADDRESS                                     0x0C00
#define PWM2_ADDRESS                                     0x0800
#define PWM1_ADDRESS                                     0x0400
#define PWM0_ADDRESS                                     0x0000

/*
PWMx Frequency :
                000 = 10Hz (default)
                001 = 20Hz 
                010 = 40Hz
                011 = 80Hz
                100 = 160Hz
                101 = 320Hz
                110 = 640Hz
                111 = 1.28Khz
                
*/
#define PWMX_FREQUENCY_MASK                              0x0380
#define PWM_FREQUENCY_10HZ                               0x0000
#define PWM_FREQUENCY_20HZ                               0x0080
#define PWM_FREQUENCY_40HZ                               0x0100                                                              
#define PWM_FREQUENCY_80HZ                               0x0180
#define PWM_FREQUENCY_160HZ                              0x0200
#define PWM_FREQUENCY_320HZ                              0x0280
#define PWM_FREQUENCY_640HZ                              0x0300
#define PWM_FREQUENCY_1_28HZ                             0x0380


//Only valid for Emulator
#if HARDWARE == (EMULATOR)

UINT8 u8MC33810_Status(void);
UINT8 M33810_test(void);

/** MC33810 initialization */
void vfMC33810_Init(void); 

#endif
#endif