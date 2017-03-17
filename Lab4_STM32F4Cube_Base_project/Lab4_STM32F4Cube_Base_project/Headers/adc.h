#include <stm32f4xx_hal.h>
#include "cmsis_os.h"

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

void ConfigureADC(void);
int start_temp_thread(void);
void temp_acquisition_thread(void const * argument);

#endif // ADC_CONFIG_H_
