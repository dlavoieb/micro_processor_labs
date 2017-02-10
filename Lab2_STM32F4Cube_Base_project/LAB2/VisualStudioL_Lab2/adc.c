#include "adc.h"

DMA_HandleTypeDef  g_DmaHandle;
ADC_HandleTypeDef g_AdcHandle;

void ConfigureADC()
{
	__GPIOC_CLK_ENABLE();
	__ADC1_CLK_ENABLE();

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	ADC_ChannelConfTypeDef adcChannel;

	g_AdcHandle.Instance = ADC1;

	g_AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;		// 168 MHz divided by 8 = 21 MHz
	g_AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;					// 12 bits, so 15 cycles, so 1.4 MHz processing
	g_AdcHandle.Init.ScanConvMode = DISABLE;
	g_AdcHandle.Init.ContinuousConvMode = ENABLE;						//Enable continuous conversion 
	g_AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	g_AdcHandle.Init.NbrOfDiscConversion = 1;
	g_AdcHandle.Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;
	g_AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	g_AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	g_AdcHandle.Init.NbrOfConversion = 1;
	g_AdcHandle.Init.DMAContinuousRequests = ENABLE;					// Enable DMA requests to use DMA
	g_AdcHandle.Init.EOCSelection = DISABLE;

	HAL_ADC_Init(&g_AdcHandle);
	
	adcChannel.Channel = ADC_CHANNEL_TEMPSENSOR;
	adcChannel.Rank = 1;
	adcChannel.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	adcChannel.Offset = 0;

	if (HAL_ADC_ConfigChannel(&g_AdcHandle, &adcChannel) != HAL_OK)		// Make sure ADC configuration did not fuck up
	{
#ifdef VISUAL_STUDIO
		asm("bkpt 255");
#else
		;
#endif // VISUAL_STUDIO

	}
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
