#include "adc.h"

DMA_HandleTypeDef  g_DmaHandle;
ADC_HandleTypeDef g_AdcHandle;

void ConfigureADC()
{
	ADC_ChannelConfTypeDef g_AdcChannel;									// definition of ADC1 channel struct
	ADC_MultiModeTypeDef g_AdcMode; 										// define ADC1 mode struct
	
	/*  initialize ADC init struct */
	g_AdcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;					// ADC Clock frequency 42MHz (168/4)
	g_AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;							// 12 bit resolution, better but slower
	g_AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;							// align the 12 bits data at the right of the 32 bits words
	g_AdcHandle.Init.ScanConvMode = DISABLE;									// single channel mode
	g_AdcHandle.Init.EOCSelection = ADC_EOC_SEQ_CONV;							// perform ADC conversions without having to read all conversion data
	g_AdcHandle.Init.ContinuousConvMode = DISABLE;								// single mode convertion
	g_AdcHandle.Init.DMAContinuousRequests = DISABLE;							// single mode DMA request
	g_AdcHandle.Init.NbrOfConversion = 1;										// one conversion
	g_AdcHandle.Init.DiscontinuousConvMode = ENABLE;							// enable discountinuous mode
	g_AdcHandle.Init.NbrOfDiscConversion = 1;									// one conversion
	g_AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;						// no external trigger
	g_AdcHandle.Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;					// no external trigger
	
	/* initialize ADC handle struct */
	g_AdcHandle.Instance = ADC1;
	g_AdcHandle.NbrOfCurrentConversionRank = 1;
	g_AdcHandle.State = 0;
	g_AdcHandle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/* initialize ADC channel struct */
	g_AdcChannel.Channel = ADC_CHANNEL_TEMPSENSOR;
	g_AdcChannel.Rank = 1;												// use to determine the rank in which this channel is sampled
	g_AdcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longuer means more accurate
	g_AdcChannel.Offset = 0;
	
	/* initialize ADC mode struct */
	g_AdcMode.Mode = ADC_MODE_INDEPENDENT;														
	g_AdcMode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	g_AdcMode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	HAL_ADC_ConfigChannel(&g_AdcHandle, &g_AdcChannel);
	HAL_ADCEx_MultiModeConfigChannel(&g_AdcHandle, &g_AdcMode);	/* configure mode */

	//GPIO_InitTypeDef GPIO_InitDef; 								
	__HAL_RCC_GPIOA_CLK_ENABLE(); 									
}

void ConfigureDMA()
{
	__DMA2_CLK_ENABLE(); 
	g_DmaHandle.Instance = DMA2_Stream4;
  
	g_DmaHandle.Init.Channel  = DMA_CHANNEL_0;						// ADC 1 is on channel 0 stream 4 (or 0)
	g_DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;				// Dump data to memory
	g_DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	g_DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	g_DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	g_DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	g_DmaHandle.Init.Mode = DMA_CIRCULAR;							// Circular mode so data is continuously read
	g_DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
	g_DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
	g_DmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	g_DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;
	g_DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE; 
    
	HAL_DMA_Init(&g_DmaHandle);
    
	__HAL_LINKDMA(&g_AdcHandle, DMA_Handle, g_DmaHandle);
 
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);   
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
}

void DMA2_Stream4_IRQHandler()
{
	HAL_DMA_IRQHandler(&g_DmaHandle);
}
 
void ADC_IRQHandler()
{
	HAL_ADC_IRQHandler(&g_AdcHandle);
}
