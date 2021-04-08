/*******************************************************************************/
/**
Copyright (c) 2009 Freescale Semiconductor
Freescale Confidential Proprietary
\file       ADC.c
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

/** Seabreeze Emulator Compilation Options */
#include "Seabreeze_Emulator.h"
/** S12X derivative information */
#include __S12X_DERIVATIVE
/* ADC routines prototypes */ 
#include "ADC.h"
/** Variable types and common definitions */
#include "typedefs.h"
/** IO definitions */
#include "GPIO_macros.h"
/** Application definitions */
#include "Application Definitions.h"


/*******************************************************************************/
/* Global ADC status */
UINT8 u8ADC_Status;

/* Analog channels data array */
sAnalogDatachannel_t au16AnalogSamples[ADC_CHANNELS_TO_SAMPLE];
/* ADC conversion result average */
UINT16 au16AnalogAverage[ADC_CHANNELS_TO_SAMPLE];



/* Array of analog signals to monitor */
const UINT8 signals_array[] = {
    //if VBAT is defined in application definitions, add to array
    #ifdef VBAT
    0,
    #endif
    //if TPS is defined in application definitions, add to array
    #ifdef TPS
    1,
    #endif
    //if ETEMP is defined in application definitions, add to array
    #ifdef ETEMP
    2,
    #endif
    //if MAP is defined in application definitions, add to array
    #ifdef MAP
    3,
    #endif
    //if O2 is defined in application definitions, add to array
    #ifdef O2
    4,
    #endif
    //if ATEMP is defined in application definitions, add to array
    #ifdef ATEMP
    5,
    #endif
    //if MAF is defined in application definitions, add to array
    #ifdef MAF
    6,
    #endif
    //if BAP is defined in application definitions, add to array
    #ifdef BAP
    7
    #endif
};
/* Define the length of signals_array[] */
#ifdef VBAT
    #define E1 1
#else
    #define E1 0
#endif
#ifdef TPS
    #define E2 1
#else
    #define E2 0
#endif
#ifdef ETEMP
    #define E3 1
#else
    #define E3 0
#endif
#ifdef MAP
    #define E4 1
#else
    #define E4 0
#endif
#ifdef O2
    #define E5 1
#else
    #define E5 0
#endif
#ifdef ATEMP
    #define E6 1
#else
    #define E6 0
#endif
#ifdef MAF
    #define E7 1
#else
    #define E7 0
#endif
#ifdef BAP
    #define E8 1
#else
    #define E8 0
#endif
#define signals_array_length E1+E2+E3+E4+E5+E6+E7+E8

/* Initialize the pointer of the array */
UINT8 signals_pointer = 0;

//Analog monitoring functions
#ifdef VBAT
extern void vfnVBAT_Monitoring(UINT8);        
#endif                
        
#ifdef TPS
extern void vfnTPS_Monitoring(UINT8);
#endif
        
#ifdef ETEMP
extern void vfnETEMP_Monitoring(UINT8);
#endif
        
#ifdef MAP
extern void vfnMAP_Monitoring(UINT8);
#endif

#ifdef O2 
extern void vfnO2_Monitoring(UINT8);
#endif                                                                
        
#ifdef ATEMP
extern void vfnATEMP_Monitoring(UINT8);
#endif                
        
#ifdef MAF
extern void vfnMAF_Monitoring(UINT8);
#endif
        
#ifdef BAP
extern void vfnBAP_Monitoring(UINT8);
#endif


#if HARDWARE == REFERENCE
//Raw buffer of ADC results
  #ifdef Analog_Data_8
    unsigned char RAW_ADC[16]{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  #else {
    unsigned int RAW_ADC[16]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};         
  #endif
#endif

UINT16 samples_array[signals_array_length];


