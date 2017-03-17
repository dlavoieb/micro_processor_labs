#include "timer.h"
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"

TIM_HandleTypeDef TIM_Handle2;
volatile uint8_t timer2_flag;
uint8_t display_timer_flag;
void init_timers(void)
{
	__TIM2_CLK_ENABLE();
	
	// PERIOD*PRESCALAR CANNOT EXCEED 32BIT (TIM2)
	TIM_Handle2.Instance				= TIM2;
	TIM_Handle2.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV4;
	TIM_Handle2.Init.Prescaler 			= 0x1000U;														// PRESCALAR OF 999
	TIM_Handle2.Init.Period 			= 0xFFFFU;														// PERIOD OF 6999 FOR OPTIMAL POLLING RATE OF KEYPAD
	TIM_Handle2.Init.CounterMode 		= TIM_COUNTERMODE_UP;
	TIM_Handle2.Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
	TIM_Handle2.Init.RepetitionCounter 	= 0;
	
	if (HAL_TIM_Base_Init(&TIM_Handle2))
		Error_Handler(TIM_INIT_FAIL);
		
	HAL_NVIC_SetPriority(TIM2_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start_IT(&TIM_Handle2);
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_Handle2);
}

// CALLBACK FUNCTION, TIME THE IRQHANDLER IS CALLED
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// CHECK FOR TIMER INSTANCES
	if (htim->Instance == TIM2)
	{
		timer2_flag = 1; //SET FLAG TO BE USED IN MAIN
	}
}

void start_abort_timer(void)
{
	HAL_TIM_Base_Start_IT(&TIM_Handle2);
}

void stop_abort_timer(void)
{
	HAL_TIM_Base_Stop_IT(&TIM_Handle2);
}
