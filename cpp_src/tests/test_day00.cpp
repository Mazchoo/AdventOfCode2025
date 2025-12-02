#include "doctest.h"
#include "day00.hpp"

using namespace day00;

// Test suite for basic add function
TEST_CASE("Testing add function") {
    CHECK(add(2, 3) == 5);
    CHECK(add(0, 0) == 0);
    CHECK(add(-1, 1) == 0);
    CHECK(add(-5, -3) == -8);
    CHECK(add(100, 200) == 300);
}

// Test suite for FloatVector class
TEST_SUITE("FloatVector Tests") {
    TEST_CASE("FloatVector creation and size") {
        FloatVector vec(10);
        CHECK(vec.get_size() == 10);
        
        FloatVector empty_vec(0);
        CHECK(empty_vec.get_size() == 0);
        
        FloatVector negative_vec(-5);
        CHECK(negative_vec.get_size() == 0);
    }
    
    TEST_CASE("FloatVector set and get elements") {
        FloatVector vec(5);
        
        // Test setting and getting valid elements
        CHECK(vec.set_element(0, 1.5f) == true);
        CHECK(vec.get_element(0) == doctest::Approx(1.5f));
        
        CHECK(vec.set_element(4, 9.9f) == true);
        CHECK(vec.get_element(4) == doctest::Approx(9.9f));
        
        // Test out of bounds access
        CHECK(vec.set_element(-1, 5.0f) == false);
        CHECK(vec.set_element(5, 5.0f) == false);
        CHECK(vec.get_element(-1) == 0.0f);
        CHECK(vec.get_element(5) == 0.0f);
    }
    
    TEST_CASE("FloatVector sum") {
        FloatVector vec(5);
        
        // Initially all zeros
        CHECK(vec.sum() == doctest::Approx(0.0f));
        
        // Set some values
        vec.set_element(0, 1.0f);
        vec.set_element(1, 2.0f);
        vec.set_element(2, 3.0f);
        vec.set_element(3, 4.0f);
        vec.set_element(4, 5.0f);
        
        CHECK(vec.sum() == doctest::Approx(15.0f));
    }
}

// Test suite for FloatVector factory functions
TEST_SUITE("FloatVector Factory Functions") {
    TEST_CASE("create and free float vector") {
        FloatVector* vec = create_float_vector(10);
        REQUIRE(vec != nullptr);
        CHECK(vec->get_size() == 10);
        free_float_vector(vec);
    }
    
    TEST_CASE("sum_float_vector") {
        FloatVector* vec = create_float_vector(3);
        REQUIRE(vec != nullptr);
        
        set_vector_element(vec, 0, 1.0f);
        set_vector_element(vec, 1, 2.0f);
        set_vector_element(vec, 2, 3.0f);
        
        CHECK(sum_float_vector(vec) == doctest::Approx(6.0f));
        CHECK(sum_float_vector(nullptr) == 0.0f);
        
        free_float_vector(vec);
    }
    
    TEST_CASE("set and get vector element via pointers") {
        FloatVector* vec = create_float_vector(5);
        REQUIRE(vec != nullptr);
        
        CHECK(set_vector_element(vec, 2, 7.5f) == true);
        CHECK(get_vector_element(vec, 2) == doctest::Approx(7.5f));
        
        CHECK(set_vector_element(nullptr, 0, 1.0f) == false);
        CHECK(get_vector_element(nullptr, 0) == 0.0f);
        
        free_float_vector(vec);
    }
}

