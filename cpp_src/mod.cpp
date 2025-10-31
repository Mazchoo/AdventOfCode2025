#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define EXPORT_FUNC EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT_FUNC
#endif

#include "src/day00.hpp"

extern "C" {
    // Simple addition function
    EXPORT_FUNC
    int add(int a, int b) {
        return day00::add(a, b);
    }
    
    // Simple multiplication function
    EXPORT_FUNC
    int multiply(int a, int b) {
        return a * b;
    }
    
    // Factorial function
    EXPORT_FUNC
    int factorial(int n) {
        if (n <= 1) {
            return 1;
        }
        return n * factorial(n - 1);
    }
    
    // Power function
    EXPORT_FUNC
    double power(double base, int exponent) {
        double result = 1.0;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }
    
    // Float vector functions for WASM bindings
    
    // Creates a new float vector of specified size
    EXPORT_FUNC
    void* create_float_vector(int size) {
        return static_cast<void*>(day00::create_float_vector(size));
    }
    
    // Frees memory allocated for a float vector
    EXPORT_FUNC
    void free_float_vector(void* vector) {
        day00::free_float_vector(static_cast<day00::FloatVector*>(vector));
    }
    
    // Sums all elements in a float vector
    EXPORT_FUNC
    float sum_float_vector(void* vector) {
        return day00::sum_float_vector(static_cast<day00::FloatVector*>(vector));
    }
    
    // Sets a value at specified index in float vector
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_vector_element(void* vector, int index, float value) {
        return day00::set_vector_element(static_cast<day00::FloatVector*>(vector), index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in float vector
    EXPORT_FUNC
    float get_vector_element(void* vector, int index) {
        return day00::get_vector_element(static_cast<day00::FloatVector*>(vector), index);
    }
    
    // Raw float array functions for WASM bindings (performance comparison)
    
    // Creates a new float array of specified size
    EXPORT_FUNC
    float* create_float_array(int size) {
        return day00::create_float_array(size);
    }
    
    // Frees memory allocated for a float array
    EXPORT_FUNC
    void free_float_array(float* ptr) {
        day00::free_float_array(ptr);
    }
    
    // Sums all elements in a float array
    EXPORT_FUNC
    float sum_float_array(float* ptr, int size) {
        return day00::sum_float_array(ptr, size);
    }
    
    // Sets a value at specified index in float array
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_array_element(float* ptr, int index, float value) {
        return day00::set_array_element(ptr, index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in float array
    EXPORT_FUNC
    float get_array_element(float* ptr, int index) {
        return day00::get_array_element(ptr, index);
    }
}