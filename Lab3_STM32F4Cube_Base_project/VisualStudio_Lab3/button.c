#include "button.h"

void init_button_gpio(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.Mode = GPIO_MODE_INPUT;
	gpio_init.Speed = GPIO_SPEED_LOW;
	gpio_init.Pin = GPIO_PIN_0;
	gpio_init.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

GPIO_PinState read_button(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}
