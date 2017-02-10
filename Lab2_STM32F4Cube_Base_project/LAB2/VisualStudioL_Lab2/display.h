#include <stm32f4xx_hal.h>

#ifndef DISPLAY_H
#define DISPLAY_H

//#define CELCIUS_UNITS ((uint8_t) (1))
//#define FARENHEIT_UNITS ((uint8_t) (2))

typedef enum
{
	CELCIUS_UNITS = 0,
	FARENHEIT_UNITS
} DisplayUnits;

typedef enum
{
	DIGIT_1 = 0,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4
}DigitNumber;

void display_temperature(float temperature, DisplayUnits units, DigitNumber digit);

void gpio_led_init(void);

#endif // !DISPLAY_H
