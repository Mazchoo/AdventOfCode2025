// Day03
#include <vector>
#include <string_view>
#include <utility>
#include <cstdint>

namespace day03
{
    // Parses a string into vectors containing individual digits
    // Format: "digits\ndigits\ndigits..." where \n separates lines
    // e.g. "1234\n6789\n42" would parse to [{1, 2, 3, 4}, {6, 7, 8, 9}, {4, 2}]
    // Each digit character is converted to its numeric value (0-9) as uint8_t
    std::vector<std::vector<uint8_t>> parse_digit_lines(std::string_view payload) {
        std::vector<std::vector<uint8_t>> result;
        
        enum class State { SKIP_WHITESPACE, PARSE_DIGITS };
        State state = State::SKIP_WHITESPACE;
        std::vector<uint8_t> current_line;
        
        for (size_t i = 0; i < payload.length(); i++) {
            char c = payload[i];
            
            if (state == State::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    continue;

                current_line.clear();
                state = State::PARSE_DIGITS;
            }
            
            if (state == State::PARSE_DIGITS) {
                if (c >= '0' && c <= '9') {
                    current_line.push_back(static_cast<uint8_t>(c - '0'));
                    continue;
                } else if (c == '\n') {
                    // End of line - store the current line
                    if (!current_line.empty())
                        result.push_back(current_line);
\
                    state = State::SKIP_WHITESPACE;
                }
            }
        }
        
        // Handle the last line if we ended while parsing digits
        if (state == State::PARSE_DIGITS && !current_line.empty())
            result.push_back(current_line);

        return result;
    }

    // Find highest pair of digits in a line
    uint32_t highest_digit_pair(const std::vector<uint8_t>& line) {
        const size_t line_length = line.size();
        if (line_length == 0) return 0;
        if (line_length == 1) return static_cast<uint8_t>(line[0] * 10);

        uint8_t first_digit = 0;
        uint8_t second_digit = 0;
        for (size_t i = 0; i < line_length; i++) {
            uint8_t entry = line[i];
            if (entry > first_digit && i < line_length - 1) {
                first_digit = entry;
                second_digit = 0;
                continue;
            }
            if (entry > second_digit)
                second_digit = entry;
        }

        return first_digit * 10 + second_digit;
    }

    uint32_t day03_pt1(std::string_view payload) {
        uint64_t result = 0;
        auto digit_lines = parse_digit_lines(payload);

        for (const auto& line: digit_lines)
            result += highest_digit_pair(line);

        return result;
    }
}