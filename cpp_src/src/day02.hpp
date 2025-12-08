// Day02
#include <vector>
#include <string_view>
#include <utility>
#include <cstdint>
#include <cmath>

namespace day02
{
    // Parses a string into pairs of 64-bit unsigned integers
    // Format: "num1-num2,num3-num4,..."
    // e.g. "1111-2222,3333-4444" would parse to [(1111, 2222), (3333, 4444)]
    std::vector<std::pair<uint64_t, uint64_t>> parse_integer_pairs(std::string_view payload) {
        std::vector<std::pair<uint64_t, uint64_t>> result;
        
        for (size_t i = 0; i < payload.length(); ) {
            // Skip whitespace and line endings
            if (payload[i] == ' ' || payload[i] == '\t' ||
                payload[i] == '\r' || payload[i] == '\n') {
                i++;
                continue;
            }
            
            // Parse first number
            uint64_t first = 0;
            while (i < payload.length() && payload[i] >= '0' && payload[i] <= '9') {
                first = first * 10 + (payload[i++] - '0');
            }
            
            // Skip the dash separator
            if (i < payload.length() && payload[i] == '-') {
                i++;
            }
            
            // Parse second number
            uint64_t second = 0;
            while (i < payload.length() && payload[i] >= '0' && payload[i] <= '9') {
                second = second * 10 + (payload[i++] - '0');
            }
            
            // Add the pair to result
            result.push_back({first, second});
            
            // Skip comma separator if present
            if (i < payload.length() && payload[i] == ',') {
                i++;
            }
        }
        
        return result;
    }

    // Checks if a value is within the range defined by a pair (inclusive)
    // Returns true if min <= value <= max, where min = pair.first, max = pair.second
    bool is_in_range(uint64_t value, const std::pair<uint64_t, uint64_t>& range) {
        return value >= range.first && value <= range.second;
    }

    // Counts the number of base 10 digits in a uint64_t
    // Uses binary search for optimal performance
    // Returns 1 for value 0, otherwise returns the digit count
    inline int count_digits(uint64_t value) {
        if (value == 0) return 1;
        
        // Binary search through powers of 10
        if (value < 10000) {
            if (value < 100) {
                return (value < 10) ? 1 : 2;
            } else {
                return (value < 1000) ? 3 : 4;
            }
        } else if (value < 100000000) {
            if (value < 1000000) {
                return (value < 100000) ? 5 : 6;
            } else {
                return (value < 10000000) ? 7 : 8;
            }
        } else if (value < 10000000000000ULL) {
            if (value < 10000000000ULL) {
                return (value < 1000000000) ? 9 : 10;
            } else {
                if (value < 100000000000ULL) {
                    return 11;
                } else {
                    return (value < 1000000000000ULL) ? 12 : 13;
                }
            }
        } else {
            if (value < 1000000000000000ULL) {
                return (value < 100000000000000ULL) ? 14 : 15;
            } else {
                if (value < 100000000000000000ULL) {
                    if (value < 10000000000000000ULL) {
                        return 16;
                    } else {
                        return 17;
                    }
                } else {
                    if (value < 1000000000000000000ULL) {
                        return 18;
                    } else {
                        return (value < 10000000000000000000ULL) ? 19 : 20;
                    }
                }
            }
        }
    }

    // Returns the first n/2 digits of a number
    // For example: get_first_half_digits(123456) returns 123
    // Uses pow to calculate 10^(n/2) as the divisor
    inline uint64_t get_first_half_digits(uint64_t value) {
        int n = count_digits(value);
        int half = n / 2;
        uint64_t divisor = static_cast<uint64_t>(std::pow(10, half));
        return value / divisor;
    }

    // Repeats a number by concatenating it with itself
    // For example: repeat_number(111) returns 111111
    // For example: repeat_number(42) returns 4242
    inline uint64_t repeat_number(uint64_t value) {
        int digits = count_digits(value);
        uint64_t multiplier = static_cast<uint64_t>(std::pow(10, digits));
        return value * multiplier + value;
    }

    uint32_t get_repeated_in_range(const std::pair<uint64_t, uint64_t> range) {
        uint32_t result = 0;
        int nr_first_digits = count_digits(range.first);

        uint64_t current_value = 0;
        if (nr_first_digits % 2 == 0) {
            current_value = get_first_half_digits(range.first);
        } else {
            current_value = std::pow(10, nr_first_digits/2 + 1);
        }

        while (true) {
            auto repeat_value = repeat_number(current_value++);
    
            if (is_in_range(repeat_value, range)) {
                result++;
            } else {
                break;
            }
        }

        return result;
    }

    uint32_t get_number_repeated_digits(std::string_view payload) {
        uint32_t result = 0;
        auto ranges = parse_integer_pairs(payload);

        for (const auto& range: ranges)
            result += get_repeated_in_range(range);

        return result;
    }
}