// Test suite for raw float array functions
TEST_SUITE("Float Array Tests") {
    TEST_CASE("create and free float array") {
        float* arr = create_float_array(10);
        REQUIRE(arr != nullptr);
        
        // Check initialization to zero
        CHECK(arr[0] == 0.0f);
        CHECK(arr[9] == 0.0f);
        
        free_float_array(arr);
        
        // Test invalid size
        float* invalid = create_float_array(0);
        CHECK(invalid == nullptr);
        
        invalid = create_float_array(-5);
        CHECK(invalid == nullptr);
    }
    
    TEST_CASE("sum_float_array") {
        float* arr = create_float_array(5);
        REQUIRE(arr != nullptr);
        
        // Initially all zeros
        CHECK(sum_float_array(arr, 5) == doctest::Approx(0.0f));
        
        // Set values
        arr[0] = 1.5f;
        arr[1] = 2.5f;
        arr[2] = 3.5f;
        arr[3] = 4.5f;
        arr[4] = 5.5f;
        
        CHECK(sum_float_array(arr, 5) == doctest::Approx(17.5f));
        
        // Test null pointer
        CHECK(sum_float_array(nullptr, 5) == 0.0f);
        
        // Test invalid size
        CHECK(sum_float_array(arr, 0) == 0.0f);
        CHECK(sum_float_array(arr, -1) == 0.0f);
        
        free_float_array(arr);
    }
    
    TEST_CASE("set and get array element") {
        float* arr = create_float_array(5);
        REQUIRE(arr != nullptr);
        
        CHECK(set_array_element(arr, 0, 10.5f) == true);
        CHECK(get_array_element(arr, 0) == doctest::Approx(10.5f));
        
        CHECK(set_array_element(arr, 4, 20.5f) == true);
        CHECK(get_array_element(arr, 4) == doctest::Approx(20.5f));
        
        // Test null pointer
        CHECK(set_array_element(nullptr, 0, 1.0f) == false);
        CHECK(get_array_element(nullptr, 0) == 0.0f);
        
        // Test negative index
        CHECK(set_array_element(arr, -1, 1.0f) == false);
        CHECK(get_array_element(arr, -1) == 0.0f);
        
        free_float_array(arr);
    }
}

// Test suite for byte array functions
TEST_SUITE("Byte Array Tests") {
    TEST_CASE("create and free byte array") {
        uint8_t* arr = create_byte_array(10);
        REQUIRE(arr != nullptr);
        
        // Check initialization to zero
        CHECK(arr[0] == 0);
        CHECK(arr[9] == 0);
        
        free_byte_array(arr);
        
        // Test invalid size
        uint8_t* invalid = create_byte_array(0);
        CHECK(invalid == nullptr);
        
        invalid = create_byte_array(-5);
        CHECK(invalid == nullptr);
    }
    
    TEST_CASE("sum_byte_array") {
        uint8_t* arr = create_byte_array(5);
        REQUIRE(arr != nullptr);
        
        // Initially all zeros
        CHECK(sum_byte_array(arr, 5) == 0);
        
        // Set values
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;
        arr[4] = 50;
        
        CHECK(sum_byte_array(arr, 5) == 150);
        
        // Test null pointer
        CHECK(sum_byte_array(nullptr, 5) == 0);
        
        // Test invalid size
        CHECK(sum_byte_array(arr, 0) == 0);
        CHECK(sum_byte_array(arr, -1) == 0);
        
        free_byte_array(arr);
    }
    
    TEST_CASE("sum_byte_array_simd") {
        // Test with size that's a multiple of 16
        uint8_t* arr = create_byte_array(32);
        REQUIRE(arr != nullptr);
        
        // Set all values to 1
        for (int i = 0; i < 32; i++) {
            arr[i] = 1;
        }
        
        CHECK(sum_byte_array_simd(arr, 32) == 32);
        
        // Test with non-multiple of 16
        uint8_t* arr2 = create_byte_array(20);
        REQUIRE(arr2 != nullptr);
        
        for (int i = 0; i < 20; i++) {
            arr2[i] = 5;
        }
        
        CHECK(sum_byte_array_simd(arr2, 20) == 100);
        
        // Test null pointer
        CHECK(sum_byte_array_simd(nullptr, 5) == 0);
        
        free_byte_array(arr);
        free_byte_array(arr2);
    }
    
    TEST_CASE("set and get byte array element") {
        uint8_t* arr = create_byte_array(5);
        REQUIRE(arr != nullptr);
        
        CHECK(set_byte_array_element(arr, 0, 100) == true);
        CHECK(get_byte_array_element(arr, 0) == 100);
        
        CHECK(set_byte_array_element(arr, 4, 255) == true);
        CHECK(get_byte_array_element(arr, 4) == 255);
        
        // Test null pointer
        CHECK(set_byte_array_element(nullptr, 0, 1) == false);
        CHECK(get_byte_array_element(nullptr, 0) == 0);
        
        // Test negative index
        CHECK(set_byte_array_element(arr, -1, 1) == false);
        CHECK(get_byte_array_element(arr, -1) == 0);
        
        free_byte_array(arr);
    }
    
    TEST_CASE("byte array overflow handling") {
        uint8_t* arr = create_byte_array(3);
        REQUIRE(arr != nullptr);
        
        // Set values that would overflow uint8_t individually but not int
        arr[0] = 200;
        arr[1] = 200;
        arr[2] = 200;
        
        // Sum should be 600 (returned as int to avoid overflow)
        CHECK(sum_byte_array(arr, 3) == 600);
        CHECK(sum_byte_array_simd(arr, 3) == 600);
        
        free_byte_array(arr);
    }
}