#include "display.h"

extern uint8_t accelerometer_flag;
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
	osEvent accelerometer_event;
	
	enable_accelerometer_interrupt();
	
	while (1){
		// Get values for accelerometer and filter them if necessary.
		accelerometer_event = osSignalWait(0x0001, 0);
		if (accelerometer_event.status == osEventSignal)
		{
			// Filter the results.
			osSignalClear(DisplayThreadID, 0x0001);
			enable_accelerometer_interrupt();
		}
	}
}
