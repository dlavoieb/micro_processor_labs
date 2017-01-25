#include <stdio.h>
#include "arm_math.h"
#include <stdlib.h>
#include "main.h"

#define COEFFS_LENGTH 5
#define DATA_LENGTH 1000

int main()
{
	float * data = (float *) malloc((DATA_LENGTH + COEFFS_LENGTH)* sizeof(float32_t));
	float * output = (float *) malloc(DATA_LENGTH * sizeof(float32_t));
	
	// pad data array with 0, note first 0 is never used
	for (int i = 0; i<COEFFS_LENGTH; i++) {
		data[i] = 0;
	}
	// create data values
	for (int i = COEFFS_LENGTH; i<DATA_LENGTH + COEFFS_LENGTH; i++) {
		data[i] = i;
	}
	float coeffs[COEFFS_LENGTH] = {0.1, 0.15, 0.5, 0.15, 0.1};
	
	int data_index = COEFFS_LENGTH;
	
	while (data_index < COEFFS_LENGTH + DATA_LENGTH) {
		//build a window
		//compute average for window
		//store average in output array
		output[data_index - COEFFS_LENGTH] = average(&data[data_index - COEFFS_LENGTH + 1], coeffs, COEFFS_LENGTH);
		data_index++;
	}
	
	// print out the results
	printf("Data array: \n[");
	for (int i = 0; i < COEFFS_LENGTH + DATA_LENGTH; i++) {
		printf("%f, ", data[i]);
	}
	printf("]\nOutput values: \n[");
	for (int i = 0; i < DATA_LENGTH; i++) {
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
