#include "conversion.h"
static const float V25 = 0.76;
static const float av_slope = 2.5 / 1000;

static const float THRESHOLD_CELCIUS = 45.0;
static const float THRESHOLD_FAHRENHEIT = 113.0;

float celcius_from_ADC_RAW(float adc_val) {
	adc_val = adc_val * 3.0 / 4096.0;
	return ((adc_val - V25) / av_slope) + 25;
}

float fahrenheit_from_ADC_RAW(float adc_val)
{
	float celcius = celcius_from_ADC_RAW(adc_val);
	return 32.0 + (celcius * 9.0 / 5.0);
}

int temp_alarm(float temp, DisplayUnits units)
{
	if (units == CELCIUS_UNITS)
		return temp > THRESHOLD_CELCIUS;
	else
		return temp > THRESHOLD_FAHRENHEIT;
}
