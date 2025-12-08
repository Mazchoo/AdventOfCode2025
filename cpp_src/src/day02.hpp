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
}