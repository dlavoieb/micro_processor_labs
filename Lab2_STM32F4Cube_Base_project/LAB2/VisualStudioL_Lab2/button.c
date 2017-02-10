#include "button.h"

void init_button_gpio(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Speed = GPIO_SPEED_LOW;
	gpio_init.Pin = GPIO_PIN_0;
	gpio_init.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

void init_internal_led_gpio(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init;
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

GPIO_PinState read_button(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}
