#include <stm32f4xx_hal.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#define CELCIUS_UNITS ((uint8_t) (1))
#define FARENHEIT_UNITS ((uint8_t) (2))

float celcius_from_ADC_RAW(float adc_val);
float fahrenheit_from_ADC_RAW(float adc_val);

void display_temperature(float temperature, uint8_t units);

void gpio_led_init(void);
void init_segment_disp(void);
void init_segment_segments(void);
void led_number(char number, int dot);
void led_char(char number, int dot);

#endif // !DISPLAY_H
