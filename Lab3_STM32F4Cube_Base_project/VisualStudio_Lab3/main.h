#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

enum AppState
{
	INIT,
	DONE,
	ROLL,
	PITCH,
	WAIT,
	ABORT
} app_state;

#endif /* __MAIN_H */
