// Day06
#include <vector>
#include <string_view>
#include <cstdint>

namespace day06
{
    // Enum for operations
    enum class Operation {
        Add,      // +
        Multiply  // *
    };

    // Type alias for the return type of parse_values_and_operations
    using ValuesAndOperations = std::pair<std::vector<std::vector<uint64_t>>, std::vector<Operation>>;

    // Parses input containing rows of values and a final row of operations
    // Format:
    //   123 328  51 64 
    //    45 64  387 23 
    //     6 98  215 314
    //   *   +   *   +  
    // Returns: pair of (vector of value rows, vector of operations)
    ValuesAndOperations parse_values_and_operations(std::string_view payload) {
        std::vector<std::vector<uint64_t>> values;
        std::vector<Operation> operations;
        
        std::vector<uint64_t> current_row;
        uint64_t current_number = 0;
        bool has_number = false;
        bool parsing_operations = false;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            // Skip whitespace except newlines
            if (c == ' ' || c == '\t' || c == '\r') {
                // If we have a number, complete it
                if (has_number && !parsing_operations) {
                    current_row.push_back(current_number);
                    current_number = 0;
                    has_number = false;
                }
                continue;
            }
            
            if (c >= '0' && c <= '9') {
                // Building a number
                current_number = current_number * 10 + (c - '0');
                has_number = true;
            } else if (c == '+') {
                // Addition operation
                parsing_operations = true;
                operations.push_back(Operation::Add);
            } else if (c == '*') {
                // Multiplication operation
                parsing_operations = true;
                operations.push_back(Operation::Multiply);
            } else if (c == '\n') {
                // End of line
                if (has_number && !parsing_operations) {
                    // Complete the current number
                    current_row.push_back(current_number);
                    current_number = 0;
                    has_number = false;
                }
                
                // If we have values in current row, save it
                if (!current_row.empty()) {
                    values.push_back(current_row);
                    current_row.clear();
                }
            }
        }
        
        // Handle the last number if we ended while parsing
        if (has_number && !parsing_operations) {
            current_row.push_back(current_number);
        }
        
        // Handle the last row if it wasn't empty
        if (!current_row.empty()) {
            values.push_back(current_row);
        }
        
        return {values, operations};
    }

    // Total the result of every sum
    uint64_t calculate_homework_total(std::string_view payload) {
        auto values_and_ops = parse_values_and_operations(payload);
        auto values = values_and_ops.first;
        size_t nr_values = values.size();
        auto operations = values_and_ops.second;

        uint64_t result = 0;
        for (size_t i = 0; i < operations.size(); i++) {
            uint64_t sub_total = 0;
            if (operations[i] == Operation::Multiply)
                sub_total = 1;

            for (size_t j = 0; j < nr_values; j++) {
                // Assume all same length
                if (operations[i] == Operation::Add) {
                    sub_total += values[j][i];
                } else {
                    sub_total *= values[j][i];
                }
            }

            result += sub_total;
        }

        return result;
    }
}