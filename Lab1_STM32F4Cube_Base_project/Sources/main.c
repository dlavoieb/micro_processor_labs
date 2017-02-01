#include <stdio.h>
#include "arm_math.h"
#include <stdlib.h>
#include "main.h"

#define COEFFS_LENGTH 5
#define DATA_LENGTH 1000

//CMSIS Defines
#define NUM_TAPS 5
#define BLOCK_SIZE 32 


static float32_t output[DATA_LENGTH];
static float32_t data [DATA_LENGTH + COEFFS_LENGTH];
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
static float coeffs[COEFFS_LENGTH] = {0.1, 0.15, 0.5, 0.15, 0.1};

int main()
{
	// pad data array with 0, note first 0 is never used
	for (int i = 0; i<COEFFS_LENGTH; i++) {
		data[i] = 0;
	}
	// create data values
	for (int i = COEFFS_LENGTH; i<DATA_LENGTH + COEFFS_LENGTH; i++) {
		data[i] = i;
	}
	
	//---------------------------
	//---Custom implementation---
	//---------------------------
	int data_index = COEFFS_LENGTH;	
	while (data_index < COEFFS_LENGTH + DATA_LENGTH) {
		//build a window
		//compute average for window
		//store average in output array
		output[data_index - COEFFS_LENGTH] = average(&data[data_index - COEFFS_LENGTH + 1], coeffs, COEFFS_LENGTH);
		data_index++;
	}
	
	// print out the results
	print_data(data, DATA_LENGTH + COEFFS_LENGTH);
	print_output(output, DATA_LENGTH);
	
	
	//-----------------------------
	//---Assembly Implementation---
	//-----------------------------
//	FIR_asm();
	AVERAGE_asm(&data[1], coeffs, &output[1], COEFFS_LENGTH);
	AVERAGE_asm(&data[3], coeffs, &output[3], COEFFS_LENGTH);
	AVERAGE_asm(&data[5], coeffs, &output[5], COEFFS_LENGTH);
	
	//--------------------------
	//---CMSIS Implementation---
	//--------------------------
  arm_fir_instance_f32 S;
  float32_t  *inputF32, *outputF32;
	uint32_t blockSize = BLOCK_SIZE;
	uint32_t numBlocks = DATA_LENGTH/BLOCK_SIZE;

  /* Initialize input and output buffer pointers */
  inputF32 = &data[5];
  outputF32 = &output[0];

  /* Call FIR init function to initialize the instance structure. */
  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&coeffs[0], &firStateF32[0], blockSize);

  /* ----------------------------------------------------------------------
  ** Call the FIR process function for every blockSize samples
  ** ------------------------------------------------------------------- */

  for(int i=0; i < numBlocks; i++) {
    arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
  }

	print_output(outputF32, DATA_LENGTH);
	
	while(1);
}

void print_data(float32_t * data, size_t length) {
	printf("Data array: \n[");
	for (int i = 0; i < length; i++) {
		printf("%f, ", data[i]);
	}
	printf("]\n");
}

void print_output(float32_t* output, size_t length) {
	printf("Output values: \n[");
	for (int i = 0; i < length; i++) {
		printf("%f, ", output[i]);
	}
	printf("]\n");
}


/**
 * Compute the weighted average of a data set
 */
float32_t average(float * data, float * coeffs, size_t length) {
	float prod_sum = 0;
	int i;
	for (i = 0; i < length; i++) {
		prod_sum += data[i] * coeffs[i];
	}
	return prod_sum;
}
