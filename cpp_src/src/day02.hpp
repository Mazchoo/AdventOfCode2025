// Day02
#include <vector>
#include <string_view>
#include <utility>
#include <cstdint>

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
}