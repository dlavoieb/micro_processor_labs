#include <stm32f4xx_hal.h>

#ifndef DISPLAY_H
#define DISPLAY_H

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
 *	PIN  1 = PIN E 12			// Digit 1 Selector (Inverted)
 *	PIN  2 = PIN E 13			// Digit 2 Selector (Inverted)
 *	PIN  6 = PIN E 14			// Digit 3 Selector (Inverted)
 *	PIN  8 = PIN E 15			// Digit 4 Selector (Inverted)
 */

/**
 * @brief Unit type for display
 */
typedef enum
{
	CELCIUS_UNITS = 0,
	FARENHEIT_UNITS
} DisplayUnits;

/**
 * @brief Selector for active digit in 7-segment display
 */
typedef enum
{
	DIGIT_1 = 0,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4
}DigitNumber;

/**
 * @brief Sets the GPIO to output the selected digit of the temperature 
 *
 * This display function will display temperatures values between 0 and 999,
 * in the specified units.
 *
 * It is left to the caller to figure out at what frequency and with which digit 
 * to call the function. It should be called once for every digit on the display, 
 * so four (4) times for the complete number.
 *
 * @param temperature The temperature value to display
 * @param units The unit suffix will change based on this parameter
 * @param ditig This select which of the 4 digits will be set
 */
void display_temperature(float temperature, DisplayUnits units, DigitNumber digit);

/**
 * @brief Initalizes the GPIO clocks and sets the pin directions and properties.
 */
void gpio_led_init(void);

void display_angle(int16_t angle, char suffix, DigitNumber digit);

#endif // !DISPLAY_H
