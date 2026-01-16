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
        size_t nr_digits = 0;
        std::vector<std::vector<float>> transitions = {};
        std::vector<float> final_values = {};
    };

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
    // {3,5,4,7} is parsed as a vector of float values
    inline IntegerState parse_line_integer(std::string_view line) {
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
        float current_value = 0;
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
                    std::vector<float> transition(state.nr_digits, 0);
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

    inline uint32_t fewest_button_presses(std::string_view payload) {
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

    // Solve an IntegerState using linear programming
    // Minimize: sum of solution vector
    // Subject to: sum(solution[i] * transitions[i]) = final_values
    //            solution[i] >= 0 for all i
    // Returns: vector of floats representing the solution
    inline std::vector<float> solve_simplex(const IntegerState& state) {
        size_t m = state.nr_digits;        // Number of constraints (rows)
        size_t n = state.transitions.size(); // Number of variables (columns)
        
        if (n == 0 || m == 0) {
            return std::vector<float>();
        }
        
        // Build tableau for two-phase simplex
        // Columns: [original vars | artificial vars | RHS]
        // Rows: [constraints | objective]
        size_t total_cols = n + m + 1;  // n original + m artificial + 1 RHS
        size_t total_rows = m + 1;       // m constraints + 1 objective
        
        std::vector<std::vector<float>> tab(total_rows, std::vector<float>(total_cols, 0.0f));
        
        // Fill constraint rows: A*x + I*a = b
        for (size_t i = 0; i < m; i++) {
            // Original variable coefficients
            for (size_t j = 0; j < n; j++) {
                tab[i][j] = state.transitions[j][i];
            }
            // Artificial variable (identity)
            tab[i][n + i] = 1.0f;
            // RHS
            tab[i][total_cols - 1] = state.final_values[i];
        }
        
        // PHASE 1: Minimize sum of artificial variables
        // Initial objective: min sum(a_i) = -sum(a_i) in max form
        // Need to eliminate basic artificial variables from objective
        for (size_t i = 0; i < m; i++) {
            // Add constraint row i to objective to eliminate artificial variable
            for (size_t j = 0; j < total_cols; j++) {
                tab[m][j] -= tab[i][j];
            }
        }
        
        // Run simplex for phase 1
        for (int iter = 0; iter < 1000; iter++) {
            // Find entering variable (most negative reduced cost)
            int enter_col = -1;
            float most_neg = -1e-9f;
            for (size_t j = 0; j < n + m; j++) {
                if (tab[m][j] < most_neg) {
                    most_neg = tab[m][j];
                    enter_col = j;
                }
            }
            if (enter_col < 0) break; // Optimal
            
            // Find leaving variable (min ratio test)
            int leave_row = -1;
            float min_ratio = 1e30f;
            for (size_t i = 0; i < m; i++) {
                if (tab[i][enter_col] > 1e-9f) {
                    float ratio = tab[i][total_cols - 1] / tab[i][enter_col];
                    if (ratio < min_ratio) {
                        min_ratio = ratio;
                        leave_row = i;
                    }
                }
            }
            if (leave_row < 0) break; // Unbounded
            
            // Pivot
            float pivot_val = tab[leave_row][enter_col];
            for (size_t j = 0; j < total_cols; j++) {
                tab[leave_row][j] /= pivot_val;
            }
            for (size_t i = 0; i < total_rows; i++) {
                if (i != static_cast<size_t>(leave_row)) {
                    float mult = tab[i][enter_col];
                    for (size_t j = 0; j < total_cols; j++) {
                        tab[i][j] -= mult * tab[leave_row][j];
                    }
                }
            }
        }
        
        // PHASE 2: Minimize sum of original variables
        // Reset objective row to minimize sum of x_i
        tab[m].assign(total_cols, 0.0f);
        for (size_t j = 0; j < n; j++) {
            tab[m][j] = 1.0f;
        }
        
        // Eliminate basic variables from objective
        for (size_t i = 0; i < m; i++) {
            // Find basic variable in row i (among original variables)
            for (size_t j = 0; j < n; j++) {
                // Check if column j is basic in row i
                bool is_basic_here = (std::abs(tab[i][j] - 1.0f) < 1e-9f);
                if (is_basic_here) {
                    // Check it's the only non-zero in this column
                    bool only_one = true;
                    for (size_t k = 0; k < m; k++) {
                        if (k != i && std::abs(tab[k][j]) > 1e-9f) {
                            only_one = false;
                            break;
                        }
                    }
                    if (only_one) {
                        // Eliminate from objective
                        float coef = tab[m][j];
                        for (size_t c = 0; c < total_cols; c++) {
                            tab[m][c] -= coef * tab[i][c];
                        }
                        break;
                    }
                }
            }
        }
        
        // Run simplex for phase 2
        for (int iter = 0; iter < 1000; iter++) {
            int enter_col = -1;
            float most_neg = -1e-9f;
            for (size_t j = 0; j < n; j++) {  // Only original variables
                if (tab[m][j] < most_neg) {
                    most_neg = tab[m][j];
                    enter_col = j;
                }
            }
            if (enter_col < 0) break;
            
            int leave_row = -1;
            float min_ratio = 1e30f;
            for (size_t i = 0; i < m; i++) {
                if (tab[i][enter_col] > 1e-9f) {
                    float ratio = tab[i][total_cols - 1] / tab[i][enter_col];
                    if (ratio < min_ratio) {
                        min_ratio = ratio;
                        leave_row = i;
                    }
                }
            }
            if (leave_row < 0) break;
            
            float pivot_val = tab[leave_row][enter_col];
            for (size_t j = 0; j < total_cols; j++) {
                tab[leave_row][j] /= pivot_val;
            }
            for (size_t i = 0; i < total_rows; i++) {
                if (i != static_cast<size_t>(leave_row)) {
                    float mult = tab[i][enter_col];
                    for (size_t j = 0; j < total_cols; j++) {
                        tab[i][j] -= mult * tab[leave_row][j];
                    }
                }
            }
        }
        
        // Extract solution
        std::vector<float> solution(n, 0.0f);
        for (size_t j = 0; j < n; j++) {
            // Find if variable j is basic
            for (size_t i = 0; i < m; i++) {
                if (std::abs(tab[i][j] - 1.0f) < 1e-9f) {
                    // Check it's the only non-zero in column
                    bool is_basic = true;
                    for (size_t k = 0; k < m; k++) {
                        if (k != i && std::abs(tab[k][j]) > 1e-9f) {
                            is_basic = false;
                            break;
                        }
                    }
                    if (is_basic) {
                        solution[j] = tab[i][total_cols - 1];
                        if (solution[j] < 0.0f) solution[j] = 0.0f;
                        break;
                    }
                }
            }
        }
        
        return solution;
    }

    // Helper function to check if a solution satisfies all constraints
    // For integer problems, we need exact equality
    inline bool satisfies_constraints(const std::vector<int64_t>& solution,
                                     const IntegerState& state) {
        for (size_t i = 0; i < state.nr_digits; i++) {
            int64_t sum = 0;
            for (size_t j = 0; j < state.transitions.size(); j++) {
                // Since transitions are 0 or 1, and final_values are integers
                sum += solution[j] * static_cast<int64_t>(state.transitions[j][i]);
            }
            // Check for exact equality (final_values should be integers)
            if (sum != static_cast<int64_t>(state.final_values[i])) {
                return false;
            }
        }
        return true;
    }

    // Helper function to calculate objective value (sum of all variables)
    inline int64_t calculate_objective(const std::vector<int64_t>& solution) {
        int64_t sum = 0;
        for (int64_t val : solution) {
            sum += val;
        }
        return sum;
    }

    // Search in the neighborhood of the relaxed solution for an integer solution
    inline std::vector<int64_t> neighborhood_search(const std::vector<float>& relaxed_solution,
                                                     const IntegerState& state) {
        size_t n = relaxed_solution.size();
        
        // Try different rounding strategies first
        std::vector<int64_t> best_solution(n);
        int64_t best_objective = INT64_MAX;
        bool found_valid = false;
        
        // Strategy 1: Round to nearest
        std::vector<int64_t> rounded(n);
        for (size_t i = 0; i < n; i++) {
            rounded[i] = static_cast<int64_t>(std::round(relaxed_solution[i]));
            if (rounded[i] < 0) rounded[i] = 0;
        }
        if (satisfies_constraints(rounded, state)) {
            best_solution = rounded;
            best_objective = calculate_objective(rounded);
            found_valid = true;
        }
        
        // Strategy 2: Ceil all (more likely to satisfy constraints)
        std::vector<int64_t> ceiled(n);
        for (size_t i = 0; i < n; i++) {
            ceiled[i] = static_cast<int64_t>(std::ceil(relaxed_solution[i]));
            if (ceiled[i] < 0) ceiled[i] = 0;
        }
        if (satisfies_constraints(ceiled, state)) {
            int64_t obj = calculate_objective(ceiled);
            if (!found_valid || obj < best_objective) {
                best_solution = ceiled;
                best_objective = obj;
                found_valid = true;
            }
        }
        
        // If simple strategies worked, return the best
        if (found_valid) {
            return best_solution;
        }
        
        // Otherwise, do BFS search - must find a valid solution!
        std::queue<std::vector<int64_t>> queue;
        std::map<std::vector<int64_t>, bool> visited;
        
        // Start from multiple initial points
        queue.push(ceiled);
        visited[ceiled] = true;
        queue.push(rounded);
        visited[rounded] = true;
        
        int iterations = 0;
        const int max_iterations = 5000000;  // Increased iterations
        
        while (!queue.empty() && iterations < max_iterations) {
            iterations++;
            auto current = queue.front();
            queue.pop();
            
            // Check if current solution satisfies constraints
            if (satisfies_constraints(current, state)) {
                int64_t obj = calculate_objective(current);
                if (!found_valid || obj < best_objective) {
                    best_solution = current;
                    best_objective = obj;
                    found_valid = true;
                }
                // Don't stop - continue exploring for better solutions
                continue;
            }
            
            // Explore neighbors - no radius limit, just explore!
            for (size_t i = 0; i < n; i++) {
                // Try incrementing
                std::vector<int64_t> neighbor_inc = current;
                neighbor_inc[i]++;
                if (visited.find(neighbor_inc) == visited.end()) {
                    visited[neighbor_inc] = true;
                    queue.push(neighbor_inc);
                }
                
                // Try decrementing (but keep non-negative)
                if (current[i] > 0) {
                    std::vector<int64_t> neighbor_dec = current;
                    neighbor_dec[i]--;
                    if (visited.find(neighbor_dec) == visited.end()) {
                        visited[neighbor_dec] = true;
                        queue.push(neighbor_dec);
                    }
                }
            }
        }
        
        // MUST return a valid solution - if not found, this is an error
        if (!found_valid) {
            std::cerr << "ERROR: Could not find valid integer solution after " << iterations << " iterations!" << std::endl;
            std::cerr << "Relaxed solution was: ";
            for (float val : relaxed_solution) {
                std::cerr << val << " ";
            }
            std::cerr << std::endl;
            // Return ceiled as last resort, but this indicates a problem
            return ceiled;
        }
        
        return best_solution;
    }

    inline uint32_t fewest_presses_to_configuration(std::string_view payload) {
        std::vector<IntegerState> states = parse_input_integer(payload);
        uint32_t result = 0;

        for (size_t state_idx = 0; state_idx < states.size(); state_idx++) {
            auto& state = states[state_idx];
            const auto relaxed_solution = solve_simplex(state);
            
            std::cout << "State " << state_idx << " relaxed solution: ";
            for (size_t i = 0; i < relaxed_solution.size(); i++) {
                std::cout << relaxed_solution[i] << " ";
            }
            std::cout << std::endl;
            
            // Search for an integer solution in the neighborhood of the relaxed solution
            const auto integer_solution = neighborhood_search(relaxed_solution, state);

            std::cout << "State " << state_idx << " integer solution: ";
            for (size_t i = 0; i < integer_solution.size(); i++) {
                std::cout << integer_solution[i] << " ";
            }
            std::cout << std::endl;
            
            // Verify the solution satisfies constraints
            bool valid = satisfies_constraints(integer_solution, state);
            std::cout << "State " << state_idx << " valid: " << (valid ? "YES" : "NO") << std::endl;
            
            if (!valid) {
                std::cout << "ERROR: Invalid solution found!" << std::endl;
                std::cout << "Constraint check:" << std::endl;
                for (size_t i = 0; i < state.nr_digits; i++) {
                    int64_t sum = 0;
                    for (size_t j = 0; j < state.transitions.size(); j++) {
                        sum += integer_solution[j] * static_cast<int64_t>(state.transitions[j][i]);
                    }
                    std::cout << "  Digit " << i << ": sum=" << sum << " expected=" << state.final_values[i] << std::endl;
                }
            }

            uint32_t presses = 0;
            for (size_t i = 0; i < integer_solution.size(); i++) {
                presses += integer_solution[i];
            }
            std::cout << "State " << state_idx << " presses: " << presses << std::endl;
            std::cout << std::endl;
            result += presses;
        }

        return result;
    }
}
