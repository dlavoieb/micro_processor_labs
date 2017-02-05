#include <stdio.h>
#include "arm_math.h"
#include <stdlib.h>
#include "main.h"
#include <math.h>

#define COEFFS_LENGTH 5
#define DATA_LENGTH 10

//CMSIS Defines
#define NUM_TAPS COEFFS_LENGTH
#define BLOCK_SIZE 32


static float32_t data_demo[DATA_LENGTH] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
static float32_t output[DATA_LENGTH];
static float32_t output_asm[DATA_LENGTH];
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
        data[i] = data_demo[COEFFS_LENGTH - i];
    }

    // ==== PART A ====

    //-----------------------------
    //---Custom C implementation---
    //-----------------------------
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
    FIR_asm(&data[1], coeffs, output_asm, COEFFS_LENGTH, DATA_LENGTH);
    printf("ASM implementation: ");
    print_output(output_asm, DATA_LENGTH);

    //--------------------------
    //---CMSIS Implementation---
    //--------------------------

    // Initialize input and output buffer pointers
    inputF32 = &data[5];
    outputF32 = &output[0];

    // Call FIR init function to initialize the instance structure.
    arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&coeffs[0], &firStateF32[0], blockSize);

    // Call the FIR process function for every blockSize samples
    for(int i=0; i < numBlocks; i++) {
      arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
    }

    print_output(outputF32, DATA_LENGTH);


    // ==== Part b ====

    float32_t difference[DATA_LENGTH];
    float32_t avg;

    // -------------------------------
    // --- Custom C Implementation ---
    // -------------------------------

    // Subtraction of original data stream and data obtained by filter tracking.
    datastream_substraction(&data[COEFFS_LENGTH], output, difference, DATA_LENGTH);
    print_data(difference, DATA_LENGTH);

    // Calculates the standard deviation and the average for for the difference.
    float32_t std_dev = std_deviation(difference, &avg, DATA_LENGTH);
    printf("STD DEVIATION: %f\nAVERAGE: %f\n", std_dev, avg);


    // Calculates the correlation between original and tracked vectors.
    float32_t correlation_data[DATA_LENGTH * 2 - 1];
    correlation(correlation_data, &data[COEFFS_LENGTH], output, DATA_LENGTH);
    printf("Calculate correlation :");
    print_output(correlation_data, DATA_LENGTH);

    // ---------------------------
    // --- CMSIS DATA ANALYSIS ---
    // ---------------------------

    arm_sub_f32 (&data[COEFFS_LENGTH], output, difference, DATA_LENGTH);
    printf("Difference from CMSIS: ");
    print_output(difference, DATA_LENGTH);

    arm_std_f32 (difference, DATA_LENGTH, &std_dev);
    arm_mean_f32 (difference, DATA_LENGTH, &avg);
    printf("STD DEVIATION: %f\nAVERAGE: %f\n", std_dev, avg);

    arm_correlate_f32 (&data[COEFFS_LENGTH], DATA_LENGTH, output, DATA_LENGTH, difference);
    printf("Correlation from CMSIS: ");
    print_output(difference, DATA_LENGTH);

    while(1);
}


/**
 * @brief Printing helper for data.
 */
void print_data(float32_t * data, size_t length) {
    printf("Data array: \n[");
    for (int i = 0; i < length; i++) {
        printf("%f, ", data[i]);
    }
    printf("]\n");
}


/**
 * @brief Printing helper for output.
 */
void print_output(float32_t* output, size_t length) {
    printf("Output values: \n[");
    for (int i = 0; i < length; i++) {
        printf("%f, ", output[i]);
    }
    printf("]\n");
}


/**
 * @brief Compute the weighted average of a data set.
 */
float32_t average(float * data, float * coeffs, size_t length) {
    float prod_sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        prod_sum += data[i] * coeffs[i];
    }
    return prod_sum;
}


/**
 * @brief Set the output to the vectorized subtraction of data_two from data_one.
 */
void datastream_substraction(float32_t * data_one, float32_t * data_two, float32_t * output, size_t length) {
    for (int i = 0; i < length; i++) {
        output[i] = data_one[i] - data_two[i];
    }
}


/**
 * @brief Calculates the average and standard deviation of data set.
 */
float32_t std_deviation(float32_t* data, float32_t* avg, size_t length) {
    // average of dataset
    float32_t sum = 0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    *avg = sum / (float32_t) length;

    float32_t total_deviation;
    for (int i = 0; i < length; i++) {
        total_deviation += ((data[i] - (*avg)) * (data[i] - (*avg)));
    }

    return sqrt(total_deviation/length);
}


/**
 * @brief Calculates the correlation between and data and filtered_data.
 */
void correlation(float32_t * correlated, float32_t * data, float32_t * filtered_data, size_t length) {
    // Phase 1, the data is being overlapped from the left by the filtered data set.
    int index = 0;
    int overlap;
    for (overlap = 1;overlap <= length; overlap++){
        correlated[index] = 0;
        for (int i = 0; i < overlap; i++){
            correlated[index] += data[i] * filtered_data[length - i - 1];
        }
        index++;
    }

    // Phase 2, the overlap between the data set and the filtered data set is diminishing.
    for (;overlap > 0; overlap--) {
        correlated[index] = 0;
        for (int i = 0; i < overlap; i++){
            correlated[index] += filtered_data[i] * data[i + length - overlap];
        }
        index++;
    }
}
