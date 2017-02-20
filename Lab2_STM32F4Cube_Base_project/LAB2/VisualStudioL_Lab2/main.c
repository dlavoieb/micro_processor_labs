#include <stm32f4xx_hal.h>
#include "adc.h"
#include "SysClock_Config.h"
#include "display.h"
#include "conversion.h"
#include "button.h"

#define ADC_BUFFER_LENGTH 5
#define COEFFS_LENGTH 5
#define FILTER_COUNTER 10 // todo: change to adjust filtering frequency
#define DISPLAY_COUNTER 15 // todo: change to adjust display frequency
#define ALARM_COUNTER 150 // todo: change to adjust alarm toggle frequency

static float coeffs[COEFFS_LENGTH] = { 0.1, 0.15, 0.5, 0.15, 0.1 };
uint32_t g_ADCBuffer[ADC_BUFFER_LENGTH];

extern ADC_HandleTypeDef g_AdcHandle;
volatile int systick_flag;		///< Flag set in the SysTick ISR, drives timing in the main loop

uint32_t display_counter;		///< Counter to set display update frequency
DigitNumber display_pin;		///< Keep track of which digit is updated
uint32_t filter_counter;		///< Counter to set the filtering routing frequency
DisplayUnits unit_selector;		///< Determines which unit the temperature is displayed in
GPIO_PinState past_button;		///< Save past button value to prevent continuous switching of units
float g_AdcValue;				///< Filtered RAW Adc value, still needs scaling and interpretation
float temperature;				///< ADC value converted to the proper units
uint8_t alarm;					///< Boolean flag for enabling the alarm mechanism
uint32_t alarm_counter;			///< Counter to flicker temperature display

int main(void)
{
	// Init system modules and sub-systems
	HAL_Init();
	SystemClock_Config();
	ConfigureADC();
	ConfigureDMA();
	HAL_ADC_Start_DMA(&g_AdcHandle, g_ADCBuffer, ADC_BUFFER_LENGTH);
	init_button_gpio();
	gpio_led_init();
	
	// Initialize varibles 
	unit_selector = CELCIUS_UNITS;
	display_pin = DIGIT_1;
	display_counter = 0;
	filter_counter = 0;
	alarm_counter = 0;
	systick_flag = 0;
	past_button = GPIO_PIN_RESET;
	
	// Main-Loop
	while (1)
	{
		if (systick_flag == 1) // Systick interrupt flag
		{
			systick_flag = 0;
			// Filter new set of ADC values
			if (filter_counter++ > FILTER_COUNTER) 
			{
				filter_counter = 0;
				g_AdcValue = 0;
				
				for (int i = 0; i < 5; i++)
					g_AdcValue += *(g_ADCBuffer + i) * coeffs[i];
			}
			
			// Display new data element
			alarm_counter = (alarm_counter + 1) % ALARM_COUNTER;
			// Update displayed value only at start of cycle
			if (display_pin == DIGIT_1)
			{
				// Check button and change units
				if (read_button() == GPIO_PIN_SET)
				{
					if (past_button == GPIO_PIN_RESET)
						unit_selector = !unit_selector;
					past_button = GPIO_PIN_SET;
				}
				else 
					past_button = GPIO_PIN_RESET;
				
				// Check if we update displayed value
				if (display_counter++ > DISPLAY_COUNTER)
				{
					display_counter = 0;
					// Convert ADC value to selected units
					if (unit_selector == CELCIUS_UNITS) 
						temperature = celcius_from_ADC_RAW(g_AdcValue);
					else if (unit_selector == FARENHEIT_UNITS)
						temperature = fahrenheit_from_ADC_RAW(g_AdcValue);
				}
				alarm = temp_alarm(temperature, unit_selector);
			}
				
			// Increment which pin is updated and display value
			display_pin = (display_pin + 1) % 4; 
				
			// Check is alarm is active
			if (!alarm || alarm_counter > ALARM_COUNTER/2)
				// Normal display
				display_temperature(temperature, unit_selector, display_pin);				
			else
				// Disable display
				display_temperature(-1, unit_selector, display_pin);
			
		}
		
		// Identify units with LED for debug
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
