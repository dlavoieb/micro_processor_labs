#include <stm32f4xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>
#include "main.h"
#include "display.h"
#include "accelerometer.h"

osThreadId LEDThread1Handle, LEDThread2Handle;
osThreadId displayTreadHandle;

enum ApplicationDisplayModeEnum
{
	TEMPERATURE_DISPLAY,
	TILT_DISPLAY
};

struct ApplicationStateType
{
	// Display mode of the application.
	enum ApplicationDisplayModeEnum display_mode;
	
	// Orrientation
	
	// Temperature
	// Hazard?
} ApplicationState;

static void LED_Thread1(void const *argument);
static void LED_Thread2(void const *argument);

int main(void)
{
  /* STM32F4xx HAL library initialization:
	   - Configure the Flash prefetch, instruction and Data caches
	   - Configure the Systick to generate an interrupt each 1 msec
	   - Set NVIC Group Priority to 4
	   - Global MSP (MCU Support Package) initialization
	 */
	HAL_Init();  
	
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// Acceleromter setup.
	accelerometer_init();

	/* Thread 1 definition */
	osThreadDef(LED1, LED_Thread1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  
	 /*  Thread 2 definition */
	osThreadDef(LED2, LED_Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	
	osThreadDef(DISPLAY, display_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	
	displayTreadHandle = osThreadCreate(osThread(DISPLAY), NULL);
  
	/* Start scheduler */
	osKernelStart();

	  /* We should never get here as control is now taken by the scheduler */
	for (;;)
		;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}

/**
  * @brief  Toggle LED1
  * @param  thread not used
  * @retval None
  */
static void LED_Thread1(void const *argument)
{
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		osDelay(2000);
		
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		osThreadSuspend(LEDThread2Handle);
		osDelay(2000);
		
		osThreadResume(LEDThread2Handle);
	}
}

/**
  * @brief  Toggle LED2 thread
  * @param  argument not used
  * @retval None
  */
static void LED_Thread2(void const *argument)
{
	uint32_t count;
	(void) argument;
  
	for (;;)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		osDelay(200);
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
