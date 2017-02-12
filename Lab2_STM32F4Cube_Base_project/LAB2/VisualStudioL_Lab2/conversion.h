#ifndef CONVERSION_H_
#define CONVERSION_H_
#include "display.h"

float celcius_from_ADC_RAW(float adc_val);
float fahrenheit_from_ADC_RAW(float adc_val);
int temp_alarm(float temp, DisplayUnits units);

#endif // !CONVERSION_H_
