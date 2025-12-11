#include "doctest.h"
#include "day03.hpp"

using namespace day03;

// Test suite for parse_digit_lines function
TEST_SUITE("Day03 Tests") {
    TEST_CASE("day03_pt2 - sample input") {
        std::string_view input = R"(987654321111111
811111111111119
234234234234278
818181911112111)";
        uint64_t result = day03_pt2(input);

        CHECK(result == 3121910778619);
    }

    TEST_CASE("day03_pt1 - sample input") {
        std::string_view input = R"(987654321111111
811111111111119
234234234234278
818181911112111)";
        uint32_t result = day03_pt1(input);

        CHECK(result == 357);
    }

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

    TEST_CASE("highest_digit_pair - highest digits at start") {
        std::vector<uint8_t> input = { 9, 8, 7, 6 };

        auto result = highest_digit_pair(input);
        CHECK(result == 98);
    }

    TEST_CASE("highest_digit_pair - highest digits at start and end") {
        std::vector<uint8_t> input = { 9, 5, 5, 8 };

        auto result = highest_digit_pair(input);
        CHECK(result == 98);
    }

    TEST_CASE("highest_digit_pair - highest digits at end") {
        std::vector<uint8_t> input = { 4, 5, 7, 8 };

        auto result = highest_digit_pair(input);
        CHECK(result == 78);
    }

    TEST_CASE("highest_digit_pair - highest digits in middle") {
        std::vector<uint8_t> input = { 8, 9, 1, 2, 1 };

        auto result = highest_digit_pair(input);
        CHECK(result == 92);
    }

    TEST_CASE("highest_digit_12_digits - highest 12 digits at start") {
        std::vector<uint8_t> input = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1 };

        auto result = highest_digit_12_digits(input);
        CHECK(result == 987654321111);
    }

    TEST_CASE("highest_digit_12_digits - highest 12 digits at start and end") {
        std::vector<uint8_t> input = { 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9 };

        auto result = highest_digit_12_digits(input);
        CHECK(result == 811111111119);
    }

    TEST_CASE("highest_digit_12_digits - highest 12 digits in middle") {
        std::vector<uint8_t> input = { 2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 7, 8 };

        auto result = highest_digit_12_digits(input);
        CHECK(result == 434234234278);
    }

    TEST_CASE("highest_digit_12_digits - highest 12 at beginning") {
        std::vector<uint8_t> input = { 8, 1, 8, 1, 8, 1, 9, 1, 1, 1, 1, 2, 1, 1, 1 };

        auto result = highest_digit_12_digits(input);
        CHECK(result == 888911112111);
    }
}