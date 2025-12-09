#include "doctest.h"
#include "day02.hpp"

using namespace day02;

// Test suite for parse_integer_pairs function
TEST_SUITE("Day02 Tests") {
    TEST_CASE("day01_pt2 - sample input") {
        std::string_view input = R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
1698522-1698528,446443-446449,38593856-38593862,565653-565659,
824824821-824824827,2121212118-2121212124)";
        uint32_t result = day02_pt2(input);

        CHECK(result == 4174379265);
    }


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

    TEST_CASE("get_first_nth_digits - edge cases") {
        // Single digit: 5 -> first 0 digits = 5 (1/2 = 0, divisor = 1)
        CHECK(get_first_nth_digits(55, 2) == 5);
        
        // Large number: 18 digits, n=2 (half)
        CHECK(get_first_nth_digits(123456789012345678ULL, 2) == 123456789ULL);
        
        // Number with leading zeros in second half: 100001 (6 digits) -> 100
        CHECK(get_first_nth_digits(100001, 2) == 100);
        
        // All same digits: 111111 (6 digits) -> 111
        CHECK(get_first_nth_digits(111111, 2) == 111);
        
        // Test n=3: 123456 (6 digits) -> first 2 digits = 12
        CHECK(get_first_nth_digits(123456, 3) == 12);
        
        // Test n=3: 123456789 (9 digits) -> first 3 digits = 123
        CHECK(get_first_nth_digits(123456789, 3) == 123);
        
        // Test n=4: 12345678 (8 digits) -> first 2 digits = 12
        CHECK(get_first_nth_digits(12345678, 4) == 12);
        
        // Test n=1: should return the full number (digits/1 = digits)
        CHECK(get_first_nth_digits(123456, 1) == 123456);
    }

    TEST_CASE("repeat_number - basic examples") {
        // 111 repeated 2 times -> 111111
        CHECK(repeat_number(111, 2) == 111111);
        
        // 42 repeated 2 times -> 4242
        CHECK(repeat_number(42, 2) == 4242);
        
        // Single digit: 7 repeated 2 times -> 77
        CHECK(repeat_number(7, 2) == 77);
        
        // Larger number: 12345 repeated 2 times -> 1234512345
        CHECK(repeat_number(12345, 2) == 1234512345ULL);
        
        // 42 repeated 3 times -> 424242
        CHECK(repeat_number(42, 3) == 424242);
        
        // 5 repeated 4 times -> 5555
        CHECK(repeat_number(5, 4) == 5555);
        
        // 123 repeated 3 times -> 123123123
        CHECK(repeat_number(123, 3) == 123123123);
        
        // Edge case: n = 1 should return the value itself
        CHECK(repeat_number(999, 1) == 999);
        
        // Edge case: n = 0 should return 0
        CHECK(repeat_number(123, 0) == 0);
    }

    TEST_CASE("count_repeated_in_range 11-22 returns 22") {
        std::pair<uint64_t, uint64_t> range = {11, 22};
        CHECK(count_repeated_in_range(range, 2) == 33);
    }

    TEST_CASE("count_repeated_in_range 95-115 returns 99") {
        std::pair<uint64_t, uint64_t> range = { 95, 115 };
        CHECK(count_repeated_in_range(range, 2) == 99);
    }

    TEST_CASE("count_repeated_in_range 998-1012 returns 1010") {
        std::pair<uint64_t, uint64_t> range = { 998, 1012 };
        CHECK(count_repeated_in_range(range, 2) == 1010);
    }

    TEST_CASE("count_repeated_in_range 1188511880-1188511890 returns 1188511885") {
        std::pair<uint64_t, uint64_t> range = { 1188511880, 1188511890 };
        CHECK(count_repeated_in_range(range, 2) == 1188511885);
    }

    TEST_CASE("count_repeated_in_range 222220-222224 returns 222222") {
        std::pair<uint64_t, uint64_t> range = { 222220, 222224 };
        CHECK(count_repeated_in_range(range, 2) == 222222);
    }

    TEST_CASE("count_repeated_in_range 1698522-1698528 returns 0") {
        std::pair<uint64_t, uint64_t> range = { 1698522, 1698528 };
        CHECK(count_repeated_in_range(range, 2) == 0);
    }

    TEST_CASE("count_repeated_in_range 3-14 returns 11") {
        std::pair<uint64_t, uint64_t> range = { 3, 14 };
        CHECK(count_repeated_in_range(range, 2) == 11);
    }

    TEST_CASE("count_repeated_in_range 1-100 returns 495") {
        std::pair<uint64_t, uint64_t> range = { 1, 100 };
        CHECK(count_repeated_in_range(range, 2) == 495);
    }

    TEST_CASE("count_repeated_in_range 446443-446449 returns 446446") {
        std::pair<uint64_t, uint64_t> range = { 446443, 446449 };
        CHECK(count_repeated_in_range(range, 2) == 446446);
    }

    TEST_CASE("count_repeated_in_range 14-25 returns 22") {
        std::pair<uint64_t, uint64_t> range = { 17, 25 };
        CHECK(count_repeated_in_range(range, 2) == 22);
    }

    TEST_CASE("count_repeated_in_range 3 998-1012 returns 999") {
        std::pair<uint64_t, uint64_t> range = { 998, 1012 };
        CHECK(count_repeated_in_range(range, 3) == 999);
    }

    TEST_CASE("count_repeated_in_range 4 1000-1112 returns 1111") {
        std::pair<uint64_t, uint64_t> range = { 1000, 1112 };
        CHECK(count_repeated_in_range(range, 4) == 1111);
    }

    TEST_CASE("count_repeated_in_range 3 565653-565659 returns 565656") {
        std::pair<uint64_t, uint64_t> range = { 565653, 565659 };
        CHECK(count_repeated_in_range(range, 3) == 565656);
    }
}