/*******************************************************************************/
/**
* \brief    ADC initialization
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void vfnADC_Init(void)
{
    UINT8  u8var1;
    UINT8  u8var2;
    
    /* Analog Inputs Initialization is performed at "vfnInputs_Outputs_Init()" function */

    /*Initialize buffers that store conversion results */
    for (u8var1=0; u8var1<ADC_CHANNELS_TO_SAMPLE; u8var1++)
    {
        for (u8var2=0; u8var2<ADC_SAMPLE_BUFFER_SIZE; u8var2++)
        {
            au16AnalogSamples[u8var1].u16Sample[u8var2] = 0; 
        }
        au16AnalogAverage[u8var1] = 0;
    } 

        
    /* Select 8 bit resolution */
    #ifdef  Analog_Data_8
    #if HARDWARE == REFERENCE
      ATDCTL1_SRES = 0x00;
    #else  
      ATD0CTL1_SRES = 0x00;
    #endif
    #endif    
    /* Select 10 bit resolution */
    #ifdef  Analog_Data_10   
    #if HARDWARE == REFERENCE
      ATDCTL1_SRES = 0x01;
    #else
      ATD0CTL1_SRES = 0x01;
    #endif
    #endif
    /* Select 12 bit resolution */
    #ifdef  Analog_Data_12   
    #if HARDWARE == REFERENCE
      ATDCTL1_SRES = 0x02;
    #else
      ATD0CTL1_SRES = 0x02; 
    #endif   
    #endif
          
    /* Discharge capacitor before sampling */
    #if HARDWARE == REFERENCE
      ATDCTL1_SMP_DIS = 1;       
    #else
      ATD0CTL1_SMP_DIS = 1;       
    #endif
    /* A read access to the result register will cause the associated CCF[n] flag to clear automatically. */
    //ATD0CTL2_AFFC = 1;      
    
    /* Disable external triggering */
    #if HARDWARE == REFERENCE
      ATDCTL2_ETRIGE = 0;    
    #else
      ATD0CTL2_ETRIGE = 0;    
    #endif
    /* Conversion result right justified */
    #if HARDWARE == REFERENCE
      ATDCTL3_DJM = 1;       
    #else
      ATD0CTL3_DJM = 1;       
    #endif
    /* 10 (all channels)conversions per sequence  */
    #if HARDWARE == REFERENCE
      ATDCTL3_S8C = 1;       
      ATDCTL3_S4C = 1;
      ATDCTL3_S2C = 1;
      ATDCTL3_S1C = 1;
    #else
      ATD0CTL3_S8C = 1;       
      ATD0CTL3_S4C = 1;
      ATD0CTL3_S2C = 1;
      ATD0CTL3_S1C = 1;
    #endif
    
    #if HARDWARE == REFERENCE
    //Continuously convert and sample with software for latest value. S12P solution  
    ATDCTL5_SCAN = 1;      /* Scan all active channels sequence */
    ATDCTL5_MULT = 1;      /* Sample multiple channels */ 
    /*Wrap around results after channel 9.        */
    ATDCTL0 = ATDCTL0 & 0x0F;
    /* Enable ADC freeze in debug mode = finish current conversion, then freeze */
    ATDCTL3_FRZ = 2;
    /* Select sample time = 10 ATD clock cycles */
    ATDCTL4_SMP = 3;
    /* Define ADC clock frequency = 4MHz */
    ATDCTL4_PRS = ADC_PRESCALER_VALUE;
    
    /* Clear Sequence Complete flag */
    //ATD0STAT0_SCF = 1;
    
    /* Enable conversion complete interrupt */
    ATDCTL2_ASCIE = 1;
    //INT_CFADDR      = 0xD0;     /* with vectors from 0xFFD0 to 0xFFDE */
    //INT_CFDATA1     = 2;        /* Periodic Interrupt Timer, priority 6 */    

    //Start the ADC collection data          
    //vfnADC_Start_Conversion();
    //Start data collection on channel 0 to allign with RAW_ADC buffer
    //Clear lower nibble to select channel 0
    ATDCTL5 = ATDCTL5 & 0xF0;
    //ADC should be running
        
    #else if HARDWARE ==(EMULATOR | PROTOTYPE)
    //S12XE and S12XS us PIT as a time based trigger 
    //Use PIT to trigger A/D conversion and do single conversion
    ATD0CTL5_SCAN = 0;      /* Single conversion sequence */
    ATD0CTL5_MULT = 0;      /* Sample one channel */ 
    /* Enable ADC freeze in debug mode = finish current conversion, then freeze */
    ATD0CTL3_FRZ = 2;
    /* Select sample time = 4 ATD clock cycles */
    ATD0CTL4_SMP = 0;
    /* Define ADC clock frequency = 4MHz */
    ATD0CTL4_PRS = ADC_PRESCALER_VALUE;
    
    /* Clear Sequence Complete flag */
    //ATD0STAT0_SCF = 1;
    
    /* Enable conversion complete interrupt */
    ATD0CTL2_ASCIE = 1;
    INT_CFADDR      = 0xD0;     /* with vectors from 0xFFD0 to 0xFFDE */
    INT_CFDATA1     = 2;        /* Periodic Interrupt Timer, priority 6 */    
        
    #endif
}

