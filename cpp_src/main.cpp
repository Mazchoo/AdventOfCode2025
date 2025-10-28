#include <iostream>
#include <iomanip>
#include "mod.h"

// Main function for testing and debugging the math functions natively
int main() {
    std::cout << "=== Math Library Native Testing ===" << std::endl;
    std::cout << std::endl;

    // Test addition
    std::cout << "Testing Addition:" << std::endl;
    int a = 5, b = 3;
    int addResult = add(a, b);
    std::cout << "add(" << a << ", " << b << ") = " << addResult << std::endl;
    std::cout << std::endl;

    // Test multiplication
    std::cout << "Testing Multiplication:" << std::endl;
    int c = 4, d = 7;
    int multiplyResult = multiply(c, d);
    std::cout << "multiply(" << c << ", " << d << ") = " << multiplyResult << std::endl;
    std::cout << std::endl;

    // Test factorial
    std::cout << "Testing Factorial:" << std::endl;
    for (int i = 0; i <= 10; i++) {
        int factResult = factorial(i);
        std::cout << "factorial(" << i << ") = " << factResult << std::endl;
    }
    std::cout << std::endl;

    // Test power
    std::cout << "Testing Power:" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    double base = 2.5;
    for (int exp = 0; exp <= 5; exp++) {
        double powerResult = power(base, exp);
        std::cout << "power(" << base << ", " << exp << ") = " << powerResult << std::endl;
    }
    std::cout << std::endl;

    // Interactive testing
    std::cout << "=== Interactive Testing ===" << std::endl;
    std::cout << "You can set breakpoints here to debug the functions:" << std::endl;
    
    // Set breakpoints on these lines to debug individual functions
    int debugAdd = add(10, 20);
    int debugMultiply = multiply(6, 8);
    int debugFactorial = factorial(5);
    double debugPower = power(3.0, 4);
    
    std::cout << "Debug results:" << std::endl;
    std::cout << "Debug add: " << debugAdd << std::endl;
    std::cout << "Debug multiply: " << debugMultiply << std::endl;
    std::cout << "Debug factorial: " << debugFactorial << std::endl;
    std::cout << "Debug power: " << debugPower << std::endl;

    std::cout << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}