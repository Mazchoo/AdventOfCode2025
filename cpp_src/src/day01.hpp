// Day01
#include <vector>
#include <string_view>

namespace day01
{
    // Parses a string into incremental changes based on left and right
    // Lines that increase state with R, lines that decrease start with L
    // e.g.
    // R880\nL35 would parse to [880, -35]
    std::vector<int16_t> parse_safe_adjustments(std::string_view payload) {
        std::vector<int16_t> result;
        
        enum class State { SKIP_WHITESPACE, READ_DIRECTION, PARSE_NUMBER };
        State state = State::SKIP_WHITESPACE;
        char direction = '\0';
        int16_t value = 0;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    continue;
                }
                // Found non-whitespace, treat as direction
                direction = c;
                if (direction == 'R' || direction == 'L') {
                    value = 0;
                    state = State::PARSE_NUMBER;
                }
                // else stay in SKIP_WHITESPACE to skip invalid characters
            }
            else if (state == State::PARSE_NUMBER) {
                if (c >= '0' && c <= '9') {
                    value = value * 10 + (c - '0');
                } else {
                    result.push_back(direction == 'L' ? -value : value);
                    state = State::SKIP_WHITESPACE;
                    // Process current character as whitespace in next iteration
                    if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                        // If it's not whitespace, it might be a direction
                        direction = c;
                        if (direction == 'R' || direction == 'L') {
                            value = 0;
                            state = State::PARSE_NUMBER;
                        }
                    }
                }
            }
        }
        
        // Handle the last value if we ended while parsing a number
        if (state == State::PARSE_NUMBER) {
            result.push_back(direction == 'L' ? -value : value);
        }
        
        return result;
    }

    // Find total number of zeros when starting at 50 and
    // adding each adjustment modulo 100
    uint32_t count_total_zeros(const std::vector<int16_t>& adjustments) {
        int16_t currentState = 50;
        uint32_t totalNrZeros = 0;

        for (const auto adjustment : adjustments) {
            currentState += adjustment;
            currentState %= 100;
            if (currentState == 0)
                totalNrZeros++;
        }

        return totalNrZeros;
    }

    uint32_t day01_pt1(std::string_view payload) {
        const std::vector<int16_t> adjustments = parse_safe_adjustments(payload);
        return count_total_zeros(adjustments);
    }

    // Find total number of zeros when starting at 50 and
    // adding each adjustment modulo 100
    // Each time zero is reached or passed, the count increases
    uint32_t count_passing_zeros(const std::vector<int16_t>& adjustments) {
        int16_t currentState = 50;
        uint32_t totalNrZeros = 0;

        for (const auto adjustment : adjustments) {
            currentState += adjustment;

            while (currentState > 99) {
                currentState -= 100;
                if (currentState != 0)
                    totalNrZeros++;
            }

            while (currentState < 0) {
                if (currentState - adjustment != 0)
                    totalNrZeros++;
                currentState += 100;
            }

            if (currentState == 0)
                totalNrZeros++;
        }

        return totalNrZeros;
    }

    uint32_t day01_pt2(std::string_view payload) {
        const std::vector<int16_t> adjustments = parse_safe_adjustments(payload);
        return count_passing_zeros(adjustments);
    }
}