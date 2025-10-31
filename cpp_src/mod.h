#ifndef MATH_H
#define MATH_H

extern "C" {
    // Simple addition function
    int add(int a, int b);
    
    // Simple multiplication function
    int multiply(int a, int b);
    
    // Factorial function
    int factorial(int n);
    
    // Power function
    double power(double base, int exponent);
    
    // Float vector functions for WASM bindings (object-oriented approach)
    // Creates a new float vector of specified size
    void* create_float_vector(int size);
    
    // Frees memory allocated for a float vector
    void free_float_vector(void* vector);
    
    // Sums all elements in a float vector
    float sum_float_vector(void* vector);
    
    // Sets a value at specified index in float vector
    int set_vector_element(void* vector, int index, float value);
    
    // Gets a value at specified index in float vector
    float get_vector_element(void* vector, int index);
    
    // Raw float array functions for WASM bindings (performance comparison)
    // Creates a new float array of specified size
    float* create_float_array(int size);
    
    // Frees memory allocated for a float array
    void free_float_array(float* ptr);
    
    // Sums all elements in a float array
    float sum_float_array(float* ptr, int size);
    
    // Sets a value at specified index in float array
    int set_array_element(float* ptr, int index, float value);
    
    // Gets a value at specified index in float array
    float get_array_element(float* ptr, int index);
}

#endif // MATH_H