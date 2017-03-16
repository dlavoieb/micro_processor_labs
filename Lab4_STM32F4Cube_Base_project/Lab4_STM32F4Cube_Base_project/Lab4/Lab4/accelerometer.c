/**
 * @file	accelerometer.c
 * @author	Malcolm Watt
 * @version	0.0.3
 * @date	2017-03-09
 * @brief	Configuration and implementation of ISR for accelerometer data.
 */

#include "accelerometer.h"


uint8_t accelerometer_flag = 0;
struct RawAccelerometerData raw_accelerometer_data_buffer[5];
uint8_t buffer_position = 0;

void accelerometer_init(void)
{
	// Sets the data rate for the accelerometer.
	sensor_config();
	
	// Sets up interrupt type for the accelerometer.
	interrupt_generation_config();
	
	// Configure GPIOs for interrupts.
	interrupt_GPIO_config();
	
	// Set the priority of the Line 0 interrupt to 1 with subgroup priority of 0.
	// Note that the keypad has a subgroup priority of 1.
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	
	enable_accelerometer_interrupt();
}


void EXTI0_IRQHandler(void)
{
	// Clear the interrupt pin.
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	
	float accelerometer_data_buffer[ACC_BUFFER_LEN];

	// Read out the values from the accelerometer.
	LIS3DSH_ReadACC(&accelerometer_data_buffer[0]);
	
	// Place the aquired data in global buffer.
	raw_accelerometer_data_buffer[buffer_position].x = accelerometer_data_buffer[0];
	raw_accelerometer_data_buffer[buffer_position].y = accelerometer_data_buffer[1];
	raw_accelerometer_data_buffer[buffer_position].z = accelerometer_data_buffer[2];
	
	// Transform based on calibration data.
	//adjust_accelerometer_angle(&accelerometer_data_buffer[0]);
	
	// Calculate the tilt and roll angles of the accelerometer.
	//accelerometer_angle_calculation(&accelerometer_data_buffer[0], &angle_buffer[buffer_position]);

	// Update the position of the next entry;
	buffer_position = (buffer_position + 1) % 5;

	// Set the accelerometer flag high.
	accelerometer_flag = 1;
}


void sensor_config(void)
{
	// Generate a LIS3DSH init construct, and set the appropriate values for our application.
	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;

	// Set data rate to 100 Hz as this is approximately the refresh rate of our display.
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;

	// This leaves the BDU feature unset. Acceptable since reading the angular data is not a speed bottleneck.
	LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;

	// Enable all three axes of the accelerometer since they are all required for angle determination.
	LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE;

	// Using a 2G ceiling gives us better sensitivity which is desireable for our application.
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2;
	
	// The following two settings are slightly arbitrary.
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_800;
	LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;

	// Call the configuration function for the accelerometer.
	LIS3DSH_Init(&LIS3DSH_InitStruct);
}


void interrupt_generation_config(void)
{
	// Initialise the interupt configuration struct.
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfig;
	
	// Enable interupts.
	LIS3DSH_InterruptConfig.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	
	// Make the interupt signal active high.
	LIS3DSH_InterruptConfig.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	
	// Use pulsed interupt signals.
	LIS3DSH_InterruptConfig.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	// Call the configuration function for the interupts.
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfig);
}


void interrupt_GPIO_config(void)
{
	// Explicitly reset port A0 (push button port) to avoid conflicts.
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
	
	// Setup GPIO port E0 for accelerometer interrupts.
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}


void accelerometer_angle_calculation(struct RawAccelerometerData * data, struct AccelerometerAngles * result)
{
	float x_acc = data->x;
	float y_acc = data->y;
	float z_acc = data->z;
	
	
	// Calculate the denominator for pitch: Ay^2 + Az^2.
	float pitch_denom_squared = y_acc * y_acc + z_acc * z_acc;
	
	// Get the square root of the denominator using a cmsis lib call.
	float pitch_denominator;
	arm_sqrt_f32(pitch_denom_squared, &pitch_denominator);
	
	// Calculate the pitch using arctan.
	float temp = atan2(x_acc, pitch_denominator);
	result->pitch = temp;
	
	
	// Calculate the denominator for pitch: Ax^2 + Az^2.
	float roll_denom_squared = x_acc * x_acc + z_acc * z_acc;
	
	// Get the square root of the denominator using a cmsis lib call.
	float roll_denominator;
	arm_sqrt_f32(roll_denom_squared, &roll_denominator);
	
	// Calculate the roll using arctan.
	result->roll = atan2f(y_acc, roll_denominator);
}


void adjust_accelerometer_angle(struct RawAccelerometerData * data)
{
	// Raw data.
	float Ax = data->x;
	float Ay = data->y;
	float Az = data->z;
	
	/* 
	 * Note: The following equations can be found in AN3182
	 * "Tilt measurement using a low-g 3-axis accelerometer".
	 */
	
	// Corrected and normalized data.
	float Ax1 = ACC_CALIB_11 * Ax + ACC_CALIB_12 * Ay + ACC_CALIB_13 * Az + ACC_CALIB_10;
	float Ay1 = ACC_CALIB_21 * Ax + ACC_CALIB_22 * Ay + ACC_CALIB_23 * Az + ACC_CALIB_20;
	float Az1 = ACC_CALIB_31 * Ax + ACC_CALIB_32 * Ay + ACC_CALIB_33 * Az + ACC_CALIB_30;
	
	// Replace the raw data with the normalized values.
	data->x = Ax1;
	data->y = Ay1;
	data->z = Az1;
}


void enable_accelerometer_interrupt(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


void disable_accelerometer_interrupt(void)
{
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}