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
