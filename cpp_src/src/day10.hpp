// Day10
#pragma once

#include <vector>
#include <map>
#include <string_view>
#include <cstdint>
#include <cmath>
#include <queue>
#include <algorithm>
#include <iostream>

namespace day10
{
    struct BooleanState {
        size_t nr_digits = 0;
        uint16_t initial_state = {};
        std::vector<uint16_t> transitions = {};
    };

    struct IntegerState {
        std::vector<uint16_t> transitions = {};
        std::vector<uint8_t> final_values = {};
    };

    // Convert a vector of uint8_t values to a uint16_t bitmask
    // where each bit represents whether the corresponding value is odd (1) or even (0)
    // e.g., {3, 5, 4, 7} -> bits at positions 0,1,3 set (odd values) -> 0b1011
    inline uint16_t vector_to_odd_even_bitmask(const std::vector<uint8_t>& values) {
        uint16_t bitmask = 0;
        for (size_t i = 0; i < values.size() && i < 16; i++) {
            if (values[i] & 1) {  // Check if odd
                bitmask |= (1 << i);
            }
        }
        return bitmask;
    }

    // Count the number of true values in a boolean vector
    inline size_t nr_transitions_in_presses(const std::vector<bool>& states_used) {
        size_t count = 0;
        for (bool state : states_used) {
            if (state) {
                count++;
            }
        }
        return count;
    }

    // Count how many times each bit position is set across used transitions
    // For each transition that is marked as used (states_used[i] == true),
    // increment the count at each index where that transition has a 1 bit
    // e.g., transitions = {0b00100, 0b11000, 0b10000}, states_used = {true, true, true}
    //       -> [2, 1, 1, 0, 0] (bit 0 appears in 2 transitions, bit 1 in 1, bit 2 in 1)
    inline std::vector<uint8_t> count_transition_bits(
        const std::vector<uint16_t>& transitions,
        const std::vector<bool>& states_used,
        size_t solution_length
    ) {
        std::vector<uint8_t> result(solution_length, 0);

        for (size_t i = 0; i < transitions.size() && i < states_used.size(); i++) {
            if (states_used[i]) {
                uint16_t transition = transitions[i];
                for (size_t bit = 0; bit < solution_length && bit < 16; bit++) {
                    if (transition & (1 << bit)) {
                        result[bit]++;
                    }
                }
            }
        }

        return result;
    }

    // Subtract counts_update from target and update counts_update in place
    // Returns true if operation is valid (counts_update[i] <= target[i] for all i)
    // If valid, sets counts_update[i] = target[i] - counts_update[i]
    // If invalid, returns false and leaves counts_update unchanged
    inline bool subtract_and_update(std::vector<uint8_t>& counts_update, const std::vector<uint8_t>& target) {
        // Check if operation is valid
        if (counts_update.size() != target.size()) {
            return false;
        }

        for (size_t i = 0; i < counts_update.size(); i++) {
            if (counts_update[i] > target[i]) {
                return false;
            }
        }

        // Perform subtraction in place
        for (size_t i = 0; i < counts_update.size(); i++) {
            counts_update[i] = target[i] - counts_update[i];
        }

        return true;
    }

    // Halve all values in a solution vector in place
    // Each element is divided by 2 (integer division)
    inline void reduce_even_solution(std::vector<uint8_t>& solution) {
        for (auto& val : solution) {
            val /= 2;
        }
    }

    // Parse a single line of input and return a State object
    // Format: [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
    // The initial state [.##.] is converted to a bitwise uint16_t
    // where '.' = false (0) and '#' = true (1)
    // Each parentheses group like (3) or (1,3) represents a bitmask
    // where the numbers indicate which bit positions are set to true
    // e.g., (3) = [False, False, False, True] = bit 3 set = 0b1000
    // e.g., (1,3) = bits 1 and 3 set = 0b1010
    inline BooleanState parse_line_boolean(std::string_view line) {
        BooleanState state;
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
                    state.transitions.push_back(bitmask);
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

    // Parse a single line of input and return a State2 object
    // Format: [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
    // [.##.] is ignored but used to determine nr_digits (length of the pattern)
    // Each parentheses group like (3) or (1,3) represents positions to set in a vector
    // e.g., (3) with 4 digits = [0, 0, 0, 1] (4th position is 1)
    // e.g., (1,3) with 4 digits = [0, 1, 0, 1] (2nd and 4th positions are 1)
    // {3,5,4,7} is parsed as a vector of uint8_t values
    inline IntegerState parse_line_integer(std::string_view line) {
        IntegerState state;
        
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
                if (c == ']') {
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
                    state.transitions.push_back(bitmask);
                    bit_positions.clear();
                    continue;
                }
                
                if (c == '{') {
                    parse_state = ParseState::PARSE_BRACES;
                    current_value = 0;
                    parsing_number = false;
                    continue;
                }
            }
            
            if (parse_state == ParseState::PARSE_BRACES) {
                if (c == ' ' || c == '\t')
                    continue;
                
                if (c >= '0' && c <= '9') {
                    current_value = current_value * 10 + (c - '0');
                    parsing_number = true;
                    continue;
                }
                
                if (c == ',') {
                    if (parsing_number) {
                        state.final_values.push_back(static_cast<uint8_t>(current_value));
                        current_value = 0;
                        parsing_number = false;
                    }
                    continue;
                }
                
                if (c == '}') {
                    if (parsing_number) {
                        state.final_values.push_back(static_cast<uint8_t>(current_value));
                        current_value = 0;
                        parsing_number = false;
                    }
                    parse_state = ParseState::DONE;
                    break;
                }
            }
        }
        
        return state;
    }

    // Parse multiple lines of input and return a vector of State objects
    inline std::vector<BooleanState> parse_input_boolean(std::string_view payload) {
        std::vector<BooleanState> states;
        
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
                        states.push_back(parse_line_boolean(line));
                    }
                }
                
                line_start = i + 1;
            }
        }
        
        return states;
    }

    // Parse multiple lines of input and return a vector of IntegerState objects
    inline std::vector<IntegerState> parse_input_integer(std::string_view payload) {
        std::vector<IntegerState> states;
        
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
                        states.push_back(parse_line_integer(line));
                    }
                }
                
                line_start = i + 1;
            }
        }
        
        return states;
    }

    uint32_t fewest_button_presses(std::string_view payload) {
        std::vector<BooleanState> states = parse_input_boolean(payload);
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
                    for (auto& transition: state.transitions) {
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

    // Find every solution in the solution space that can provide the odd and even numbers
    // in a configuation
    // In each case subtract that state from the final state which will be series of even
    // numbers, and then half the even numbers and solve the state recursively
    // until all numbers are 1 or 0
    uint32_t fewest_presses_to_configuration(std::string_view payload) {
        std::vector<IntegerState> states = parse_input_integer(payload);
        uint32_t result = 0;

        return result;
    }
}
