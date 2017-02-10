#include "display.h"
#include <assert.h>

#define V25 0.76

char unit_char;

float celcius_from_ADC_RAW(float adc_val) {
    adc_val = adc_val * 3.0 / 4096.0;
    float av_slope = 2.5 / 1000;
    return ((adc_val - V25) / av_slope) + 25;
}

float fahrenheit_from_ADC_RAW(float adc_val)
{
    float celcius = celcius_from_ADC_RAW(adc_val);
    return 32.0 + (celcius * 9.0 / 5.0);
}


void display_temperature(float temperature, uint8_t units)
{
    assert(units == CELCIUS_UNITS || units == FARENHEIT_UNITS); // carefull asserts are only compiled and executed in debug code
    unit_char = (units == CELCIUS_UNITS) ? 'C' : 'F'; // If not celcius, assume it's farenheit

}

/* 7-SEGMENTS DISPLAY TO STM32F4XX PIN REFERENCE
 *
 *	PIN 14 = PIN D 11			// A
 *	PIN 16 = PIN D 10			// B
 *	PIN 13 = PIN D  9			// C
 *	PIN  3 = PIN D  8			// D
 *	PIN  5 = PIN B 15			// E
 *	PIN 11 = PIN B 14			// F
 *	PIN 15 = PIN B 13 			// G
 *	PIN  7 = PIN B 12			// LOWER DOT
 *	PIN 10 = PIN B 11			// UPPER DOT
*/

// SWITCH STATEMENT FOR CHOSING WHAT NUMBER TO DISPLAY
void led_number(char number, int dot) {
    switch (number) {

        // DISPLAY NUMBER 0
    case 0:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
        break;

// DISPLAY NUMBER 1
    case 1:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);                  // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
        break;

// DISPLAY NUMBER 2
    case 2:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);                   // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

      // DISPLAY NUMBER 3
    case 3:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

// DISPLAY NUMBER 4
    case 4:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);                  // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

      // DISPLAY NUMBER 5
    case 5:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);                  // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

      // DISPLAY NUMBER 6
    case 6:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);                  // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

      // DISPLAY NUMBER 7
    case 7:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
        break;

      // DISPLAY NUMBER 8
    case 8:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
        break;

      // DISPLAY NUMBER 9
    case 9:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G

    default:
        break;
    }

    // DISPLAY DOT
    if (dot == 1) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                    // LOWER DOT
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);                  // UPPER DOT
    }
    else if (dot == 2) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                  // LOWER DOT
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);                    // UPPER DOT
    }
    else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                  // LOWER DOT
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);                  // UPPER DOT
    }
}

// SWITCH STATEMENT FOR CHOSING WHAT CHARACTER TO DISPLAY
void led_char(char number, int dot) {
	switch (number) {
		
		// DISPLAY NOTHING
	case '#':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);				// A
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);				// B
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);				// C
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);				// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);				// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);				// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);				// G
		break;
		
	case 'f':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);				// A
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);				// B
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);				// C
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);				// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);				// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);				// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);				// G
		break;
		
// DISPLAY 'C'
	case 'c':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);				// A
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);				// B
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);				// C
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);					// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);				// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);				// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);				// G
		break;
		
	default:
		break;
	}
}

// INITIALISE LED SEGMENTS
void gpio_led_init(void) {
	GPIO_InitTypeDef GPIO_InitDef; 								
	__HAL_RCC_GPIOD_CLK_ENABLE(); 									
	
	GPIO_InitDef.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8, GPIO_PIN_RESET);
}

// INITIALISE 7-SEGMENTS DISPLAY
void init_segment_disp(void)
{
	GPIO_InitTypeDef GPIO_InitDef; 								
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// INDIVIDUAL DISPLAY MAPPED TO RGB LEDS (PIN 12-15)
	GPIO_InitDef.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11, GPIO_PIN_RESET);
}

// INITIALISE 7-SEGMENTS SEGMENTS
void init_segment_segments(void)
{
	GPIO_InitTypeDef GPIO_InitDef; 								
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	// INDIVIDUAL DISPLAY MAPPED TO RGB LEDS (PIN 12-15)
	GPIO_InitDef.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12; 	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12, GPIO_PIN_RESET);
}