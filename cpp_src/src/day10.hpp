// Day10
#pragma once

#include <vector>
#include <string_view>
#include <cstdint>
#include <cmath>

namespace day10
{
    struct State {
        size_t nr_digits;
        uint16_t initial_state;
        std::vector<uint16_t> state_changes;
    };

    // Parse a single line of input and return a State object
    // Format: [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
    // The initial state [.##.] is converted to a bitwise uint16_t
    // where '.' = false (0) and '#' = true (1)
    // Each parentheses group like (3) or (1,3) represents a bitmask
    // where the numbers indicate which bit positions are set to true
    // e.g., (3) = [False, False, False, True] = bit 3 set = 0b1000
    // e.g., (1,3) = bits 1 and 3 set = 0b1010
    State parse_line(std::string_view line) {
        State state;
        state.initial_state = 0;
        state.nr_digits = 0;
        
        enum class ParseState {
            SKIP_WHITESPACE,
            PARSE_INITIAL_STATE,
            PARSE_PARENTHESES,
            PARSE_BRACES,
            DONE
        };
        
        ParseState parse_state = ParseState::SKIP_WHITESPACE;
        std::vector<uint16_t> bit_positions;
        uint16_t current_value = 0;
        bool parsing_number = false;
        
        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];
            
            if (parse_state == ParseState::SKIP_WHITESPACE) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    continue;
                
                if (c == '[') {
                    parse_state = ParseState::PARSE_INITIAL_STATE;
                    continue;
                }
            }
            
            if (parse_state == ParseState::PARSE_INITIAL_STATE) {
                if (c == '#' || c == '.') {
                    bool bit = (c == '#');
                    state.initial_state |= (bit << state.nr_digits);
                    state.nr_digits++;
                } else if (c == ']') {
                    parse_state = ParseState::SKIP_WHITESPACE;
                    // After closing bracket, look for parentheses
                    parse_state = ParseState::PARSE_PARENTHESES;
                }
                continue;
            }
            
            if (parse_state == ParseState::PARSE_PARENTHESES) {
                if (c == ' ' || c == '\t')
                    continue;
                
                if (c == '(') {
                    current_value = 0;
                    parsing_number = false;
                    continue;
                }
                
                if (c >= '0' && c <= '9') {
                    current_value = current_value * 10 + (c - '0');
                    parsing_number = true;
                    continue;
                }
                
                if (c == ',') {
                    if (parsing_number) {
                        bit_positions.push_back(current_value);
                        current_value = 0;
                        parsing_number = false;
                    }
                    continue;
                }
                
                if (c == ')') {
                    if (parsing_number) {
                        bit_positions.push_back(current_value);
                        current_value = 0;
                        parsing_number = false;
                    }
                    
                    // Convert bit positions to bitmask
                    uint16_t bitmask = 0;
                    for (uint16_t pos : bit_positions) {
                        bitmask |= (1 << pos);
                    }
                    state.state_changes.push_back(bitmask);
                    bit_positions.clear();
                    continue;
                }
                
                if (c == '{') {
                    // Skip the curly braces section for now
                    parse_state = ParseState::DONE;
                    break;
                }
            }
        }
        
        return state;
    }

    // Parse multiple lines of input and return a vector of State objects
    std::vector<State> parse_input(std::string_view payload) {
        std::vector<State> states;
        
        size_t line_start = 0;
        for (size_t i = 0; i < payload.length(); i++) {
            if (payload[i] == '\n' || i == payload.length() - 1) {
                size_t line_end = (i == payload.length() - 1 && payload[i] != '\n') ? i + 1 : i;
                
                if (line_end > line_start) {
                    std::string_view line = payload.substr(line_start, line_end - line_start);
                    
                    // Skip empty lines
                    bool has_content = false;
                    for (char c : line) {
                        if (c != ' ' && c != '\t' && c != '\r') {
                            has_content = true;
                            break;
                        }
                    }
                    
                    if (has_content) {
                        states.push_back(parse_line(line));
                    }
                }
                
                line_start = i + 1;
            }
        }
        
        return states;
    }

    uint32_t fewest_button_presses(std::string_view payload) {
        std::vector<State> states = parse_input(payload);
        uint32_t result = 0;

        for (auto& state: states) {
            if (state.initial_state == 0)
                continue;

            std::vector<bool> solution(std::pow(2, state.nr_digits), false);
            std::vector<uint16_t> candidates = {state.initial_state};
            std::vector<uint16_t> new_candidates = {};
            solution[state.initial_state] = true;

            bool solution_found = false;
            uint32_t current_iteration = 0;
            while (!solution_found) {
                current_iteration++;
                for (auto& candidate: candidates) {
                    for (auto& transition: state.state_changes) {
                        auto new_state = candidate ^ transition;

                        if (new_state == 0) {
                            solution_found = true;
                            break;
                        }

                        if (solution[new_state] == false) {
                            solution[new_state] = true;
                            new_candidates.push_back(new_state);
                        }
                    }
                    if (solution_found)
                        break;
                }
                candidates = new_candidates;
                new_candidates.clear();
            }
            result += current_iteration;
        }

        return result;
    }
}
