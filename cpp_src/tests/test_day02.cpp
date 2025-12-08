#include "doctest.h"
#include "day02.hpp"

using namespace day02;

// Test suite for parse_integer_pairs function
TEST_SUITE("Day02 Tests") {
    TEST_CASE("parse_integer_pairs - basic example") {
        std::string_view input = "1111-2222,3333-4444";
        std::vector<std::pair<uint64_t, uint64_t>> result = parse_integer_pairs(input);
        
        REQUIRE(result.size() == 2);
        CHECK(result[0].first == 1111);
        CHECK(result[0].second == 2222);
        CHECK(result[1].first == 3333);
        CHECK(result[1].second == 4444);
    }
    
    TEST_CASE("parse_integer_pairs - single pair") {
        std::string_view input = "100-200";
        std::vector<std::pair<uint64_t, uint64_t>> result = parse_integer_pairs(input);
        
        REQUIRE(result.size() == 1);
        CHECK(result[0].first == 100);
        CHECK(result[0].second == 200);
    }
    
    TEST_CASE("parse_integer_pairs - multiple pairs with whitespace") {
        std::string_view input = "10-20, 30-40, 50-60";
        std::vector<std::pair<uint64_t, uint64_t>> result = parse_integer_pairs(input);
        
        REQUIRE(result.size() == 3);
        CHECK(result[0].first == 10);
        CHECK(result[0].second == 20);
        CHECK(result[1].first == 30);
        CHECK(result[1].second == 40);
        CHECK(result[2].first == 50);
        CHECK(result[2].second == 60);
    }
}