#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "LIS3DSH.h"
#include "keypad.h"
#include "clock_config.h"
#include "main.h"
#include "timer.h"
#include "display.h"
#include "led.h"
#include "accelerometer.h"

extern volatile uint8_t keypress_flag;
extern volatile uint8_t timer2_flag;
extern volatile uint8_t display_timer_flag;
extern volatile uint8_t accelerometer_flag;
extern struct AccelerometerAngles accelerometer_angles;

uint16_t read_char;
int16_t target_pitch;
int16_t target_roll;
int16_t temp_pitch;
int16_t temp_roll;
int16_t current_pitch;
int16_t current_roll;

int16_t display_value;
DigitNumber digit;
char c;
char suffix;
uint8_t intensity = 0;

int main(void)
{	
	HAL_Init();
	SystemClock_Config();
	keypad_init();
	init_timers();
	gpio_led_init();
	init_pwm_led();
	accelerometer_init();

	while (1) {
		if (timer2_flag == 1)
		{
			timer2_flag = 0;
			// this is the abort entry timer
			app_state = ABORT;
			keypress_flag = 1;
		}
		
		if (display_timer_flag == 1)
		{
			display_timer_flag = 0;
			switch (app_state)
			{
			case ROLL:
				display_value = temp_roll;
				suffix = 'R';
				break;
			case PITCH:
				display_value = temp_pitch;
				suffix = 'P';
				break;
			default:
				display_value = -1;
				suffix = '-';
				break;
			}
			
			display_angle(display_value, suffix, digit);
			digit = (digit + 1) % 4;
		}
		
		if (keypress_flag == 1)
		{
			stop_abort_timer();
			read_char = keypad_read_char(&c);
						
			switch (app_state)
			{
			case INIT:
				// Store initial values
				temp_pitch = 0;
				temp_roll = 0;
				target_pitch = 0;
				target_roll = 0;
				app_state = WAIT;
				break;
				
			case DONE:
				// apply new data to comparators
				target_pitch = temp_pitch;
				target_roll = temp_roll;
				temp_pitch = 0;
				temp_roll = 0;
				app_state = WAIT;
				break;
					
			case WAIT:
				if (read_char == 1)	
					break;
				else 
					app_state = ROLL;
			case ROLL:
				if (read_char == 0)
				{
					if ('0' <= c && c <= '9')
					{
						temp_roll = (temp_roll * 10) + (c - '0');
					}
					else if (c == '#')
					{
						app_state = PITCH;
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
				if (read_char == 0)
				{
						
					if ('0' <= c && c <= '9')
					{
						temp_pitch = (temp_pitch * 10) + (c - '0');
					}
					else if (c == '#')
					{
						app_state = DONE;
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
				app_state = WAIT;
				break;
			}
			
			keypress_flag = 0;	
		}
		
		if (accelerometer_flag == 1)
		{
			accelerometer_flag = 0;
			
			float deg_to_rad = 360.0 / (2 * PI);
			
			current_roll = (int16_t) (accelerometer_angles.roll * deg_to_rad);
			current_pitch = (int16_t) (accelerometer_angles.pitch * deg_to_rad);
			
			int16_t roll_intensity = (target_roll - current_roll); //absolute difference (make relative)
			roll_intensity = roll_intensity > 0 ? roll_intensity : -roll_intensity;
			
			int16_t pitch_intensity = (target_pitch - current_pitch); //absolute difference (make relative)
			pitch_intensity = pitch_intensity > 0 ? pitch_intensity : -pitch_intensity;
			
			set_duty_cycle_percent(roll_intensity, LED_GREEN);
			set_duty_cycle_percent(roll_intensity, LED_RED);
			set_duty_cycle_percent(pitch_intensity, LED_BLUE);
			set_duty_cycle_percent(pitch_intensity, LED_ORANGE);
			
		}
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	display_timer_flag = 1;
	accelerometer_flag = 1; // todo: REMOVE ACCEL FLAG
}