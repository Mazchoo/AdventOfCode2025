#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define EXPORT_FUNC EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT_FUNC
#endif

extern "C" {
    // Simple addition function
    EXPORT_FUNC
    int add(int a, int b) {
        return a + b;
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
}