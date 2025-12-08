#include "doctest.h"
#include "day02.hpp"

using namespace day02;

// Test suite for parse_integer_pairs function
TEST_SUITE("Day02 Tests") {
    TEST_CASE("day01_pt1 - sample input") {
        std::string_view input = R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
1698522-1698528,446443-446449,38593856-38593862,565653-565659,
824824821-824824827,2121212118-2121212124)";
        uint32_t result = day02_pt1(input);

        CHECK(result == 1227775554);
    }

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

    TEST_CASE("count_digits - boundary values") {
        // Just below powers of 10
        CHECK(count_digits(9ULL) == 1);
        CHECK(count_digits(99ULL) == 2);
        CHECK(count_digits(999ULL) == 3);
        CHECK(count_digits(9999ULL) == 4);
        CHECK(count_digits(99999ULL) == 5);
        CHECK(count_digits(999999ULL) == 6);
        CHECK(count_digits(9999999ULL) == 7);
        CHECK(count_digits(99999999ULL) == 8);
        CHECK(count_digits(999999999ULL) == 9);
        CHECK(count_digits(9999999999ULL) == 10);
        CHECK(count_digits(99999999999ULL) == 11);
        CHECK(count_digits(999999999999ULL) == 12);
        CHECK(count_digits(9999999999999ULL) == 13);
        CHECK(count_digits(99999999999999ULL) == 14);
        CHECK(count_digits(999999999999999ULL) == 15);
        CHECK(count_digits(9999999999999999ULL) == 16);
        CHECK(count_digits(99999999999999999ULL) == 17);
        CHECK(count_digits(999999999999999999ULL) == 18);
        CHECK(count_digits(9999999999999999999ULL) == 19);
    }

    TEST_CASE("count_digits - max uint64_t") {
        // Maximum value for uint64_t: 18,446,744,073,709,551,615 (20 digits)
        CHECK(count_digits(18446744073709551615ULL) == 20);
    }

    TEST_CASE("get_first_half_digits - edge cases") {
        // Single digit: 5 -> first 0 digits = 5 (1/2 = 0, divisor = 1)
        CHECK(get_first_half_digits(5) == 5);
        
        // Large number: 18 digits
        CHECK(get_first_half_digits(123456789012345678ULL) == 123456789ULL);
        
        // Number with leading zeros in second half: 100001 (6 digits) -> 100
        CHECK(get_first_half_digits(100001) == 100);
        
        // All same digits: 111111 (6 digits) -> 111
        CHECK(get_first_half_digits(111111) == 111);
    }

    TEST_CASE("repeat_number - basic examples") {
        // 111 -> 111111
        CHECK(repeat_number(111) == 111111);
        
        // 42 -> 4242
        CHECK(repeat_number(42) == 4242);
        
        // Single digit: 7 -> 77
        CHECK(repeat_number(7) == 77);
        
        // Larger number: 12345 -> 1234512345
        CHECK(repeat_number(12345) == 1234512345ULL);
    }

    TEST_CASE("count_repeated_in_range 11-22 returns 2") {
        std::pair<uint64_t, uint64_t> range = {11, 22};
        CHECK(count_repeated_in_range(range) == 33);
    }

    TEST_CASE("count_repeated_in_range 95-115 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 95, 115 };
        CHECK(count_repeated_in_range(range) == 99);
    }

    TEST_CASE("count_repeated_in_range 998-1012 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 998, 1012 };
        CHECK(count_repeated_in_range(range) == 1010);
    }

    TEST_CASE("count_repeated_in_range 1188511880-1188511890 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 1188511880, 1188511890 };
        CHECK(count_repeated_in_range(range) == 1188511885);
    }

    TEST_CASE("count_repeated_in_range 222220-222224 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 222220, 222224 };
        CHECK(count_repeated_in_range(range) == 222222);
    }

    TEST_CASE("count_repeated_in_range 1698522-1698528 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 1698522, 1698528 };
        CHECK(count_repeated_in_range(range) == 0);
    }

    TEST_CASE("count_repeated_in_range 3-14 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 3, 14 };
        CHECK(count_repeated_in_range(range) == 11);
    }

    TEST_CASE("count_repeated_in_range 1-100 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 1, 100 };
        CHECK(count_repeated_in_range(range) == 495);
    }

    TEST_CASE("count_repeated_in_range 446443-446449 returns 1") {
        std::pair<uint64_t, uint64_t> range = { 446443, 446449 };
        CHECK(count_repeated_in_range(range) == 446446);
    }
}