#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "LIS3DSH.h"
#include "keypad.h"
#include "clock_config.h"

int main(void)
{	
  HAL_Init();
	char c;
  /* Configure the system clock */
  SystemClock_Config();
	keypad_init();
	keypad_read_char(&c);
  /* Initialize all configured peripherals */

	while (1){
		printf("a");
	}
}

