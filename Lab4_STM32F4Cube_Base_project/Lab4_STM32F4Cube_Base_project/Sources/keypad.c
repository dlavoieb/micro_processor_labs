#include "keypad.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "timer.h"

#define R1 GPIOB, GPIO_PIN_6
#define R2 GPIOB, GPIO_PIN_7
#define R3 GPIOB, GPIO_PIN_8
#define R4 GPIOB, GPIO_PIN_9
#define C1 GPIOC, GPIO_PIN_6
#define C2 GPIOC, GPIO_PIN_7
#define C3 GPIOC, GPIO_PIN_8
#define C4 GPIOC, GPIO_PIN_9

extern struct AppState appState;
extern volatile uint8_t timer2_flag;
extern osMutexId app_state_mutex_id;

void keypad_main_thread(void const * arguments);

osThreadId(KeypadThreadID);
osThreadDef(keypad_main_thread, osPriorityNormal, 1, 0);

enum KeypadState
{
	INIT,
	DONE,
	ROLL,
	PITCH,
	WAIT,
	ABORT
} keypad_state;

uint16_t temp_pitch;
uint16_t temp_roll;
uint16_t read_char;
uint16_t prev_read_char;
char c;
// Private Functions
char internal_read_char();

// Implementation of public functions
void keypad_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct;	

	// Activate the clocks
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// Initialize 4 inputs and 4 outputs
	GPIO_InitStruct.Pin			= GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
	GPIO_InitStruct.Mode		= GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode		= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin			= GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
	GPIO_InitStruct.Pull 		= GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	// HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
	
	// enable_keypad_interrupt();
}

int keypad_read_char(char * c)
{
	// disable_keypad_interrupt();
	(*c) = internal_read_char();
	// enable_keypad_interrupt();
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


void keypad_main_thread(void const * arguments)
{
	while(1) 
	{

		if (timer2_flag == 1)
		{
			// abort procedure
			keypad_state = ABORT;
			timer2_flag = 0;
		}

		// read button
		// if button pressed 
		// change state and update temp vars
		// if state done
		// publish new variables
		read_char = keypad_read_char(&c);

		if (read_char != prev_read_char)
		{
			// key change
			stop_abort_timer();		
		}
		
		osMutexWait(app_state_mutex_id, osWaitForever);					
		switch (keypad_state)
		{
		case INIT:
			// Store initial values
			temp_pitch = 0;
			temp_roll = 0;
			appState.target_pitch = 0;
			appState.target_roll = 0;
			keypad_state = WAIT;
			break;
			
		case DONE:
			// apply new data to comparators
			appState.target_pitch = temp_pitch < 90 ? temp_pitch : 90;
			appState.target_roll = temp_roll < 90 ? temp_roll : 90;
			temp_pitch = 0;
			temp_roll = 0;
			keypad_state = WAIT;
			break;
				
		case WAIT:
			appState.display_state = TEMPERATURE;
			if (read_char == 1 && prev_read_char == 0)	
				keypad_state = ROLL;
			else 
				break;
		case ROLL:
			appState.display_state = ANGLE;		
			if (read_char == 0 && prev_read_char == 1)
			{
				if ('0' <= c && c <= '9')
				{
					temp_roll = (temp_roll * 10) + (c - '0');
				}
				else if (c == '#')
				{
					keypad_state = PITCH;
				}
				else if (c == '*')
				{
					float temp = (temp_roll / 10.0);
					temp_roll = (uint16_t) temp;
					// start timer for clear, will timeout if nothing is entered in the next 3 seconds
					start_abort_timer();
				}
			}
			break;
				
		case PITCH:
			if (read_char == 0 && prev_read_char == 1)
			{
					
				if ('0' <= c && c <= '9')
				{
					temp_pitch = (temp_pitch * 10) + (c - '0');
				}
				else if (c == '#')
				{
					keypad_state = DONE;
				}
				else if (c == '*')
				{
					float temp = (temp_pitch / 10.0);
					temp_pitch = (uint16_t) temp;
					// start timer for clear, will timeout if nothing is entered in the next 3 seconds
					start_abort_timer();
				}
			}
			break;
		case ABORT:
			// clear temporary data
			temp_pitch = 0;
			temp_roll = 0;
			keypad_state = WAIT;
			break;
		}
		osMutexRelease(app_state_mutex_id);
		prev_read_char = read_char;
		osDelay(1);
	}
}

int start_keypad_thread2(void){
	KeypadThreadID = osThreadCreate(osThread(keypad_main_thread), NULL);
	if(!KeypadThreadID) return(-1);
	return(0);
}
