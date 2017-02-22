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

// Private Functions
void enable_keypad_interrupt(void)
{
	// Drive outputs high
	HAL_GPIO_WritePin(R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_SET);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	
}

void disable_keypad_interrupt(void)
{
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);	
}

// Interrupt handler
void EXTI9_5_IRQHandler(void) {
	// key pressed, ask for key read	
}

// Implementation of public functions
void keypad_init(void) 
{
	// Activate the clocks
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// Initialize 4 inputs and 4 outputs
	GPIO_InitTypeDef GPIO_InitStruct;	
	GPIO_InitStruct.Mode		= GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pin			= GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_MEDIUM;
	GPIO_InitStruct.Pull 		= GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Mode		= GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
	enable_keypad_interrupt();
}

int keypad_read_char(char * c)
{
	disable_keypad_interrupt();
	HAL_GPIO_WritePin(R1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		(*c) = '1';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		(*c) = '2';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		(*c) = '3';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		(*c) = 'a';
		return 0;
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		(*c) = '4';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		(*c) = '5';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		(*c) = '6';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		(*c) = 'b';
		return 0;
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		(*c) = '7';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		(*c) = '8';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{
		(*c) = '9';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		(*c) = 'c';
		return 0;
	}
	
	HAL_GPIO_WritePin(R1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R4, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(C1) == 1)
	{
		(*c) = '*';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C2) == 1)
	{
		(*c) = '0';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C3) == 1)
	{  
		(*c) = '#';
		return 0;
	} 
	else if (HAL_GPIO_ReadPin(C4) == 1)
	{
		(*c) = 'd';
		return 0;
	}
	return 1;
	enable_keypad_interrupt();
}
