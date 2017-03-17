#include "conversion.h"
static const float V25 = 0.76;
static const float av_slope = 2.5 / 1000;

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

float to_unit_from_ADC_RAW(float adc_val, DisplayUnits units)
{
	switch(units){
		case CELCIUS_UNITS:
			return celcius_from_ADC_RAW(adc_val);
		case FARENHEIT_UNITS:
			return fahrenheit_from_ADC_RAW(adc_val);
	}
}
