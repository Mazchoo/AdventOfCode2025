#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "mod.h"
#include "src/day10.hpp"

// Main function for testing and debugging the math functions natively
int main() {
    // Read the full input file
    std::ifstream file("../../python_src/day10/input/full.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file python_src/day10/input/full.txt" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();
    file.close();

    // Run fewest_presses_to_configuration
    std::cout << "Running fewest_presses_to_configuration on full input..." << std::endl;
    uint32_t result = day10::fewest_presses_to_configuration(input);
    std::cout << "Result: " << result << std::endl;

    std::cout << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
