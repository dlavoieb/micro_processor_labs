#include "display.h"

extern uint8_t accelerometer_flag;
extern struct RawAccelerometerData raw_accelerometer_data_buffer[];
struct AccelerometerAngles angles;
float to_display;

osThreadId DisplayThreadID;
osThreadDef(thread_display, osPriorityNormal, 1, 0);


int start_display_thread(void)
{
	DisplayThreadID = osThreadCreate(osThread(thread_display), NULL);
	if(!DisplayThreadID) return(-1);
	return(0);
}

void thread_display(void const * argument){
	while (1){
		int x = 1;
	}
}
