#include <stm32f4xx_hal.h>
#include "adc.h"
#include "SysClock_Config.h"
#include "display.h"
#include "conversion.h"
#include "button.h"
#ifdef __cplusplus
extern "C"
#endif
#define ADC_BUFFER_LENGTH 4096
#define COEFFS_LENGTH 5

static float coeffs[COEFFS_LENGTH] = { 0.1, 0.15, 0.5, 0.15, 0.1 };
float g_AdcValue; // Filtered RAW Adc value, still needs scaling and interpretation
float temperature;
uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];

extern ADC_HandleTypeDef g_AdcHandle;

volatile int systick_flag; // flag to tell if a systick interrup has occured

uint32_t display_counter;
DigitNumber display_pin;
uint32_t filter_counter;
DisplayUnits unit_selector;
GPIO_PinState past_button;

#define FILTER_COUNTER 100 // todo: change to adjust filtering frequency
#define DISPLAY_COUNTER 1000 // todo: change to adjust display frequency

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	ConfigureADC();
	ConfigureDMA();
	HAL_ADC_Start_DMA(&g_AdcHandle, g_ADCBuffer, ADC_BUFFER_LENGTH);
	init_button_gpio();
	gpio_led_init();
	
	unit_selector = CELCIUS_UNITS;
	display_pin = DIGIT_1;
	display_counter = 0;
	filter_counter = 0;
	systick_flag = 0;
	past_button = read_button();
	
	while (1)
	{
		if (systick_flag == 1)
		{
			systick_flag = 0;
			if (filter_counter++ > FILTER_COUNTER)
			{
				filter_counter = 0;
				g_AdcValue = 0;
				
				for (int i = 0; i < 5; i++)
					g_AdcValue += *(g_ADCBuffer + i) * coeffs[i];
			}
			
			if (display_counter++ > DISPLAY_COUNTER)
			{
				if (display_pin == DIGIT_1)
				{
					//read pin a.0 and change unit if button is pressed
					if (read_button() == GPIO_PIN_SET)
					{
						if (past_button == GPIO_PIN_RESET)
							unit_selector = !unit_selector;
						past_button = GPIO_PIN_SET;
					}
					else 
						past_button = GPIO_PIN_RESET;
					
					if (unit_selector == CELCIUS_UNITS) 
						temperature = celcius_from_ADC_RAW(g_AdcValue);
					else if (unit_selector == FARENHEIT_UNITS)
						temperature = fahrenheit_from_ADC_RAW(g_AdcValue);
				}

				display_pin = (display_pin + 1) % 4; // go to next digit 
				display_temperature(temperature, unit_selector, display_pin);				
			}
		}
		if (unit_selector == CELCIUS_UNITS)
		{
			LED_BLUE_ON_
			LED_GREEN_OFF_
		}
		else
		{
			LED_BLUE_OFF_
			LED_GREEN_ON_
		}
	}

}

