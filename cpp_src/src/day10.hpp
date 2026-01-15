// Day10
#pragma once

#include <vector>
#include <map>
#include <string_view>
#include <cstdint>
#include <cmath>

namespace day10
{
    struct BooleanState {
        size_t nr_digits;
        uint16_t initial_state;
        std::vector<uint16_t> transitions;
    };

    struct IntegerState {
        size_t nr_digits;
        std::vector<std::vector<uint8_t>> transitions;
        std::vector<uint8_t> final_values;
    };

    // Parse a single line of input and return a State object
    // Format: [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
    // The initial state [.##.] is converted to a bitwise uint16_t
    // where '.' = false (0) and '#' = true (1)
    // Each parentheses group like (3) or (1,3) represents a bitmask
    // where the numbers indicate which bit positions are set to true
    // e.g., (3) = [False, False, False, True] = bit 3 set = 0b1000
    // e.g., (1,3) = bits 1 and 3 set = 0b1010
    BooleanState parse_line_boolean(std::string_view line) {
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
    IntegerState parse_line_integer(std::string_view line) {
        IntegerState state;
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
                    state.nr_digits++;
                } else if (c == ']') {
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
                    
                    // Convert bit positions to vector
                    std::vector<uint8_t> transition(state.nr_digits, 0);
                    for (uint16_t pos : bit_positions) {
                        if (pos < state.nr_digits) {
                            transition[pos] = 1;
                        }
                    }
                    state.transitions.push_back(transition);
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
    std::vector<BooleanState> parse_input_boolean(std::string_view payload) {
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
    std::vector<IntegerState> parse_input_integer(std::string_view payload) {
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

    uint32_t fewest_presses_to_configuration(std::string_view payload) {
        std::vector<IntegerState> states = parse_input_integer(payload);
        uint32_t result = 0;

        for (auto& state : states) {
            auto target_state = state.final_values;
            std::map<std::vector<uint8_t>, bool> solution;

            std::vector<uint8_t> initial_state(state.nr_digits, 0);
            if (initial_state == target_state)
                continue;
            solution[initial_state] = true;

            std::vector<std::vector<uint8_t>> candidates = { initial_state };
            std::vector<std::vector<uint8_t>> new_candidates = {};

            bool solution_found = false;
            uint32_t current_iteration = 0;
            while (!solution_found) {
                current_iteration++;
                for (auto& candidate : candidates) {
                    for (auto& transition : state.transitions) {
                        std::vector<uint8_t> new_state = candidate;
                        for (int i = 0; i < state.nr_digits; i++)
                            new_state[i] += transition[i];

                        if (new_state == target_state) {
                            solution_found = true;
                            break;
                        }

                        auto it = solution.find(new_state);
                        if (it == solution.end()) {
                            solution[new_state] = true;
                        } else {
                            continue;
                        }

                        bool overflow_constraint = false;
                        for (int i = 0; i < state.nr_digits; i++) {
                            if (new_state[i] > target_state[i]) {
                                overflow_constraint = true;
                                break;
                            }
                        }
                        if (!overflow_constraint)
                            new_candidates.push_back(new_state);
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
