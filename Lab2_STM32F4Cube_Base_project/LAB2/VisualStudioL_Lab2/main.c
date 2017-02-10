#include <stm32f4xx_hal.h>
#include "adc.h"
#include "SysClock_Config.h"
#include "display.h"
#ifdef __cplusplus
extern "C"
#endif

static float coeffs[5] = { 0.1, 0.15, 0.5, 0.15, 0.1 };
float g_AdcValue; // Filtered RAW Adc value, still needs scaling and interpretation
float celcius;
float farenheit;
enum{ ADC_BUFFER_LENGTH = 4096 };
uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];

extern ADC_HandleTypeDef g_AdcHandle;

volatile int systick_flag; // flag to tell if a systick interrup has occured

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	ConfigureADC();
	ConfigureDMA();
	HAL_ADC_Start_DMA(&g_AdcHandle, g_ADCBuffer, ADC_BUFFER_LENGTH);

	// start systick interrupt to run the filter 
	
	gpio_led_init();
	init_segment_disp();
	init_segment_segments();
	
	while (1)
	{
		if (systick_flag == 1)
		{
			systick_flag = 0;
			g_AdcValue = 0;
			for (int i = 0; i < 5; i++)
			{
				g_AdcValue += *(g_ADCBuffer + i) * coeffs[i];
			}
			celcius = celcius_from_ADC_RAW(g_AdcValue);
			farenheit = fahrenheit_from_ADC_RAW(g_AdcValue);
			led_number(6, 0);
		}
	}

}

