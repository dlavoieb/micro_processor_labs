/**
 * @file	accelerometer.c
 * @author	Malcolm Watt
 * @version	0.0.2
 * @date	2017-02-24
 * @brief	Configuration and implementation of ISR for accelerometer data.
 */

#include "accelerometer.h"


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
	
	// Clear interrupt line and enable through NVIC library call.
	enable_accelerometer_interrupt();
}


void EXTI0_IRQHandler(void)
{
	// Clear the interrupt pin.
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	
	// Read out the values from the accelerometer.
	LIS3DSH_ReadACC(&accelerometer_data_buffer[0]);
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
	__HAL_RCC_GPIOE_CLK_ENABLE();

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


void enable_accelerometer_interrupt(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
