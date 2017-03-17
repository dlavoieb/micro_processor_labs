#ifndef CONVERSION_H_
#define CONVERSION_H_
#include "display.h"

static const float THRESHOLD_CELCIUS = 45.0;
static const float THRESHOLD_FAHRENHEIT = 113.0;

float celcius_from_ADC_RAW(float adc_val);
float fahrenheit_from_ADC_RAW(float adc_val);
int temp_alarm(float temp, DisplayUnits units);
float to_unit_from_ADC_RAW(float adc_val, DisplayUnits units);

#endif // !CONVERSION_H_
