#include "doctest.h"
#include "day10.hpp"

using namespace day10;

TEST_SUITE("Day10 Tests") {
    TEST_CASE("fewest_presses_to_configuration - gives expected on sample input") {
        std::string_view input =  R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})";
        auto result = fewest_presses_to_configuration(input);
        
        CHECK(result == 33);
    }


    TEST_CASE("fewest_button_presses - gives expected on sample input") {
        std::string_view input =  R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})";
        auto result = fewest_button_presses(input);
        
        CHECK(result == 7);
    }

    TEST_CASE("parse_line - simple initial state with dots") {
        std::string_view input = "[....]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0000);  // All dots = all zeros
        CHECK(state.transitions.empty());
    }
    
    TEST_CASE("parse_line - simple initial state with hashes") {
        std::string_view input = "[####]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b1111);  // All hashes = all ones
        CHECK(state.transitions.empty());
    }
    
    TEST_CASE("parse_line - mixed initial state") {
        std::string_view input = "[.##.]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        // Reading left to right: . # # .
        // Bit positions:        0 1 2 3
        // Binary representation: 0110 (bits 1 and 2 set)
        CHECK(state.initial_state == 0b0110);
        CHECK(state.transitions.empty());
    }
    
    TEST_CASE("parse_line - initial state with single parenthesis") {
        std::string_view input = "[.##.] (3)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b1000);  // Bit 3 set
    }
    
    TEST_CASE("parse_line - initial state with multiple bit positions in one parenthesis") {
        std::string_view input = "[.##.] (1,3)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b1010);  // Bits 1 and 3 set
    }
    
    TEST_CASE("parse_line - multiple parentheses groups") {
        std::string_view input = "[.##.] (3) (1,3) (2)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 3);
        CHECK(state.transitions[0] == 0b1000);  // (3) = bit 3
        CHECK(state.transitions[1] == 0b1010);  // (1,3) = bits 1 and 3
        CHECK(state.transitions[2] == 0b0100);  // (2) = bit 2
    }
    
    TEST_CASE("parse_line - complex example with multiple state changes") {
        std::string_view input = "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 6);
        CHECK(state.transitions[0] == 0b1000);  // (3)
        CHECK(state.transitions[1] == 0b1010);  // (1,3)
        CHECK(state.transitions[2] == 0b0100);  // (2)
        CHECK(state.transitions[3] == 0b1100);  // (2,3)
        CHECK(state.transitions[4] == 0b0101);  // (0,2)
        CHECK(state.transitions[5] == 0b0011);  // (0,1)
    }
    
    TEST_CASE("parse_line - with curly braces section (should be ignored)") {
        std::string_view input = "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 6);
        // Curly braces section should be ignored
    }
    
    TEST_CASE("parse_line - with whitespace") {
        std::string_view input = "  [.##.]   (3)   (1,3)  ";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 2);
        CHECK(state.transitions[0] == 0b1000);
        CHECK(state.transitions[1] == 0b1010);
    }
    
    TEST_CASE("parse_line - single bit initial state") {
        std::string_view input = "[#]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 1);
        CHECK(state.initial_state == 0b1);
    }
    
    TEST_CASE("parse_line - longer initial state") {
        std::string_view input = "[.#.#.#.#]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 8);
        // Pattern: . # . # . # . #
        // Bits:    0 1 2 3 4 5 6 7
        // Result: 0b10101010 (bits 1, 3, 5, 7 set)
        CHECK(state.initial_state == 0b10101010);
    }
    
    TEST_CASE("parse_line - parenthesis with three values") {
        std::string_view input = "[....] (0,2,3)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0000);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b1101);  // Bits 0, 2, 3 set
    }
    
    TEST_CASE("parse_line - parenthesis with single zero") {
        std::string_view input = "[.#..] (0)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0010);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b0001);  // Bit 0 set
    }
    
    TEST_CASE("parse_line - multi-digit bit positions") {
        std::string_view input = "[################] (10,11,12)";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 16);
        CHECK(state.initial_state == 0xFFFF);  // All 16 bits set
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b0001110000000000);  // Bits 10, 11, 12 set
    }
    
    TEST_CASE("parse_input - empty string") {
        std::string_view input = "";
        auto states = parse_input_boolean(input);
        
        CHECK(states.empty());
    }
    
    TEST_CASE("parse_input - single line") {
        std::string_view input = "[.##.] (3) (1,3)";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 1);
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].initial_state == 0b0110);
        CHECK(states[0].transitions.size() == 2);
    }
    
    TEST_CASE("parse_input - multiple lines") {
        std::string_view input = "[.##.] (3)\n[####] (0,1)\n[....] (2,3)";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 3);
        
        // First line
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].initial_state == 0b0110);
        CHECK(states[0].transitions.size() == 1);
        
        // Second line
        CHECK(states[1].nr_digits == 4);
        CHECK(states[1].initial_state == 0b1111);
        CHECK(states[1].transitions.size() == 1);
        
        // Third line
        CHECK(states[2].nr_digits == 4);
        CHECK(states[2].initial_state == 0b0000);
        CHECK(states[2].transitions.size() == 1);
    }
    
    TEST_CASE("parse_input - with empty lines") {
        std::string_view input = "[.##.] (3)\n\n[####] (0,1)\n  \n[....] (2,3)";
        auto states = parse_input_boolean(input);
        
        // Empty lines should be skipped
        REQUIRE(states.size() == 3);
    }
    
    TEST_CASE("parse_input - no trailing newline") {
        std::string_view input = "[.##.] (3)\n[####] (0,1)";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 2);
        CHECK(states[1].nr_digits == 4);
        CHECK(states[1].initial_state == 0b1111);
    }
    
    TEST_CASE("parse_input - with trailing newline") {
        std::string_view input = "[.##.] (3)\n[####] (0,1)\n";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 2);
    }
    
    TEST_CASE("parse_input - complex multi-line example") {
        std::string_view input = 
            "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
            "[####] (0) (1) (2) (3)\n"
            "[....] (0,1,2,3)";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 3);
        
        // First line - complex state changes
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].initial_state == 0b0110);
        CHECK(states[0].transitions.size() == 6);
        
        // Second line - individual bits
        CHECK(states[1].nr_digits == 4);
        CHECK(states[1].initial_state == 0b1111);
        CHECK(states[1].transitions.size() == 4);
        
        // Third line - all bits in one group
        CHECK(states[2].nr_digits == 4);
        CHECK(states[2].initial_state == 0b0000);
        CHECK(states[2].transitions.size() == 1);
        CHECK(states[2].transitions[0] == 0b1111);
    }
    
    TEST_CASE("parse_input - lines with only whitespace are skipped") {
        std::string_view input = "   \n\t\t\n[.##.] (3)\n   \r\n";
        auto states = parse_input_boolean(input);
        
        REQUIRE(states.size() == 1);
        CHECK(states[0].nr_digits == 4);
    }
    
    TEST_CASE("parse_line - empty parentheses should not add state change") {
        std::string_view input = "[.##.] ()";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.initial_state == 0b0110);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0);  // Empty parentheses = no bits set
    }
    
    TEST_CASE("parse_line - alternating pattern") {
        std::string_view input = "[#.#.#.#.]";
        auto state = parse_line_boolean(input);
        
        CHECK(state.nr_digits == 8);
        // Pattern: # . # . # . # .
        // Bits:    0 1 2 3 4 5 6 7
        // Result: 0b01010101 (bits 0, 2, 4, 6 set)
        CHECK(state.initial_state == 0b01010101);
    }
    
    TEST_CASE("parse_line_integer - simple initial state") {
        std::string_view input = "[.##.]";
        auto state = parse_line_integer(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.transitions.empty());
        CHECK(state.final_values.empty());
    }
    
    TEST_CASE("parse_line_integer - with single parenthesis") {
        std::string_view input = "[.##.] (3)";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b1000);  // Bit 3 set
        CHECK(state.final_values.empty());
    }
    
    TEST_CASE("parse_line_integer - with multiple positions in one parenthesis") {
        std::string_view input = "[.##.] (1,3)";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b1010);  // Bits 1 and 3 set
    }
    
    TEST_CASE("parse_line_integer - multiple parentheses groups") {
        std::string_view input = "[.##.] (3) (1,3) (2)";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 3);
        CHECK(state.transitions[0] == 0b1000);  // (3) = bit 3
        CHECK(state.transitions[1] == 0b1010);  // (1,3) = bits 1 and 3
        CHECK(state.transitions[2] == 0b0100);  // (2) = bit 2
    }
    
    TEST_CASE("parse_line_integer - with curly braces section") {
        std::string_view input = "[.##.] (3) (1,3) {3,5,4,7}";
        auto state = parse_line_integer(input);
        
        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 2);
        REQUIRE(state.final_values.size() == 4);
        CHECK(state.final_values[0] == 3);
        CHECK(state.final_values[1] == 5);
        CHECK(state.final_values[2] == 4);
        CHECK(state.final_values[3] == 7);
    }
    
    TEST_CASE("parse_line_integer - complex example from sample") {
        std::string_view input = "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 6);
        REQUIRE(state.final_values.size() == 4);

        // Verify final values
        CHECK(state.final_values[0] == 3);
        CHECK(state.final_values[1] == 5);
        CHECK(state.final_values[2] == 4);
        CHECK(state.final_values[3] == 7);

        // Verify transitions
        CHECK(state.transitions[0] == 0b1000);  // (3)
        CHECK(state.transitions[1] == 0b1010);  // (1,3)
        CHECK(state.transitions[2] == 0b0100);  // (2)
        CHECK(state.transitions[3] == 0b1100);  // (2,3)
        CHECK(state.transitions[4] == 0b0101);  // (0,2)
        CHECK(state.transitions[5] == 0b0011);  // (0,1)
    }
    
    TEST_CASE("parse_line_integer - empty parentheses") {
        std::string_view input = "[.##.] ()";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0);  // Empty parentheses = no bits set
    }
    
    TEST_CASE("parse_line_integer - with whitespace") {
        std::string_view input = "  [.##.]   (3)   (1,3)  {5,10}  ";
        auto state = parse_line_integer(input);
        
        CHECK(state.nr_digits == 4);
        REQUIRE(state.transitions.size() == 2);
        REQUIRE(state.final_values.size() == 2);
        CHECK(state.final_values[0] == 5);
        CHECK(state.final_values[1] == 10);
    }
    
    TEST_CASE("parse_line_integer - longer initial state") {
        std::string_view input = "[.#.#.#.#] (0,2,4,6)";
        auto state = parse_line_integer(input);

        CHECK(state.nr_digits == 8);
        REQUIRE(state.transitions.size() == 1);
        CHECK(state.transitions[0] == 0b01010101);  // Bits 0, 2, 4, 6 set
    }
    
    TEST_CASE("parse_line_integer - curly braces with single value") {
        std::string_view input = "[.##.] {42}";
        auto state = parse_line_integer(input);
        
        CHECK(state.nr_digits == 4);
        CHECK(state.transitions.empty());
        REQUIRE(state.final_values.size() == 1);
        CHECK(state.final_values[0] == 42);
    }
    
    TEST_CASE("parse_line_integer - curly braces with large values") {
        std::string_view input = "[.##.] {100,200,255}";
        auto state = parse_line_integer(input);
        
        CHECK(state.nr_digits == 4);
        REQUIRE(state.final_values.size() == 3);
        CHECK(state.final_values[0] == 100);
        CHECK(state.final_values[1] == 200);
        CHECK(state.final_values[2] == 255);
    }
    
    TEST_CASE("parse_input_integer - empty string") {
        std::string_view input = "";
        auto states = parse_input_integer(input);
        
        CHECK(states.empty());
    }
    
    TEST_CASE("parse_input_integer - single line") {
        std::string_view input = "[.##.] (3) (1,3) {5,7}";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 1);
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].transitions.size() == 2);
        CHECK(states[0].final_values.size() == 2);
    }
    
    TEST_CASE("parse_input_integer - multiple lines") {
        std::string_view input = "[.##.] (3) {5}\n[####] (0,1) {10,20}\n[....] (2,3) {1,2,3}";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 3);
        
        // First line
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].transitions.size() == 1);
        CHECK(states[0].final_values.size() == 1);
        CHECK(states[0].final_values[0] == 5);
        
        // Second line
        CHECK(states[1].nr_digits == 4);
        CHECK(states[1].transitions.size() == 1);
        CHECK(states[1].final_values.size() == 2);
        CHECK(states[1].final_values[0] == 10);
        CHECK(states[1].final_values[1] == 20);
        
        // Third line
        CHECK(states[2].nr_digits == 4);
        CHECK(states[2].transitions.size() == 1);
        CHECK(states[2].final_values.size() == 3);
        CHECK(states[2].final_values[0] == 1);
        CHECK(states[2].final_values[1] == 2);
        CHECK(states[2].final_values[2] == 3);
    }
    
    TEST_CASE("parse_input_integer - with empty lines") {
        std::string_view input = "[.##.] (3) {5}\n\n[####] (0,1) {10}\n  \n[....] (2,3) {1}";
        auto states = parse_input_integer(input);
        
        // Empty lines should be skipped
        REQUIRE(states.size() == 3);
    }
    
    TEST_CASE("parse_input_integer - no trailing newline") {
        std::string_view input = "[.##.] (3) {5}\n[####] (0,1) {10}";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 2);
        CHECK(states[1].nr_digits == 4);
        CHECK(states[1].final_values[0] == 10);
    }
    
    TEST_CASE("parse_input_integer - with trailing newline") {
        std::string_view input = "[.##.] (3) {5}\n[####] (0,1) {10}\n";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 2);
    }
    
    TEST_CASE("parse_input_integer - complex multi-line example") {
        std::string_view input =
            "[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
            "[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
            "[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 3);
        
        // First line
        CHECK(states[0].nr_digits == 4);
        CHECK(states[0].transitions.size() == 6);
        CHECK(states[0].final_values.size() == 4);
        
        // Second line
        CHECK(states[1].nr_digits == 5);
        CHECK(states[1].transitions.size() == 5);
        CHECK(states[1].final_values.size() == 5);
        
        // Third line
        CHECK(states[2].nr_digits == 6);
        CHECK(states[2].transitions.size() == 4);
        CHECK(states[2].final_values.size() == 6);
    }
    
    TEST_CASE("parse_input_integer - lines with only whitespace are skipped") {
        std::string_view input = "   \n\t\t\n[.##.] (3) {5}\n   \r\n";
        auto states = parse_input_integer(input);
        
        REQUIRE(states.size() == 1);
        CHECK(states[0].nr_digits == 4);
    }
}
