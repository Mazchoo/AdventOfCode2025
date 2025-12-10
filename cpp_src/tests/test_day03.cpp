#include "doctest.h"
#include "day03.hpp"

using namespace day03;

// Test suite for parse_digit_lines function
TEST_SUITE("Day03 Tests") {
    TEST_CASE("parse_digit_lines - basic example") {
        std::string_view input = "1234\n6789";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].size() == 4);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        CHECK(result[0][3] == 4);
        
        REQUIRE(result[1].size() == 4);
        CHECK(result[1][0] == 6);
        CHECK(result[1][1] == 7);
        CHECK(result[1][2] == 8);
        CHECK(result[1][3] == 9);
    }
    
    TEST_CASE("parse_digit_lines - single line") {
        std::string_view input = "12345";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 1);
        REQUIRE(result[0].size() == 5);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        CHECK(result[0][3] == 4);
        CHECK(result[0][4] == 5);
    }
    
    TEST_CASE("parse_digit_lines - multiple lines with varying lengths") {
        std::string_view input = "123\n45678\n9";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 3);
        
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 5);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
        CHECK(result[1][3] == 7);
        CHECK(result[1][4] == 8);
        
        REQUIRE(result[2].size() == 1);
        CHECK(result[2][0] == 9);
    }
    
    TEST_CASE("parse_digit_lines - empty string") {
        std::string_view input = "";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        CHECK(result.size() == 0);
    }
    
    TEST_CASE("parse_digit_lines - single digit per line") {
        std::string_view input = "1\n2\n3\n4\n5";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 5);
        CHECK(result[0][0] == 1);
        CHECK(result[1][0] == 2);
        CHECK(result[2][0] == 3);
        CHECK(result[3][0] == 4);
        CHECK(result[4][0] == 5);
    }

    TEST_CASE("parse_digit_lines - leading whitespace") {
        std::string_view input = "  123\n  456";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
    }
    
    TEST_CASE("parse_digit_lines - trailing newlines") {
        std::string_view input = "123\n456\n\n";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
    }
    
    TEST_CASE("parse_digit_lines - Windows line endings (CRLF)") {
        std::string_view input = "123\r\n456\r\n789";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
        
        REQUIRE(result[2].size() == 3);
        CHECK(result[2][0] == 7);
        CHECK(result[2][1] == 8);
        CHECK(result[2][2] == 9);
    }
    
    TEST_CASE("parse_digit_lines - tabs and spaces") {
        std::string_view input = "\t123\n  456\t\n789";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
        
        REQUIRE(result[2].size() == 3);
        CHECK(result[2][0] == 7);
        CHECK(result[2][1] == 8);
        CHECK(result[2][2] == 9);
    }
    
    TEST_CASE("parse_digit_lines - multiple consecutive newlines") {
        std::string_view input = "123\n\n\n456";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
    }
    
    TEST_CASE("parse_digit_lines - mixed whitespace and newlines") {
        std::string_view input = "  \n\t\n  123\n\n  456  \n\n";
        std::vector<std::vector<uint8_t>> result = parse_digit_lines(input);
        
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].size() == 3);
        CHECK(result[0][0] == 1);
        CHECK(result[0][1] == 2);
        CHECK(result[0][2] == 3);
        
        REQUIRE(result[1].size() == 3);
        CHECK(result[1][0] == 4);
        CHECK(result[1][1] == 5);
        CHECK(result[1][2] == 6);
    }
}