/*******************************************************************************/
/**
* \brief    Read ADC data from conversion sequence and store it into data buffers \n
            Additionally, compute an average for each channel.
* \author   Abraham Tezmol
* \param    void
* \return   void
*/

void vfnADC_Read_Channels(void)
{ 
    static UINT8 u8BufferStoreIndex = 0;
    
  #if HARDWARE == (EMULATOR | PROTOTYPE)
    /* Read analog samples if conversion process is completed */
    if (ATD0STAT0_SCF == 1)
    {
        /* Read all converted channels */
        au16AnalogSamples[0].u16Sample[u8BufferStoreIndex] = ATD0DR0;
        au16AnalogSamples[1].u16Sample[u8BufferStoreIndex] = ATD0DR1;
        au16AnalogSamples[2].u16Sample[u8BufferStoreIndex] = ATD0DR2;
        au16AnalogSamples[3].u16Sample[u8BufferStoreIndex] = ATD0DR3;
        au16AnalogSamples[4].u16Sample[u8BufferStoreIndex] = ATD0DR4;
        au16AnalogSamples[5].u16Sample[u8BufferStoreIndex] = ATD0DR5;
        au16AnalogSamples[6].u16Sample[u8BufferStoreIndex] = ATD0DR6;
        au16AnalogSamples[7].u16Sample[u8BufferStoreIndex] = ATD0DR7;
 
        /* Update index that controls sample store location within the buffers */
        u8BufferStoreIndex++;
        /* If index exceeds buffer size, wrap around */
        if (u8BufferStoreIndex >= ADC_SAMPLE_BUFFER_SIZE)
        {
            u8BufferStoreIndex = 0;
        }
    }
  #endif
} 

/*******************************************************************************/
/**
* \brief    Compute the average for each analog sample buffer.
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void vfnADC_Compute_Average(void)
{ 
    UINT16 * pu16SampleData;
    UINT8 u8var1;
    UINT16 u16Average = 0;
    
    for (u8var1=0; u8var1<ADC_CHANNELS_TO_SAMPLE; u8var1++)
    {   
        pu16SampleData =  &au16AnalogSamples[u8var1].u16Sample[0];
        
        /* Calculate the average of all samples that correspond to the same ADC channel */
        u16Average = *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        u16Average += *pu16SampleData++;
        
        au16AnalogAverage[u8var1] = u16Average >> 4;
        /* Clear auxiliary variable in preparation for next iteration */
        u16Average = 0;
    }
} 

/*******************************************************************************/
/**
* \brief    Read ADC data from one specific channel and return the read data
* \author   OH
* \param    UINT8 channel, possible values: 1,2,3,4,5,6,7
* \return   UNIT16 read value of analog sample
*/
UINT16 u16fnADC_Read_One_Channel(UINT8 u8Channel)
{ 
   UINT16 u16adc_read = 0;
    
    /* Read analog samples if conversion process is completed */ 
    {    
    #if HARDWARE == (EMULATOR | PROTOTYPE)

       switch(u8Channel)
       {
          case (0):
            u16adc_read = ATD0DR0;
            break;

          case (1):
            u16adc_read = ATD0DR1;         
            break;

          case (2):
            u16adc_read = ATD0DR2;         
            break;

          case (3):
            u16adc_read = ATD0DR3;         
            break;

          case (4):
            u16adc_read = ATD0DR4;         
            break;

          case (5):
            u16adc_read = ATD0DR5;         
            break;

          case (6):
            u16adc_read = ATD0DR6;         
            break;

          case (7):
            u16adc_read = ATD0DR7;         
            break;
           
          default:
            break;
       }
    #endif   
    return   u16adc_read; 
    }    
}
/*******************************************************************************/


