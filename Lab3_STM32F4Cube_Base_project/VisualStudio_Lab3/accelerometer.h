/**
 * @file	accelerometer.h
 * @author	Malcolm Watt
 * @version	0.0.2
 * @date	2017-02-24
 * @brief	Configuration and implementation of ISR for accelerometer data.
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_
#define ACC_BUFFER_LEN 3

#include "LIS3DSH.h"
#include "stm32f4xx_hal.h"

float accelerometer_data_buffer[ACC_BUFFER_LEN];


/**
 * @author	Malcolm Watt
 * @brief	Initialize the LIS3DSH accelerometer, interupts, and GPIO.
 * @description	This function is the only function that needs to be called externally to setup the accelerometer
 *				for use with interrupts. The interrupt service routine is implemented in: PLACEHOLDER.
 */
void accelerometer_init(void);


/**
 * @author	Malcolm Watt
 * @brief	Interrupt handler on external line 0 for the accelerometer.
 */
void EXTI0_IRQHandler(void);


/**
 * @author	Malcolm Watt
 * @brief	Configure the LIS3DSH accelerometer.
 * @description	Internally calls the LIS3DSH library functin for setting up the accelerometer. The main concern
 *				for this is the data rate, which is set to 25 Hz.
 */
void sensor_config(void);


/**
 * @author	Malcolm Watt
 * @brief	Configure the generation of interupts by the accelerometer.
 * @description	Internally calls the LIS3DSH library function for setting up the accelerometer. The setup
 *				makes the interrupt signals active high and non-latched.
 */
void interrupt_generation_config(void);


/**
 * @author	Malcolm Watt
 * @brief	Set up the hardwired E0 GPIO for interupts. Explicitly reset the conflicting A0 GPIO.
 */
void interrupt_GPIO_config(void);


/**
 * @author	Malcolm Watt
 * @brief	Clear the interupt line and enable the ISQ.
 */
void enable_accelerometer_interrupt(void);

#endif // !ACCELEROMETER_H_