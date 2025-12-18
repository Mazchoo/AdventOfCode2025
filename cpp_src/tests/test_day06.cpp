#include "doctest.h"
#include "day06.hpp"

using namespace day06;

// Test suite for parse_values_and_operations function
TEST_SUITE("Day06 Tests") {
    TEST_CASE("parse_column_values_and_operations - basic example from documentation") {
        std::string_view input = R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";
        auto [values, operations] = parse_column_values_and_operations(input);

        REQUIRE(values.size() == 4);
        REQUIRE(operations.size() == 4);
        
        // Check first row
        REQUIRE(values[0].size() == 3);
        CHECK(values[0][0] == 1);
        CHECK(values[0][1] == 24);
        CHECK(values[0][2] == 356);
        
        // Check second row
        REQUIRE(values[1].size() == 3);
        CHECK(values[1][0] == 369);
        CHECK(values[1][1] == 248);
        CHECK(values[1][2] == 8);
        
        // Check third row
        REQUIRE(values[2].size() == 3);
        CHECK(values[2][0] == 32);
        CHECK(values[2][1] == 581);
        CHECK(values[2][2] == 175);

        // Check fourth row
        REQUIRE(values[2].size() == 3);
        CHECK(values[3][0] == 623);
        CHECK(values[3][1] == 431);
        CHECK(values[3][2] == 4);

        // Check operations
        CHECK(operations[0] == Operation::Multiply);
        CHECK(operations[1] == Operation::Add);
        CHECK(operations[2] == Operation::Multiply);
        CHECK(operations[3] == Operation::Add);
        
    }

    TEST_CASE("parse_values_and_operations - basic example from documentation") {
        std::string_view input = R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";
        auto result = calculate_homework_total(input);
        REQUIRE(result == 4277556);
        
    }

    TEST_CASE("parse_values_and_operations - basic example from documentation") {
        std::string_view input = R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 3);
        REQUIRE(operations.size() == 4);
        
        // Check first row
        REQUIRE(values[0].size() == 4);
        CHECK(values[0][0] == 123);
        CHECK(values[0][1] == 328);
        CHECK(values[0][2] == 51);
        CHECK(values[0][3] == 64);
        
        // Check second row
        REQUIRE(values[1].size() == 4);
        CHECK(values[1][0] == 45);
        CHECK(values[1][1] == 64);
        CHECK(values[1][2] == 387);
        CHECK(values[1][3] == 23);
        
        // Check third row
        REQUIRE(values[2].size() == 4);
        CHECK(values[2][0] == 6);
        CHECK(values[2][1] == 98);
        CHECK(values[2][2] == 215);
        CHECK(values[2][3] == 314);
        
        // Check operations
        CHECK(operations[0] == Operation::Multiply);
        CHECK(operations[1] == Operation::Add);
        CHECK(operations[2] == Operation::Multiply);
        CHECK(operations[3] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - single row with operations") {
        std::string_view input = R"(10 20 30
+ * +)";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 1);
        REQUIRE(operations.size() == 3);
        
        CHECK(values[0].size() == 3);
        CHECK(values[0][0] == 10);
        CHECK(values[0][1] == 20);
        CHECK(values[0][2] == 30);
        
        CHECK(operations[0] == Operation::Add);
        CHECK(operations[1] == Operation::Multiply);
        CHECK(operations[2] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - large numbers") {
        std::string_view input = R"(1234567890 9876543210
5000000000 6000000000
* +)";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 2);
        REQUIRE(operations.size() == 2);
        
        CHECK(values[0][0] == 1234567890);
        CHECK(values[0][1] == 9876543210);
        CHECK(values[1][0] == 5000000000);
        CHECK(values[1][1] == 6000000000);
        
        CHECK(operations[0] == Operation::Multiply);
        CHECK(operations[1] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - extra whitespace") {
        std::string_view input = R"(  10   20    30  
  40   50    60  
  *    +    *   )";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 2);
        REQUIRE(operations.size() == 3);
        
        CHECK(values[0].size() == 3);
        CHECK(values[0][0] == 10);
        CHECK(values[0][1] == 20);
        CHECK(values[0][2] == 30);
        
        CHECK(values[1].size() == 3);
        CHECK(values[1][0] == 40);
        CHECK(values[1][1] == 50);
        CHECK(values[1][2] == 60);
        
        CHECK(operations[0] == Operation::Multiply);
        CHECK(operations[1] == Operation::Add);
        CHECK(operations[2] == Operation::Multiply);
    }
    
    TEST_CASE("parse_values_and_operations - Windows line endings (CRLF)") {
        std::string_view input = "10 20 30\r\n40 50 60\r\n+ * +\r\n";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 2);
        REQUIRE(operations.size() == 3);
        
        CHECK(values[0].size() == 3);
        CHECK(values[0][0] == 10);
        CHECK(values[0][1] == 20);
        CHECK(values[0][2] == 30);
        
        CHECK(values[1].size() == 3);
        CHECK(values[1][0] == 40);
        CHECK(values[1][1] == 50);
        CHECK(values[1][2] == 60);
        
        CHECK(operations[0] == Operation::Add);
        CHECK(operations[1] == Operation::Multiply);
        CHECK(operations[2] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - only operations") {
        std::string_view input = R"(+ * + * +)";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 0);
        REQUIRE(operations.size() == 5);
        
        CHECK(operations[0] == Operation::Add);
        CHECK(operations[1] == Operation::Multiply);
        CHECK(operations[2] == Operation::Add);
        CHECK(operations[3] == Operation::Multiply);
        CHECK(operations[4] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - single value per row") {
        std::string_view input = R"(1
2
3
4
+ * +)";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 4);
        REQUIRE(operations.size() == 3);
        
        CHECK(values[0].size() == 1);
        CHECK(values[0][0] == 1);
        CHECK(values[1][0] == 2);
        CHECK(values[2][0] == 3);
        CHECK(values[3][0] == 4);
        
        CHECK(operations[0] == Operation::Add);
        CHECK(operations[1] == Operation::Multiply);
        CHECK(operations[2] == Operation::Add);
    }
    
    TEST_CASE("parse_values_and_operations - tabs as separators") {
        std::string_view input = "10\t20\t30\n40\t50\t60\n+\t*\t+";
        auto [values, operations] = parse_values_and_operations(input);
        
        REQUIRE(values.size() == 2);
        REQUIRE(operations.size() == 3);
        
        CHECK(values[0].size() == 3);
        CHECK(values[0][0] == 10);
        CHECK(values[0][1] == 20);
        CHECK(values[0][2] == 30);
        
        CHECK(values[1].size() == 3);
        CHECK(values[1][0] == 40);
        CHECK(values[1][1] == 50);
        CHECK(values[1][2] == 60);
        
        CHECK(operations[0] == Operation::Add);
        CHECK(operations[1] == Operation::Multiply);
        CHECK(operations[2] == Operation::Add);
    }
}