#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt ATD0_conversion_complete(void){
//Interrupt service routine for A/D converter.  Should be moved to ADC.c file.    
    
    #if HARDWARE == (EMULATOR | PROTOTYPE)
    //These use single conversion to get data.
    //store current sample in appropiate place
    switch(signals_array[signals_pointer]){
        #ifdef VBAT
        case 0:
        vfnVBAT_Monitoring(ATD0DR0);        
        break;
        #endif                
        
        #ifdef TPS
        case 1:
        vfnTPS_Monitoring(ATD0DR0);
        break;
        #endif
        
        #ifdef ETEMP
        case 2:
        vfnETEMP_Monitoring(ATD0DR0);
        break;
        #endif
        
        #ifdef MAP
        case 3:
        vfnMAP_Monitoring(ATD0DR0);
        break;
        #endif

        #ifdef O2 
        case 4:
        vfnO2_Monitoring(ATD0DR0);
        break;
        #endif                                                                
        
        #ifdef ATEMP
        case 5:
        vfnATEMP_Monitoring(ATD0DR0);
        break;
        #endif                
        
        #ifdef MAF
        case 6:
        vfnMAF_Monitoring(ATD0DR0);
        break;
        #endif
        
        #ifdef BAP
        case 7:
        vfnBAP_Monitoring(ATD0DR0);
        break;
        #endif
    }
    //trigger next conversion
    signals_pointer++;    
    if(signals_pointer>=signals_array_length){
        signals_pointer = 0; 
    } else {                    
        vfn_convertADC_Channel(signals_array[signals_pointer]);        
    }
    
    #else if HARDWARE == REFERENCE
    //S12P uses continuous conversion sequence to get data
    //Strategy is to store the data in a buffer so that most recent data is available
    //for the data management task to use.
    //All 10 channels are stored in buffer regardless of signals defined.
    #ifdef Analog_Data_8
      //8 bit data
      RAW_ADC[0]=(UINT8)ATDDR0;
      RAW_ADC[1]=(UINT8)ATDDR1;
      RAW_ADC[2]=(UINT8)ATDDR2;
      RAW_ADC[3]=(UINT8)ATDDR3;
      RAW_ADC[4]=(UINT8)ATDDR4;
      RAW_ADC[5]=(UINT8)ATDDR5;
      RAW_ADC[6]=(UINT8)ATDDR6;
      RAW_ADC[7]=(UINT8)ATDDR7;
      RAW_ADC[8]=(UINT8)ATDDR8;
      RAW_ADC[9]=(UINT8)ATDDR9;
    #else
      //16 bit data 
      RAW_ADC[0]=ATDDR0;
      RAW_ADC[1]=ATDDR1;
      RAW_ADC[2]=ATDDR2;
      RAW_ADC[3]=ATDDR3;
      RAW_ADC[4]=ATDDR4;
      RAW_ADC[5]=ATDDR5;
      RAW_ADC[6]=ATDDR6;
      RAW_ADC[7]=ATDDR7;
      RAW_ADC[8]=ATDDR8;
      RAW_ADC[9]=ATDDR9;
    #endif

      //Data is buffered for data management to collect at the necessary time.
    
      //Start next sequence at channel 0
      ATDCTL5 = 0x30;
    
    #endif
    
    //clear flag
    #if HARDWARE == REFERENCE
      ATDSTAT0_SCF = 1;
    #else
      ATD0STAT0_SCF = 1;
    #endif
}

#pragma CODE_SEG DEFAULT

