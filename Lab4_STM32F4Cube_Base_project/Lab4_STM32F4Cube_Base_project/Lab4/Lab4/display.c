#include "display.h"

extern uint8_t accelerometer_flag;
extern struct RawAccelerometerData raw_accelerometer_data_buffer[];
float to_display;

void _display_loop(void)
{
	// First start the flashing thread.
	
	// Then we have a continuous loop which displays based on whether the `hazard` global is set.
	
	// What we display is based on the current mode, if we are in the temperature mode, we display temp.
	// If we are in accelerometer mode, we filter and display the temperature.
		
	to_display = 0.0;
	
	while (1)
	{
		//if flash?
		/*if (accelerometer_flag == 1)
		{
			int x = 1;
		}*/
		osDelay(50);
	}
}
