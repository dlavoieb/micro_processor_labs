#include "keypad.h"
#include "stm32f4xx_hal.h"

#define R1 GPIOB, GPIO_PIN_6
#define R2 GPIOB, GPIO_PIN_7
#define R3 GPIOB, GPIO_PIN_8
#define R4 GPIOB, GPIO_PIN_9
#define C1 GPIOC, GPIO_PIN_6
#define C2 GPIOC, GPIO_PIN_7
#define C3 GPIOC, GPIO_PIN_8
#define C4 GPIOC, GPIO_PIN_9

uint8_t keypress_flag;

// Private Functions
char internal_read_char();

void enable_keypad_interrupt(void)
{
	// Drive outputs high
	HAL_GPIO_WritePin(R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_SET);
	
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	
}

void disable_keypad_interrupt(void)
{
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);	
}

// Interrupt handler
void EXTI9_5_IRQHandler(void) {
	// key pressed, ask for key read
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
	keypress_flag = 1;
}

// Implementation of public functions
void keypad_init(void) 
{
	keypress_flag = 1;
	// Activate the clocks
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// Initialize 4 inputs and 4 outputs
	GPIO_InitTypeDef GPIO_InitStruct;	
	GPIO_InitStruct.Pin			= GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
	GPIO_InitStruct.Mode		= GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode		= GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pin			= GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
	GPIO_InitStruct.Pull 		= GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
	
	enable_keypad_interrupt();
}

int keypad_read_char(char * c)
{
	disable_keypad_interrupt();
	(*c) = internal_read_char();
	enable_keypad_interrupt();
	return (*c) == 0 ? 1 : 0;
}

char internal_read_char()
{
	HAL_GPIO_WritePin(R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		return '1';
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		return '2';
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		return '3';
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		return 'a';
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		return '4';
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		return '5';
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		return '6';
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		return 'b';
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		return '7';
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		return '8';
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		return '9';
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		return 'c';
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		return '*';
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		return '0';
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{  
		return '#';
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		return 'd';
	}
	return 0;
}
