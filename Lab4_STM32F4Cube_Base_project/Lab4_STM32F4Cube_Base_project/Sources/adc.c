#include "adc.h"

ADC_HandleTypeDef g_AdcHandle;
float g_AdcValue;				///< Filtered RAW Adc value, still needs scaling and interpretation
float temperature;				///< ADC value converted to the proper units
#define ADC_BUFFER_LENGTH 5
uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];
#define POLL_TIMEOUT 10000
#define COEFFS_LENGTH 5
static float coeffs[COEFFS_LENGTH] = { 0.1, 0.15, 0.5, 0.15, 0.1 };
extern osMutexId temperature_global_mutex_id;

osThreadId(TempAcquistionTID);
osThreadDef(temp_acquisition_thread, osPriorityNormal, 1, 0);

void ConfigureADC()
{
	ADC_ChannelConfTypeDef adcChannel;

	__GPIOC_CLK_ENABLE();
	__ADC1_CLK_ENABLE();

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

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
}

int start_temp_thread(void){
	TempAcquistionTID = osThreadCreate(osThread(temp_acquisition_thread), NULL);
	if(!TempAcquistionTID) return(-1);
	return(0);
}

void temp_acquisition_thread(void const * argument)
{
	while(1){
		int i;
		for (i = 0; i < ADC_BUFFER_LENGTH - 1; i++)
		{
			g_ADCBuffer[i] = g_ADCBuffer[i + 1];
		}
		
		HAL_ADC_Start(&g_AdcHandle);
		if (HAL_ADC_PollForConversion(&g_AdcHandle, POLL_TIMEOUT) == HAL_OK)
			g_ADCBuffer[ADC_BUFFER_LENGTH-1] = HAL_ADC_GetValue(&g_AdcHandle);
		HAL_ADC_Stop(&g_AdcHandle);
		
		osMutexWait(temperature_global_mutex_id, osWaitForever);
		g_AdcValue = 0;
		for (i = 0; i < 5; i++)
			g_AdcValue += *(g_ADCBuffer + i) * coeffs[i];

		osMutexRelease(temperature_global_mutex_id);
		
		osDelay(50);
	}
}
