/**
 * @file		display.h
 * @author(s)	Malcolm Watt
 * @brief		Contains the thread handler for everything display related on the uP.
 */

#ifndef _DISPLAY_MODULE
#define _DISPLAY_MODULE

#include "accelerometer.h"
#include <../CMSIS_RTOS/cmsis_os.h>


void _display_loop(void);

/**
 * @author(s)	Malcolm Wattr
 * @brief		Handles the display based on the curent state of the application which is controlled by a global.
 */
static void display_thread(void const * argument)
{
	_display_loop();
}

#endif // !_DISPLAY_MODULE
