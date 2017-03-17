/**
 * @file	accelerometer.h
 * @author	Malcolm Watt
 * @version	0.0.3
 * @date	2017-03-09
 * @brief	Configuration and implementation of ISR for accelerometer data.
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

// Buffer length for the accelerometer library call.
#define ACC_BUFFER_LEN 3

// Calibration values for correcting the accelerometer.
#define ACC_CALIB_11 0.0010
#define ACC_CALIB_21 -1.5161E-5
#define ACC_CALIB_31 1.0938E-5
#define ACC_CALIB_12 -1.2551E-5
#define ACC_CALIB_22 9.7793E-4
#define ACC_CALIB_32 -5.8278E-6
#define ACC_CALIB_13 -9.0347E-6
#define ACC_CALIB_23 -3.2451E-6
#define ACC_CALIB_33 9.9294E-4
#define ACC_CALIB_10 -0.0080
#define ACC_CALIB_20 -0.0053
#define ACC_CALIB_30 -0.0249


#include "LIS3DSH.h"
#include "stm32f4xx_hal.h"
#include "math.h"
#include "display.h"


/**
 * Data structure used as a global for getting accelerometer angles in radians.
 */
struct AccelerometerAngles
{
	float pitch;
	float roll;
};

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
 * @brief	Calculate the accelerometer angles.
 */
void accelerometer_angle_calculation(float * xyz_data, struct AccelerometerAngles * result);


/**
 * @author	Malcolm Watt
 * @brief	Adjust the accelerometer angles based on the calibration data.
 * @description	Transforms the input Ax, Ay, Az into the normalized and corrected Ax1, Ay1 and Az1.
 */
void adjust_accelerometer_angle(float * xyz_data);


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

void disable_accelerometer_interrupt(void);

#endif // !ACCELEROMETER_H_
