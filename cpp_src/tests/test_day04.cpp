#include "doctest.h"
#include "day04.hpp"

using namespace day04;

// Test suite for parse_binary_image function
TEST_SUITE("Day04 Tests") {
    TEST_CASE("parse_binary_image - basic example from documentation") {
        std::string_view input = "..@@\n@@..";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 8);
        REQUIRE(pitch == 4);
        
        // First line: ..@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 0);
        CHECK(result[2] == 1);
        CHECK(result[3] == 1);
        
        // Second line: @@..
        CHECK(result[4] == 1);
        CHECK(result[5] == 1);
        CHECK(result[6] == 0);
        CHECK(result[7] == 0);
    }
    
    TEST_CASE("parse_binary_image - single line") {
        std::string_view input = ".@.@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 5);
        REQUIRE(pitch == 5);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 0);
        CHECK(result[3] == 1);
        CHECK(result[4] == 0);
    }
    
    TEST_CASE("parse_binary_image - empty string") {
        std::string_view input = "";
        auto [result, pitch] = parse_binary_image(input);
        
        CHECK(result.size() == 0);
        CHECK(pitch == 0);
    }
    
    TEST_CASE("parse_binary_image - single character dot") {
        std::string_view input = ".";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 1);
        REQUIRE(pitch == 1);
        CHECK(result[0] == 0);
    }
    
    TEST_CASE("parse_binary_image - leading whitespace") {
        std::string_view input = "  .@@\n  @@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 6);
        REQUIRE(pitch == 3);
        
        // First line: .@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        
        // Second line: @@.
        CHECK(result[3] == 1);
        CHECK(result[4] == 1);
        CHECK(result[5] == 0);
    }
    
    TEST_CASE("parse_binary_image - trailing newlines") {
        std::string_view input = ".@\n@.\n\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - Windows line endings (CRLF)") {
        std::string_view input = ".@\r\n@.\r\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - tabs and spaces") {
        std::string_view input = "\t.@\n  @.\t\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - multiple consecutive newlines") {
        std::string_view input = ".@\n\n\n@.";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 4);
        REQUIRE(pitch == 2);
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        CHECK(result[3] == 0);
    }
    
    TEST_CASE("parse_binary_image - mixed whitespace") {
        std::string_view input = "  \n\t\n  .@@\n\n  @@.  \n\n";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 6);
        REQUIRE(pitch == 3);
        
        // First line: .@@
        CHECK(result[0] == 0);
        CHECK(result[1] == 1);
        CHECK(result[2] == 1);
        
        // Second line: @@.
        CHECK(result[3] == 1);
        CHECK(result[4] == 1);
        CHECK(result[5] == 0);
    }
    
    TEST_CASE("parse_binary_image - inconsistent pitch") {
        std::string_view input = "..\n@@@";
        auto [result, pitch] = parse_binary_image(input);
        
        REQUIRE(result.size() == 0);
        REQUIRE(pitch == 0);
    }
}