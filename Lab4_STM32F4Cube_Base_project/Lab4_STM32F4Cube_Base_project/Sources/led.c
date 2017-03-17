#include "led.h"

static const uint32_t TIMER_PERIOD = 100;

void init_internal_led_gpio_toggles(void)
{
	GPIO_InitTypeDef gpio_init;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Speed = GPIO_SPEED_LOW;
	gpio_init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpio_init.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOD, &gpio_init);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

void set_internal_led(GPIO_PinState state, uint16_t led)
{
	HAL_GPIO_WritePin(GPIOD, led, state);
}

void init_pwm_led(void)
{
	/*
	TIM4 GPIO Configuration    
	PD15	------> TIM4_CH4
	PD14	------> TIM4_CH3
	PD13	------> TIM4_CH2
	PD12	------> TIM4_CH1 
	*/
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OC_InitTypeDef oc_config;
	TIM_HandleTypeDef led_timer_handle;
	TIM_MasterConfigTypeDef sMasterConfig;

	__TIM4_CLK_ENABLE();
		
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
//	timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)
//	PWM_frequency = timer_tick_frequency / (TIM_Period + 1)

	led_timer_handle.Instance = TIM4;
	led_timer_handle.Init.Prescaler = 0;
	led_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	led_timer_handle.Init.Period = TIMER_PERIOD;
	led_timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&led_timer_handle);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&led_timer_handle, &sMasterConfig);

	oc_config.OCMode = TIM_OCMODE_PWM2;
	oc_config.OCIdleState = TIM_OCIDLESTATE_SET;
	oc_config.Pulse = 0;
	oc_config.OCPolarity = TIM_OCPOLARITY_LOW;
	oc_config.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_PWM_ConfigChannel(&led_timer_handle, &oc_config, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&led_timer_handle, &oc_config, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&led_timer_handle, &oc_config, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&led_timer_handle, &oc_config, TIM_CHANNEL_4);
	
	HAL_TIM_PWM_MspInit(&led_timer_handle);
	HAL_TIM_Base_Start(&led_timer_handle); 
	
	HAL_TIM_PWM_Start(&led_timer_handle, TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&led_timer_handle, TIM_CHANNEL_2); 
	HAL_TIM_PWM_Start(&led_timer_handle, TIM_CHANNEL_3); 
	HAL_TIM_PWM_Start(&led_timer_handle, TIM_CHANNEL_4); 
}

void set_duty_cycle_percent(float percent, LED led)
{
	switch (led)
	{
	case LED_GREEN:
		TIM4->CCR1 = (uint32_t) percent;
		break;
	case LED_ORANGE:
		TIM4->CCR2 = (uint32_t) percent;
		break;
	case LED_RED:
		TIM4->CCR3 = (uint32_t) percent;
		break;
	case LED_BLUE:
		TIM4->CCR4 = (uint32_t) percent;
		break;
	}
}
