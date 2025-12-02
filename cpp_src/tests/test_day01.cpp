#include "doctest.h"
#include "day01.hpp"

using namespace day01;

// Test suite for parse_safe_adjustments function
TEST_SUITE("Day01 Tests") {
    TEST_CASE("day01_pt1 - sample input") {
        std::string input = R"(L68
            L30
            R48
            L5
            R60
            L55
            L1
            L99
            R14
            L82)";
        uint32_t result = day01_pt1(input);

        CHECK(result == 3);
    }

    TEST_CASE("parse_safe_adjustments - basic example from comments") {
        std::string input = "R880\nL35";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 880);
        CHECK(result[1] == -35);
    }
    
    TEST_CASE("parse_safe_adjustments - single R line") {
        std::string input = "R100";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 100);
    }
    
    TEST_CASE("parse_safe_adjustments - single L line") {
        std::string input = "L50";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 1);
        CHECK(result[0] == -50);
    }
    
    TEST_CASE("parse_safe_adjustments - multiple lines") {
        std::string input = "R100\nR200\nL50\nR75\nL25";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 5);
        CHECK(result[0] == 100);
        CHECK(result[1] == 200);
        CHECK(result[2] == -50);
        CHECK(result[3] == 75);
        CHECK(result[4] == -25);
    }
    
    TEST_CASE("parse_safe_adjustments - empty string") {
        std::string input = "";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        CHECK(result.size() == 0);
    }
    
    TEST_CASE("parse_safe_adjustments - zero values") {
        std::string input = "R0\nL0";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 0);
    }
    
    TEST_CASE("parse_safe_adjustments - large values") {
        std::string input = "R32767\nL32767";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 32767);
        CHECK(result[1] == -32767);
    }
    
    TEST_CASE("parse_safe_adjustments - whitespace handling") {
        std::string input = "  R100\n  L50  \nR25";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
    }
    
    TEST_CASE("parse_safe_adjustments - trailing newlines") {
        std::string input = "R100\nL50\n\n";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
    }
    
    TEST_CASE("parse_safe_adjustments - alternating R and L") {
        std::string input = "R10\nL10\nR20\nL20\nR30\nL30";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 6);
        CHECK(result[0] == 10);
        CHECK(result[1] == -10);
        CHECK(result[2] == 20);
        CHECK(result[3] == -20);
        CHECK(result[4] == 30);
        CHECK(result[5] == -30);
    }
    
    TEST_CASE("parse_safe_adjustments - Windows line endings (CRLF)") {
        std::string input = "R100\r\nL50\r\nR25";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
    }
    
    TEST_CASE("parse_safe_adjustments - Old Mac line endings (CR only)") {
        std::string input = "R100\rL50\rR25";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
    }
    
    TEST_CASE("parse_safe_adjustments - Mixed line endings") {
        std::string input = "R100\r\nL50\nR25\rL10";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 4);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
        CHECK(result[3] == -10);
    }
    
    TEST_CASE("parse_safe_adjustments - Multiple consecutive line endings") {
        std::string input = "R100\n\n\nL50\r\r\rR25";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
    }
    
    TEST_CASE("parse_safe_adjustments - Tabs and spaces with line endings") {
        std::string input = "\t  R100\r\n  \tL50  \n\t R25\r";
        std::vector<int16_t> result = parse_safe_adjustments(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0] == 100);
        CHECK(result[1] == -50);
        CHECK(result[2] == 25);
    }
}