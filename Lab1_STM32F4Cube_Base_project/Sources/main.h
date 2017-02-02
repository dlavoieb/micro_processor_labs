float32_t average(float * data, float * coeffs, size_t length);
void print_output(float32_t* output, size_t length);
void print_data(float32_t* data, size_t length);
float32_t std_deviation(float32_t* data, float32_t* avg, size_t length);
void datastream_substraction(float32_t * data_one, float32_t * data_two, float32_t * output, size_t length);

//--- Assembly Functions ---
void AVERAGE_asm(float32_t*, float32_t*, float32_t*, size_t);
void FIR_asm(float32_t*, float32_t*, float32_t*, size_t, size_t);
