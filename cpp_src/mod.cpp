#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define EXPORT_FUNC EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT_FUNC
#endif

#include <string_view>

#include "src/day00.hpp"
#include "src/day01.hpp"
#include "src/day02.hpp"

extern "C" {
    // Day01 part 2
    EXPORT_FUNC
    uint32_t count_repeated_digits(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day02::day02_pt1(payload);
    }

    // Day01 part 2
    EXPORT_FUNC
    uint32_t count_passing_zeros(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day01::day01_pt2(payload);
    }

    // Day01 part 1
    EXPORT_FUNC
    uint32_t count_total_zeros(const char* payload_ptr, int payload_len) {
        std::string_view payload(payload_ptr, payload_len);
        return day01::day01_pt1(payload);
    }

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
    
    // Raw byte array functions for WASM bindings (uint8_t operations)
    
    // Creates a new byte array of specified size
    EXPORT_FUNC
    uint8_t* create_byte_array(int size) {
        return day00::create_byte_array(size);
    }
    
    // Frees memory allocated for a byte array
    EXPORT_FUNC
    void free_byte_array(uint8_t* ptr) {
        day00::free_byte_array(ptr);
    }
    
    // Sums all elements in a byte array (returns int to avoid overflow)
    EXPORT_FUNC
    int sum_byte_array(uint8_t* ptr, int size) {
        return day00::sum_byte_array(ptr, size);
    }

    // SIMD-optimized version of sum_byte_array (WASM only)
    EXPORT_FUNC
    int sum_byte_array_simd(uint8_t* ptr, int size) {
        return day00::sum_byte_array_simd(ptr, size);
    }
    
    // Sets a value at specified index in byte array
    // Returns 1 for success, 0 for failure
    EXPORT_FUNC
    int set_byte_array_element(uint8_t* ptr, int index, uint8_t value) {
        return day00::set_byte_array_element(ptr, index, value) ? 1 : 0;
    }
    
    // Gets a value at specified index in byte array
    EXPORT_FUNC
    uint8_t get_byte_array_element(uint8_t* ptr, int index) {
        return day00::get_byte_array_element(ptr, index);
    }
}