#include "display.h"
#include "adc.h"

#define PI 3.14159265358979323846
#define deg_to_rad 360.0 / (2 * PI)

extern uint8_t accelerometer_flag;
extern struct AccelerometerAngles angle_buffer[];
extern osMutexId temperature_global_mutex_id;
struct AccelerometerAngles angles;
float to_display;
extern float g_AdcValue;

osThreadId DisplayThreadID;
osThreadDef(thread_display, osPriorityNormal, 1, 0);


int start_display_thread(void)
{
	DisplayThreadID = osThreadCreate(osThread(thread_display), NULL);
	if(!DisplayThreadID) return(-1);
	return(0);
}

void thread_display(void const * argument){
	osEvent accelerometer_event;
	float total_pitch;
	float total_roll;
	int16_t target_pitch = 0;
	int16_t target_roll = 0;
	int16_t current_pitch;
	int16_t current_roll;
	int16_t roll_intensity = 0;
	int16_t pitch_intensity = 0;
	int i;
	float local_adc_value;
	osStatus temperature_mutex_status;
	
	enable_accelerometer_interrupt();
	
	while (1){
		/*
		 * Temperature Reading
		 */
		temperature_mutex_status = osMutexWait(temperature_global_mutex_id, 1);
		if (temperature_mutex_status == osOK)
		{
			local_adc_value = g_AdcValue;
			osMutexRelease(temperature_global_mutex_id);
		}		
		
		/*
		 * Keypad code placeholder.
		 */
		
		/*
		 * Get values for accelerometer and filter them if necessary.
		 */
		accelerometer_event = osSignalWait(0x0001, 0);
		if (accelerometer_event.status == osEventSignal)
		{
			// Filter the results.
			total_pitch = 0;
			total_roll = 0;
			
			for (i = 0; i < 5; i++)
			{
				total_roll += angle_buffer[i].roll;
				total_pitch += angle_buffer[i].pitch;
			}
			
			accelerometer_flag = 0;
			
			current_roll = (int16_t)(total_roll * deg_to_rad / 5);
			current_roll = current_roll > 0 ? current_roll : -current_roll;
			current_pitch = (int16_t)(total_pitch * deg_to_rad / 5);
			current_pitch = current_pitch > 0 ? current_pitch : -current_pitch;
			
			roll_intensity = (target_roll - current_roll); //absolute difference (make relative)
			roll_intensity = roll_intensity > 0 ? roll_intensity : -roll_intensity;
			
			pitch_intensity = (target_pitch - current_pitch); //absolute difference (make relative)
			pitch_intensity = pitch_intensity > 0 ? pitch_intensity : -pitch_intensity;
			
			osSignalClear(DisplayThreadID, 0x0001);
			enable_accelerometer_interrupt();
		}
		
		set_duty_cycle_percent(pitch_intensity, LED_GREEN);
		set_duty_cycle_percent(pitch_intensity, LED_RED);
		set_duty_cycle_percent(roll_intensity, LED_BLUE);
		set_duty_cycle_percent(roll_intensity, LED_ORANGE);
		
		osDelay(40);
	}
}

// private function definitions
void select_digit(DigitNumber digit);
void led_char(char character, uint8_t dot);

void display_temperature(float temperature, DisplayUnits units, DigitNumber  digit)
{
	char display_caracter;
	uint8_t dot = 0;
	select_digit(digit);
	if (temperature < 0)
	{
		display_caracter = '#';
		digit = -1;
	}
	if (temperature < 100)
	{
		switch (digit)
		{
		case DIGIT_1:
			// first symbol:
			display_caracter = '0' + (((uint32_t) temperature) / 10);
			break;
		case DIGIT_2:
			//second symbol:
			display_caracter = '0' + (((uint32_t) temperature) % 10);
			dot = 1;
			break;
		case DIGIT_3:
			//thrird sybmol:
			display_caracter = '0' + (((uint32_t)(temperature * 10.0)) % 10);
			break;
		case DIGIT_4:
			//unit symbol
			display_caracter = (units == CELCIUS_UNITS) ? 'C' : 'F'; // If not celcius, assume it's farenheit
			break;
		default:
			break;
		}
	}
	else
	{
		switch (digit)
		{
		case DIGIT_1:
			// first symbol:
			display_caracter = '0' + (((uint32_t) temperature) / 100);
			break;
		case DIGIT_2:
			//second symbol:
			display_caracter = '0' + ((((uint32_t) temperature) / 10) % 10);
			break;
		case DIGIT_3:
			//thrird sybmol:
			display_caracter = '0' + (((uint32_t)(temperature)) % 10);
			break;
		case DIGIT_4:
			//unit symbol
			display_caracter = (units == CELCIUS_UNITS) ? 'C' : 'F'; // If not celcius, assume it's farenheit
			break;
		default:
			break;		
		}
	}
	led_char(display_caracter, dot);
}

