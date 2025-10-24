#include <emscripten.h>

extern "C" {
    // Simple addition function
    EMSCRIPTEN_KEEPALIVE
    int add(int a, int b) {
        return a + b;
    }
    
    // Simple multiplication function
    EMSCRIPTEN_KEEPALIVE
    int multiply(int a, int b) {
        return a * b;
    }
    
    // Factorial function
    EMSCRIPTEN_KEEPALIVE
    int factorial(int n) {
        if (n <= 1) {
            return 1;
        }
        return n * factorial(n - 1);
    }
    
    // Power function
    EMSCRIPTEN_KEEPALIVE
    double power(double base, int exponent) {
        double result = 1.0;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }
}