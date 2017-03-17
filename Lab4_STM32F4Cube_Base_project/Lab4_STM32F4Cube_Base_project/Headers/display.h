/**
 * @file		display.h
 * @author(s)	Malcolm Watt
 * @brief		Contains the thread handler for everything display related on the uP.
 */

#ifndef _DISPLAY_MODULE
#define _DISPLAY_MODULE

#include "cmsis_os.h"
#include "accelerometer.h"

int start_display_thread(void);

void thread_display(void const * argument);

#endif // !_DISPLAY_MODULE
