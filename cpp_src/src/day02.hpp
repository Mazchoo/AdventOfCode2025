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
        
        enum class State { SKIP_WHITESPACE, PARSE_FIRST, SKIP_DASH, PARSE_SECOND, SKIP_COMMA };
        State state = State::SKIP_WHITESPACE;
        uint64_t first = 0;
        uint64_t second = 0;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    continue;
                }
                first = 0;
                state = State::PARSE_FIRST;
            }

            if (state == State::PARSE_FIRST) {
                if (c >= '0' && c <= '9') {
                    first = first * 10 + (c - '0');
                    continue;
                } else {
                    state = State::SKIP_DASH;
                    i--; // Re-process this character in SKIP_DASH state
                }
            }

            else if (state == State::SKIP_DASH) {
                if (c == '-') {
                    second = 0;
                    state = State::PARSE_SECOND;
                    continue;
                }
            }

            if (state == State::PARSE_SECOND) {
                if (c >= '0' && c <= '9') {
                    second = second * 10 + (c - '0');
                    continue;
                } else {
                    result.push_back({first, second});
                    state = State::SKIP_COMMA;
                }
            }

            if (state == State::SKIP_COMMA) {
                if (c == ',') {
                    state = State::SKIP_WHITESPACE;
                }
            }
        }
        
        // Handle the last pair if we ended while parsing the second number
        if (state == State::PARSE_SECOND) {
            result.push_back({first, second});
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

    uint64_t count_repeated_in_range(const std::pair<uint64_t, uint64_t> range) {
        uint64_t result = 0;
        int nr_first_digits = count_digits(range.first);

        uint64_t current_value = 0;
        if (nr_first_digits % 2 == 0) {
            current_value = get_first_half_digits(range.first);
        } else {
            current_value = std::pow(10, nr_first_digits/2);
        }

        while (true) {
            auto repeat_value = repeat_number(current_value++);
    
            if (is_in_range(repeat_value, range)) {
                result += repeat_value;
            } else {
                break;
            }
        }

        return result;
    }

    uint64_t day02_pt1(std::string_view payload) {
        uint64_t result = 0;
        auto ranges = parse_integer_pairs(payload);

        for (const auto& range: ranges)
            result += count_repeated_in_range(range);

        return result;
    }
}
