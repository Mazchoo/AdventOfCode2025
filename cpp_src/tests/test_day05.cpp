#include "doctest.h"
#include "day05.hpp"

using namespace day05;

// Test suite for parse_ranges_and_ids function
TEST_SUITE("Day05 Tests") {
    TEST_CASE("count_all_valid_ids - sample input") {
        std::string_view input = R"(3-5
10-14
16-20
12-18)";
        auto result = count_all_valid_ids(input);

        REQUIRE(result == 14);
    }

    TEST_CASE("count_all_valid_ids - indepent ranges") {
        std::string_view input = R"(100-200
300-400)";
        auto result = count_all_valid_ids(input);

        REQUIRE(result == 202);
    }

    TEST_CASE("count_all_valid_ids - all fusing") {
        std::string_view input = R"(5-40
50-100
150-200
30-60
40-50
0-5
100-150)";
        auto result = count_all_valid_ids(input);

        REQUIRE(result == 201);
    }


    TEST_CASE("parse_ranges_and_ids - basic example from documentation") {
        std::string_view input = R"(3-5
10-14
16-20
12-18

1
5
8
11
17
32)";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 4);
        REQUIRE(ids.size() == 6);
        
        // Check ranges
        CHECK(ranges[0].first == 3);
        CHECK(ranges[0].second == 5);
        CHECK(ranges[1].first == 10);
        CHECK(ranges[1].second == 14);
        CHECK(ranges[2].first == 16);
        CHECK(ranges[2].second == 20);
        CHECK(ranges[3].first == 12);
        CHECK(ranges[3].second == 18);
        
        // Check IDs
        CHECK(ids[0] == 1);
        CHECK(ids[1] == 5);
        CHECK(ids[2] == 8);
        CHECK(ids[3] == 11);
        CHECK(ids[4] == 17);
        CHECK(ids[5] == 32);
    }
    
    TEST_CASE("parse_ranges_and_ids - single range and single ID") {
        std::string_view input = R"(1-10

5)";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 1);
        REQUIRE(ids.size() == 1);
        
        CHECK(ranges[0].first == 1);
        CHECK(ranges[0].second == 10);
        CHECK(ids[0] == 5);
    }
    
    TEST_CASE("parse_ranges_and_ids - multiple ranges, no IDs") {
        std::string_view input = R"(1-5
10-20

)";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 2);
        REQUIRE(ids.size() == 0);
        
        CHECK(ranges[0].first == 1);
        CHECK(ranges[0].second == 5);
        CHECK(ranges[1].first == 10);
        CHECK(ranges[1].second == 20);
    }
    
    TEST_CASE("parse_ranges_and_ids - empty string") {
        std::string_view input = "";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        CHECK(ranges.size() == 0);
        CHECK(ids.size() == 0);
    }
    
    TEST_CASE("parse_ranges_and_ids - only whitespace") {
        std::string_view input = "   \n\n\t\n  ";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        CHECK(ranges.size() == 0);
        CHECK(ids.size() == 0);
    }
 
    TEST_CASE("parse_ranges_and_ids - no ranges") {
        std::string_view input = R"(
  1
  5
  )";
        auto [ranges, ids] = parse_ranges_and_ids(input);

        REQUIRE(ranges.size() == 0);
        REQUIRE(ids.size() == 2);

        CHECK(ids[0] == 1);
        CHECK(ids[1] == 5);
    }

    TEST_CASE("parse_ranges_and_ids - leading and trailing whitespace") {
        std::string_view input = R"(  
  1-5
  10-20
  
  1
  5
  )";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 2);
        REQUIRE(ids.size() == 2);
        
        CHECK(ranges[0].first == 1);
        CHECK(ranges[0].second == 5);
        CHECK(ranges[1].first == 10);
        CHECK(ranges[1].second == 20);
        CHECK(ids[0] == 1);
        CHECK(ids[1] == 5);
    }
    
    TEST_CASE("parse_ranges_and_ids - Windows line endings (CRLF)") {
        std::string_view input = "1-5\r\n10-20\r\n\r\n1\r\n5\r\n";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 2);
        REQUIRE(ids.size() == 2);
        
        CHECK(ranges[0].first == 1);
        CHECK(ranges[0].second == 5);
        CHECK(ranges[1].first == 10);
        CHECK(ranges[1].second == 20);
        CHECK(ids[0] == 1);
        CHECK(ids[1] == 5);
    }
    
    TEST_CASE("parse_ranges_and_ids - large numbers") {
        std::string_view input = R"(1000000-9999999
5000000000-6000000000

123456789
9876543210)";
        auto [ranges, ids] = parse_ranges_and_ids(input);
        
        REQUIRE(ranges.size() == 2);
        REQUIRE(ids.size() == 2);
        
        CHECK(ranges[0].first == 1000000);
        CHECK(ranges[0].second == 9999999);
        CHECK(ranges[1].first == 5000000000);
        CHECK(ranges[1].second == 6000000000);
        CHECK(ids[0] == 123456789);
        CHECK(ids[1] == 9876543210);
    }

    // Test suite for fuse_two_ranges function
    TEST_CASE("fuse_two_ranges - overlapping ranges") {
        day05::Range range1 = {5, 15};
        day05::Range range2 = {10, 20};
        
        auto result = fuse_two_ranges(range1, range2);
        
        REQUIRE(result.has_value());
        CHECK(result->first == 5);
        CHECK(result->second == 20);
    }
    
    TEST_CASE("fuse_two_ranges - adjacent ranges") {
        day05::Range range1 = {5, 10};
        day05::Range range2 = {1, 5};
        
        auto result = fuse_two_ranges(range1, range2);
        
        REQUIRE(result.has_value());
        CHECK(result->first == 1);
        CHECK(result->second == 10);
    }
    
    TEST_CASE("fuse_two_ranges - non-overlapping ranges") {
        day05::Range range1 = {1, 5};
        day05::Range range2 = {10, 15};
        
        auto result = fuse_two_ranges(range1, range2);
        
        CHECK_FALSE(result.has_value());
    }
}