// Day06
#include <vector>
#include <string_view>
#include <cstdint>

namespace day06
{
    // Count the number of digits in a number
    inline int count_digits(uint64_t number) {
        if (number == 0) return 1;
        
        int count = 0;
        while (number > 0) {
            number /= 10;
            count++;
        }
        return count;
    }

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
        if (has_number && !parsing_operations)
            current_row.push_back(current_number);

        // Handle the last row if it wasn't empty
        if (!current_row.empty())
            values.push_back(current_row);
        
        return {values, operations};
    }

    // Get the length of the first line (up to the first newline character)
    size_t get_line_length(std::string_view payload) {
        bool next_space = false;
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            if (c == '\n' || c == '\r') {
                next_space = true;
            } else if (next_space && (payload[i] == ' ' || (c >= '0' && c <= '9'))) {
                return i;
            }
        }
        // If no newline found, return the entire length
        return payload.length();
    }

    // Parses input containing columns of values and a final row of operations
    // Format:
    //   123 328  51 64 
    //    45 64  387 23 
    //     6 98  215 314
    //   *   +   *   +  
    // Returns: vertical numbers in 1, 24, 356 & 369, 248, 8
    ValuesAndOperations parse_column_values_and_operations(std::string_view payload) {
        std::vector<std::vector<uint64_t>> values;
        std::vector<Operation> operations;
        
        size_t line_length = get_line_length(payload);
        size_t nr_rows = (payload.length() - 1) / line_length;
        uint64_t current_number = 0;
        bool has_number = false;
        bool parsing_operations = false;
        std::vector<uint8_t> figures = {};
        std::vector<uint64_t> current_row = {};
        
        for (size_t i = 0; i < line_length; i++) {
            char c = payload[i];
            
            // Skip whitespace except newlines
            if (c == '\n' || c == '\t' || c == '\r')
                continue;

            if (c >= '0' && c <= '9')
                figures.push_back(c - '0');
            
            for (size_t j = 1; j < nr_rows; j++) {
                auto other_index = i + j * line_length;
                if (other_index < payload.length()) {
                    char row_char = payload[i + j * line_length];
                    if (row_char >= '0' && row_char <= '9')
                        figures.push_back(row_char - '0');
                }
            }

            if (figures.size() == 0 && current_row.size() > 0) {
                values.push_back(current_row);
                current_row.clear();
            } else {
                uint64_t total = 0;

                for (auto& figure: figures) {
                    total *= 10;
                    total += static_cast<uint64_t>(figure);
                }
                current_row.push_back(total);

                figures.clear();
            }
        }

        if (current_row.size() > 0)
            values.push_back(current_row);

        for (size_t i = payload.length() - line_length; i < payload.length(); i++) {
            char c = payload[i];
            
            if (c == '+') {
                // Addition operation
                operations.push_back(Operation::Add);
            } else if (c == '*') {
                // Multiplication operation
                operations.push_back(Operation::Multiply);
            }
        }

        return { values, operations };
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

        // Total the result of every vertically
    uint64_t calculate_homework_total_vertical(std::string_view payload) {
        auto values_and_ops = parse_values_and_operations(payload);
        auto values = values_and_ops.first;
        size_t nr_values = values.size();
        auto operations = values_and_ops.second;

        uint64_t result = 0;
        for (size_t i = 0; i < operations.size(); i++) {
            uint64_t sub_total = 0;
            if (operations[i] == Operation::Multiply)
                sub_total = 1;

            int max_nr_digits = 0;
            for (size_t j = 0; j < nr_values; j++)
                max_nr_digits = std::max(count_digits(values[j][i]), max_nr_digits);
    
            for (size_t k = 0; k < max_nr_digits; k++) {
                uint64_t new_value = 0;
                for (size_t j = 0; j < nr_values; j++) {
                    if (values[j][i] != 0) {
                        new_value *= 10;
                        new_value += values[j][i] % 10;
                        values[j][i] /= 10;
                    }
                }

                if (operations[i] == Operation::Add) {
                    sub_total += new_value;
                } else {
                    sub_total *= new_value;
                }
            }

            result += sub_total;
        }

        return result;
    }
}