void gpio_led_init(void) {
	GPIO_InitTypeDef GPIO_InitDef; 								
	__HAL_RCC_GPIOD_CLK_ENABLE(); 									
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	GPIO_InitDef.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8; 	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11; 	
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12; 	
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
		
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_11 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12, GPIO_PIN_RESET);
}

void select_digit(DigitNumber digit) 
{
	switch (digit)
	{
	case DIGIT_1:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);	// DIGIT 1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);	// DIGIT 2
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);	// DIGIT 3
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);	// DIGIT 4
		break;
	case DIGIT_2:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
		break;
	case DIGIT_3:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
		break;
	case DIGIT_4:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
}

void led_char(char character, uint8_t dot) {
	switch (character) {

		// DISPLAY NUMBER 0
	case '0':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	| |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
		break;

			// DISPLAY NUMBER 1
	case '1':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);                  // A
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	  |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	  |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
		break;

			// DISPLAY NUMBER 2
	case '2':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	 _|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);                   // C	|_
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

		      // DISPLAY NUMBER 3
	case '3':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	 _|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	 _|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

			// DISPLAY NUMBER 4
	case '4':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);                  // A	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	  |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

		     // DISPLAY NUMBER 5
	case '5':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);                  // B	|_
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	 _|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

		      // DISPLAY NUMBER 6
	case '6':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);                  // B	|_
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

		      // DISPLAY NUMBER 7
	case '7':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	  |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	  |
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);                   // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                  // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);                  // G
		break;

		      // DISPLAY NUMBER 8
	case '8':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);                     // D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);                    // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;

		      // DISPLAY NUMBER 9
	case '9':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);                    // A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);                    // B	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);                     // C	 _|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);						// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);                  // E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                    // F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);                    // G
		break;
	    
	case 'F':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);					// A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);					// B	|_
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C	|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);					// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);					// G
		break;
		
	case 'C':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);					// A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);					// B	|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C	|_
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);						// D	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);					// G
		break;
    
	case 'P':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);					// A	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);					// B	|_|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C	|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);					// D	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);					// G
		break;

	case 'R':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);					// A	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);					// B	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C	|
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);					// D	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);					// G
		break;
	    
	case '-':
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);					// A	 
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);					// B	 _
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);					// D	
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);					// G
		break;

	default:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);					// A
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);					// B
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);					// C
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);					// D
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);					// E
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);					// F
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);					// G
		break;
	}

	    // DISPLAY DOT
	if (dot == 1) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);                    // LOWER DOT
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);                  // UPPER DOT
	}
	else if (dot == 2) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                  // LOWER DOT
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);                    // UPPER DOT
	}
	else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);                  // LOWER DOT
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);                  // UPPER DOT
	}
}


void display_angle(int16_t angle, char suffix, DigitNumber digit)
{
	char display_caracter;
	select_digit(digit);
	if (angle < 0)
	{
		display_caracter = '-';
		digit = -1;
	}
	switch (digit)
	{
	case DIGIT_1:
		// first symbol:
		display_caracter = '0' + (((uint32_t) angle) / 100);
		break;
	case DIGIT_2:
		//second symbol:
		display_caracter = '0' + ((((uint32_t) angle) / 10) % 10);
		break;
	case DIGIT_3:
		//thrird sybmol:
		display_caracter = '0' + (((uint32_t)(angle)) % 10);
		break;
	case DIGIT_4:
		//unit symbol
		display_caracter = suffix;
		break;
	default:
		break;		
	}
	
	led_char(display_caracter, 0);
}
