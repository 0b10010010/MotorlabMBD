#include "AnalogInPB0.h"

void AnalogInPB0(void) {
    for(int i=0; i<8; i++) {
        d_ADCRawValues[i] = 0;
    }   

    // Setup ADC to scan 1 signals on pins PBO continuously using DMA to write
    // 20 values to a circular buffer.
    // Runs at about 19kHz for each 20 value scan -- speed found with testing.

    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
    uint32_t ComponentsUsed;

    /* Enable clocks ****************************************/
    ComponentsUsed =  RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOB;  // AHB1 components
    RCC_AHB1PeriphClockCmd(ComponentsUsed, ENABLE);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* DMA2 Stream0 channel0 configuration **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                             
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);          
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)d_ADCRawValues;  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 20;                                  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    /* Configure pins on GPIOs as analog input ******************************/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // ADC_IN8 on GPIOB
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  

    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;           
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // no external trigger
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;     // Shouldn't matter
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      // 12 bit right aligned
    ADC_InitStructure.ADC_NbrOfConversion = 1;     
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channels configuration *************************************/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_15Cycles);  

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    ADC1->CR2 |= 0x40000000;  //Start Conversion of regular channels

    //setupInterrupt();  // only for testing
}

float getAvgVoltage(void) {
    float average = 0.0f;
    for (uint8_t i=0; i<20; i++) {
        average += d_ADCRawValues[i];
    }
    return average*3.3f/20.0f/0xFFF;
} 

// void setupInterrupt(void) { // not usually called - just for testing
//     // enable the Transfer Complete interrupt on DMA
//     NVIC_InitTypeDef NVIC_InitStructure;
//     DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);    
//     NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
// }
