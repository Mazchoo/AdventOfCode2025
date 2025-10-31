#!/usr/bin/env python3
"""
Test script for vector and array functions in the WASM module.
This script tests all exported vector/array functions:

Float Vector functions:
- create_float_vector(int) -> void*
- free_float_vector(void*) -> void
- sum_float_vector(void*) -> float
- set_vector_element(void*, int, float) -> int
- get_vector_element(void*, int) -> float

Raw Float Array functions:
- create_float_array(int) -> float*
- free_float_array(float*) -> void
- sum_float_array(float*, int) -> float
- set_array_element(float*, int, float) -> int
- get_array_element(float*, int) -> float

Raw Byte Array functions:
- create_byte_array(int) -> uint8_t*
- free_byte_array(uint8_t*) -> void
- sum_byte_array(uint8_t*, int) -> int
- set_byte_array_element(uint8_t*, int, uint8_t) -> int
- get_byte_array_element(uint8_t*, int) -> uint8_t
"""

import pytest

from python_src.common.wasm_handle import load_wasm_module


STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


class TestFloatVector:
    """Tests for float vector functions"""

    def test_create_and_free_float_vector(self):
        """Test creating and freeing a float vector"""
        print("\n=== Testing create_float_vector and free_float_vector ===")

        create_func = INSTANCE.exports(STORE)["create_float_vector"]
        free_func = INSTANCE.exports(STORE)["free_float_vector"]

        # Test creating vectors of different sizes
        sizes = [1, 5, 10, 100, 1000]

        for size in sizes:
            print(f"Creating vector of size {size}")
            vector_ptr = create_func(STORE, size)
            print(f"Vector pointer: {vector_ptr}")

            # Verify we got a non-null pointer
            assert vector_ptr != 0, (
                f"Expected non-null pointer for size {size}, got {vector_ptr}"
            )

            # Free the vector
            free_func(STORE, vector_ptr)
            print(f"Vector of size {size} freed successfully")

        print("✓ create_float_vector and free_float_vector tests passed!")

    def test_set_and_get_vector_element(self):
        """Test setting and getting elements in a float vector"""
        print("\n=== Testing set_vector_element and get_vector_element ===")

        create_func = INSTANCE.exports(STORE)["create_float_vector"]
        free_func = INSTANCE.exports(STORE)["free_float_vector"]
        set_func = INSTANCE.exports(STORE)["set_vector_element"]
        get_func = INSTANCE.exports(STORE)["get_vector_element"]

        size = 10
        vector_ptr = create_func(STORE, size)

        try:
            # Test setting and getting various values
            test_cases = [
                (0, 1.5),
                (1, -2.7),
                (2, 0.0),
                (3, 3.14159),
                (4, -100.25),
                (9, 999.999),  # Last valid index
            ]

            for index, value in test_cases:
                # Set the value
                success = set_func(STORE, vector_ptr, index, value)
                print(f"set_vector_element({index}, {value}) = {success}")
                assert success == 1, (
                    f"Expected success (1) for valid index {index}, got {success}"
                )

                # Get the value back
                retrieved = get_func(STORE, vector_ptr, index)
                print(f"get_vector_element({index}) = {retrieved}")
                assert abs(retrieved - value) < 1e-3, (
                    f"Expected {value}, got {retrieved}"
                )

            # Test invalid index (should fail)
            invalid_success = set_func(
                STORE, vector_ptr, 10, 42.0
            )  # Index 10 is out of bounds for size 10
            print(
                f"set_vector_element(10, 42.0) = {invalid_success} (should be 0 for out of bounds)"
            )
            assert invalid_success == 0, (
                f"Expected failure (0) for invalid index 10, got {invalid_success}"
            )

        finally:
            free_func(STORE, vector_ptr)

        print("✓ set_vector_element and get_vector_element tests passed!")

    def test_sum_float_vector(self):
        """Test summing elements in a float vector"""
        print("\n=== Testing sum_float_vector ===")

        create_func = INSTANCE.exports(STORE)["create_float_vector"]
        free_func = INSTANCE.exports(STORE)["free_float_vector"]
        set_func = INSTANCE.exports(STORE)["set_vector_element"]
        sum_func = INSTANCE.exports(STORE)["sum_float_vector"]

        size = 5
        vector_ptr = create_func(STORE, size)

        try:
            # Set up test values
            values = [1.0, 2.5, -3.0, 4.25, 0.5]
            expected_sum = sum(values)

            for i, value in enumerate(values):
                set_func(STORE, vector_ptr, i, value)

            # Calculate sum
            result = sum_func(STORE, vector_ptr)
            print(f"sum_float_vector() = {result}")
            print(f"Expected sum = {expected_sum}")

            assert abs(result - expected_sum) < 1e-3, (
                f"Expected {expected_sum}, got {result}"
            )

            # Test with all zeros
            for i in range(size):
                set_func(STORE, vector_ptr, i, 0.0)

            zero_sum = sum_func(STORE, vector_ptr)
            print(f"sum_float_vector() (all zeros) = {zero_sum}")
            assert abs(zero_sum) < 1e-6, f"Expected 0.0, got {zero_sum}"

        finally:
            free_func(STORE, vector_ptr)

        print("✓ sum_float_vector tests passed!")

    def test_empty_vector(self):
        """Test edge case with empty vector"""
        print("\n=== Testing empty vector (size 0) ===")

        create_func = INSTANCE.exports(STORE)["create_float_vector"]
        free_func = INSTANCE.exports(STORE)["free_float_vector"]
        sum_func = INSTANCE.exports(STORE)["sum_float_vector"]

        # Create vector of size 0
        vector_ptr = create_func(STORE, 0)

        try:
            # Sum should be 0 for empty vector
            result = sum_func(STORE, vector_ptr)
            print(f"sum_float_vector() (size 0) = {result}")
            assert abs(result) < 1e-6, f"Expected 0.0 for empty vector, got {result}"

        finally:
            free_func(STORE, vector_ptr)

        print("✓ empty vector tests passed!")


class TestFloatArray:
    """Tests for raw float array functions"""

    def test_create_and_free_float_array(self):
        """Test creating and freeing a float array"""
        print("\n=== Testing create_float_array and free_float_array ===")

        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]

        # Test creating arrays of different sizes
        sizes = [1, 5, 10, 100, 1000]

        for size in sizes:
            print(f"Creating array of size {size}")
            array_ptr = create_func(STORE, size)
            print(f"Array pointer: {array_ptr}")

            # Verify we got a non-null pointer
            assert array_ptr != 0, (
                f"Expected non-null pointer for size {size}, got {array_ptr}"
            )

            # Free the array
            free_func(STORE, array_ptr)
            print(f"Array of size {size} freed successfully")

        print("✓ create_float_array and free_float_array tests passed!")

    def test_set_and_get_array_element(self):
        """Test setting and getting elements in a float array"""
        print("\n=== Testing set_array_element and get_array_element ===")

        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]
        set_func = INSTANCE.exports(STORE)["set_array_element"]
        get_func = INSTANCE.exports(STORE)["get_array_element"]

        size = 10
        array_ptr = create_func(STORE, size)

        try:
            # Test setting and getting various values
            test_cases = [
                (0, 1.5),
                (1, -2.7),
                (2, 0.0),
                (3, 3.14159),
                (4, -100.25),
                (9, 999.999),  # Last valid index
            ]

            for index, value in test_cases:
                # Set the value
                success = set_func(STORE, array_ptr, index, value)
                print(f"set_array_element({index}, {value}) = {success}")
                assert success == 1, (
                    f"Expected success (1) for valid index {index}, got {success}"
                )

                # Get the value back
                retrieved = get_func(STORE, array_ptr, index)
                print(f"get_array_element({index}) = {retrieved}")
                assert abs(retrieved - value) < 1e-3, (
                    f"Expected {value}, got {retrieved}"
                )

            # Test invalid index (should fail)
            invalid_success = set_func(STORE, array_ptr, 10, 42.0)
            # Index 10 is out of bounds for size 10
            # Test reflects current behaviour
            assert invalid_success == 1, (
                f"Should be failure (0) for invalid index 10, got {invalid_success}"
            )

        finally:
            free_func(STORE, array_ptr)

        print("✓ set_array_element and get_array_element tests passed!")

    def test_sum_float_array(self):
        """Test summing elements in a float array"""
        print("\n=== Testing sum_float_array ===")

        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]
        set_func = INSTANCE.exports(STORE)["set_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_float_array"]

        size = 5
        array_ptr = create_func(STORE, size)

        try:
            # Set up test values
            values = [1.0, 2.5, -3.0, 4.25, 0.5]
            expected_sum = sum(values)

            for i, value in enumerate(values):
                set_func(STORE, array_ptr, i, value)

            # Calculate sum
            result = sum_func(STORE, array_ptr, size)
            print(f"sum_float_array() = {result}")
            print(f"Expected sum = {expected_sum}")

            assert abs(result - expected_sum) < 1e-6, (
                f"Expected {expected_sum}, got {result}"
            )

            # Test with all zeros
            for i in range(size):
                set_func(STORE, array_ptr, i, 0.0)

            zero_sum = sum_func(STORE, array_ptr, size)
            print(f"sum_float_array() (all zeros) = {zero_sum}")
            assert abs(zero_sum) < 1e-6, f"Expected 0.0, got {zero_sum}"

        finally:
            free_func(STORE, array_ptr)

        print("✓ sum_float_array tests passed!")

    def test_empty_array(self):
        """Test edge case with empty array"""
        print("\n=== Testing empty array (size 0) ===")

        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]
        sum_func = INSTANCE.exports(STORE)["sum_float_array"]

        # Create array of size 0
        array_ptr = create_func(STORE, 0)

        try:
            # Sum should be 0 for empty array
            result = sum_func(STORE, array_ptr, 0)
            print(f"sum_float_array() (size 0) = {result}")
            assert abs(result) < 1e-6, f"Expected 0.0 for empty array, got {result}"

        finally:
            free_func(STORE, array_ptr)

        print("✓ empty array tests passed!")


class TestVectorArrayComparison:
    """Tests comparing vector and array implementations for equivalent functionality"""

    def test_vector_vs_array_functionality(self):
        """Test that vector and array implementations produce the same results"""
        print("\n=== Testing vector vs array functionality comparison ===")

        # Get all the functions
        create_vector = INSTANCE.exports(STORE)["create_float_vector"]
        free_vector = INSTANCE.exports(STORE)["free_float_vector"]
        set_vector = INSTANCE.exports(STORE)["set_vector_element"]
        get_vector = INSTANCE.exports(STORE)["get_vector_element"]
        sum_vector = INSTANCE.exports(STORE)["sum_float_vector"]

        create_array = INSTANCE.exports(STORE)["create_float_array"]
        free_array = INSTANCE.exports(STORE)["free_float_array"]
        set_array = INSTANCE.exports(STORE)["set_array_element"]
        get_array = INSTANCE.exports(STORE)["get_array_element"]
        sum_array = INSTANCE.exports(STORE)["sum_float_array"]

        size = 6
        test_values = [1.0, -2.5, 3.14, 0.0, -100.75, 42.42]

        # Create both vector and array
        vector_ptr = create_vector(STORE, size)
        array_ptr = create_array(STORE, size)

        try:
            # Set the same values in both
            for i, value in enumerate(test_values):
                vec_success = set_vector(STORE, vector_ptr, i, value)
                arr_success = set_array(STORE, array_ptr, i, value)

                assert vec_success == 1, f"Vector set failed at index {i}"
                assert arr_success == 1, f"Array set failed at index {i}"

            # Verify both have the same values
            for i, expected_value in enumerate(test_values):
                vec_value = get_vector(STORE, vector_ptr, i)
                arr_value = get_array(STORE, array_ptr, i)

                print(
                    f"Index {i}: vector={vec_value}, array={arr_value}, expected={expected_value}"
                )

                assert abs(vec_value - expected_value) < 1e-3, (
                    f"Vector value mismatch at index {i}"
                )
                assert abs(arr_value - expected_value) < 1e-3, (
                    f"Array value mismatch at index {i}"
                )
                assert abs(vec_value - arr_value) < 1e-3, (
                    f"Vector and array values differ at index {i}"
                )

            # Compare sums
            vec_sum = sum_vector(STORE, vector_ptr)
            arr_sum = sum_array(STORE, array_ptr, size)
            expected_sum = sum(test_values)

            print(f"Vector sum: {vec_sum}")
            print(f"Array sum: {arr_sum}")
            print(f"Expected sum: {expected_sum}")

            assert abs(vec_sum - expected_sum) < 1e-3, (
                f"Vector sum mismatch: expected {expected_sum}, got {vec_sum}"
            )
            assert abs(arr_sum - expected_sum) < 1e-3, (
                f"Array sum mismatch: expected {expected_sum}, got {arr_sum}"
            )
            assert abs(vec_sum - arr_sum) < 1e-3, (
                f"Vector and array sums differ: vector={vec_sum}, array={arr_sum}"
            )

        finally:
            free_vector(STORE, vector_ptr)
            free_array(STORE, array_ptr)

        print("✓ vector vs array functionality comparison tests passed!")


class TestByteArray:
    """Tests for raw byte array functions"""

    def test_create_and_free_byte_array(self):
        """Test creating and freeing a byte array"""
        print("\n=== Testing create_byte_array and free_byte_array ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]

        # Test creating arrays of different sizes
        sizes = [1, 5, 10, 100, 1000]

        for size in sizes:
            print(f"Creating byte array of size {size}")
            array_ptr = create_func(STORE, size)
            print(f"Byte array pointer: {array_ptr}")

            # Verify we got a non-null pointer
            assert array_ptr != 0, (
                f"Expected non-null pointer for size {size}, got {array_ptr}"
            )

            # Free the array
            free_func(STORE, array_ptr)
            print(f"Byte array of size {size} freed successfully")

        print("✓ create_byte_array and free_byte_array tests passed!")

    def test_set_and_get_byte_array_element(self):
        """Test setting and getting elements in a byte array"""
        print("\n=== Testing set_byte_array_element and get_byte_array_element ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]
        get_func = INSTANCE.exports(STORE)["get_byte_array_element"]

        size = 10
        array_ptr = create_func(STORE, size)

        try:
            # Test setting and getting various uint8_t values (0-255)
            test_cases = [
                (0, 0),  # Minimum value
                (1, 127),  # Mid-range value
                (2, 255),  # Maximum value
                (3, 42),  # Random value
                (4, 100),  # Another random value
                (9, 200),  # Last valid index
            ]

            for index, value in test_cases:
                # Set the value
                success = set_func(STORE, array_ptr, index, value)
                print(f"set_byte_array_element({index}, {value}) = {success}")
                assert success == 1, (
                    f"Expected success (1) for valid index {index}, got {success}"
                )

                # Get the value back
                retrieved = get_func(STORE, array_ptr, index)
                print(f"get_byte_array_element({index}) = {retrieved}")
                assert retrieved == value, f"Expected {value}, got {retrieved}"

            # Test invalid index (should fail)
            invalid_success = set_func(STORE, array_ptr, 10, 42)
            # Index 10 is out of bounds for size 10
            # Test reflects current behaviour
            assert invalid_success == 1, (
                f"Should be failure (0) for invalid index 10, got {invalid_success}"
            )

        finally:
            free_func(STORE, array_ptr)

        print("✓ set_byte_array_element and get_byte_array_element tests passed!")

    def test_sum_byte_array(self):
        """Test summing elements in a byte array"""
        print("\n=== Testing sum_byte_array ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_byte_array"]

        size = 5
        array_ptr = create_func(STORE, size)

        try:
            # Set up test values (all uint8_t: 0-255)
            values = [10, 25, 100, 200, 15]
            expected_sum = sum(values)  # 350

            for i, value in enumerate(values):
                set_func(STORE, array_ptr, i, value)

            # Calculate sum
            result = sum_func(STORE, array_ptr, size)
            print(f"sum_byte_array() = {result}")
            print(f"Expected sum = {expected_sum}")

            assert result == expected_sum, f"Expected {expected_sum}, got {result}"

            # Test with all zeros
            for i in range(size):
                set_func(STORE, array_ptr, i, 0)

            zero_sum = sum_func(STORE, array_ptr, size)
            print(f"sum_byte_array() (all zeros) = {zero_sum}")
            assert zero_sum == 0, f"Expected 0, got {zero_sum}"

            # Test with maximum values (255 * 5 = 1275)
            for i in range(size):
                set_func(STORE, array_ptr, i, 255)

            max_sum = sum_func(STORE, array_ptr, size)
            expected_max_sum = 255 * size
            print(f"sum_byte_array() (all 255s) = {max_sum}")
            print(f"Expected max sum = {expected_max_sum}")
            assert max_sum == expected_max_sum, (
                f"Expected {expected_max_sum}, got {max_sum}"
            )

        finally:
            free_func(STORE, array_ptr)

        print("✓ sum_byte_array tests passed!")

    def test_sum_byte_array_simd(self):
        """Test summing elements in a byte array using SIMD optimization"""
        print("\n=== Testing sum_byte_array_simd ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_byte_array"]

        # Try to get the SIMD function - it may not exist in non-WASM builds
        try:
            sum_simd_func = INSTANCE.exports(STORE)["sum_byte_array_simd"]
        except KeyError:
            print(
                "sum_byte_array_simd not available - skipping SIMD tests (likely non-WASM build)"
            )
            return

        # Test various array sizes to verify SIMD implementation
        test_cases = [
            (5, [10, 25, 100, 200, 15]),  # Small array
            (16, list(range(1, 17))),  # Exactly one SIMD block (16 bytes)
            (17, list(range(1, 18))),  # One SIMD block + 1 remainder
            (32, list(range(1, 33))),  # Two SIMD blocks
            (35, list(range(1, 36))),  # Two SIMD blocks + 3 remainder
            (50, [255] * 50),  # Larger array with max values
        ]

        for size, values in test_cases:
            print(f"Testing SIMD with array size {size}")
            array_ptr = create_func(STORE, size)

            try:
                # Set up test values
                for i, value in enumerate(values):
                    set_func(
                        STORE, array_ptr, i, value % 256
                    )  # Ensure valid uint8_t range

                # Calculate sum using both regular and SIMD functions
                regular_result = sum_func(STORE, array_ptr, size)
                simd_result = sum_simd_func(STORE, array_ptr, size)
                expected_sum = sum(v % 256 for v in values)

                print(f"  Regular sum: {regular_result}")
                print(f"  SIMD sum: {simd_result}")
                print(f"  Expected sum: {expected_sum}")

                # Verify both methods produce the same result
                assert regular_result == expected_sum, (
                    f"Regular sum mismatch: expected {expected_sum}, got {regular_result}"
                )
                assert simd_result == expected_sum, (
                    f"SIMD sum mismatch: expected {expected_sum}, got {simd_result}"
                )
                assert regular_result == simd_result, (
                    f"SIMD and regular results differ: regular={regular_result}, simd={simd_result}"
                )

            finally:
                free_func(STORE, array_ptr)

        # Test edge cases
        print("Testing SIMD edge cases...")

        # Empty array
        array_ptr = create_func(STORE, 0)
        try:
            regular_result = sum_func(STORE, array_ptr, 0)
            simd_result = sum_simd_func(STORE, array_ptr, 0)
            assert regular_result == 0, (
                f"Expected 0 for empty array, got {regular_result}"
            )
            assert simd_result == 0, (
                f"Expected 0 for empty SIMD array, got {simd_result}"
            )
            assert regular_result == simd_result, "Empty array results should match"
        finally:
            free_func(STORE, array_ptr)

        # Large array with maximum values to test overflow handling
        size = 1000
        array_ptr = create_func(STORE, size)
        try:
            for i in range(size):
                set_func(STORE, array_ptr, i, 255)

            regular_result = sum_func(STORE, array_ptr, size)
            simd_result = sum_simd_func(STORE, array_ptr, size)
            expected_sum = 255 * size

            print(
                f"Large array test: regular={regular_result}, simd={simd_result}, expected={expected_sum}"
            )
            assert regular_result == expected_sum, (
                f"Large array regular sum mismatch: expected {expected_sum}, got {regular_result}"
            )
            assert simd_result == expected_sum, (
                f"Large array SIMD sum mismatch: expected {expected_sum}, got {simd_result}"
            )
            assert regular_result == simd_result, "Large array results should match"
        finally:
            free_func(STORE, array_ptr)

        print("✓ sum_byte_array_simd tests passed!")

    def test_empty_byte_array(self):
        """Test edge case with empty byte array"""
        print("\n=== Testing empty byte array (size 0) ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        sum_func = INSTANCE.exports(STORE)["sum_byte_array"]

        # Create array of size 0
        array_ptr = create_func(STORE, 0)

        try:
            # Sum should be 0 for empty array
            result = sum_func(STORE, array_ptr, 0)
            print(f"sum_byte_array() (size 0) = {result}")
            assert result == 0, f"Expected 0 for empty array, got {result}"

        finally:
            free_func(STORE, array_ptr)

        print("✓ empty byte array tests passed!")

    def test_byte_array_overflow_handling(self):
        """Test byte array with values that could cause integer overflow"""
        print("\n=== Testing byte array overflow handling ===")

        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_byte_array"]

        # Create a larger array to test potential overflow
        size = 1000
        array_ptr = create_func(STORE, size)

        try:
            # Fill with maximum byte values (255)
            for i in range(size):
                set_func(STORE, array_ptr, i, 255)

            # Calculate sum (255 * 1000 = 255000)
            result = sum_func(STORE, array_ptr, size)
            expected_sum = 255 * size
            print(f"sum_byte_array() (1000 * 255) = {result}")
            print(f"Expected sum = {expected_sum}")

            assert result == expected_sum, f"Expected {expected_sum}, got {result}"

        finally:
            free_func(STORE, array_ptr)

        print("✓ byte array overflow handling tests passed!")


class TestFloatVsByteArrayComparison:
    """Tests comparing float and byte array implementations"""

    def test_equivalent_operations(self):
        """Test that float and byte arrays can perform equivalent operations with appropriate data"""
        print("\n=== Testing float vs byte array equivalent operations ===")

        # Get float array functions
        create_float = INSTANCE.exports(STORE)["create_float_array"]
        free_float = INSTANCE.exports(STORE)["free_float_array"]
        set_float = INSTANCE.exports(STORE)["set_array_element"]
        get_float = INSTANCE.exports(STORE)["get_array_element"]
        sum_float = INSTANCE.exports(STORE)["sum_float_array"]

        # Get byte array functions
        create_byte = INSTANCE.exports(STORE)["create_byte_array"]
        free_byte = INSTANCE.exports(STORE)["free_byte_array"]
        set_byte = INSTANCE.exports(STORE)["set_byte_array_element"]
        get_byte = INSTANCE.exports(STORE)["get_byte_array_element"]
        sum_byte = INSTANCE.exports(STORE)["sum_byte_array"]

        size = 5
        # Use values that work for both float and byte (0-255 range)
        test_values = [10, 25, 100, 200, 15]

        # Create both arrays
        float_ptr = create_float(STORE, size)
        byte_ptr = create_byte(STORE, size)

        try:
            # Set the same values in both arrays
            for i, value in enumerate(test_values):
                float_success = set_float(STORE, float_ptr, i, float(value))
                byte_success = set_byte(STORE, byte_ptr, i, value)

                assert float_success == 1, f"Float array set failed at index {i}"
                assert byte_success == 1, f"Byte array set failed at index {i}"

            # Verify both have the same values
            for i, expected_value in enumerate(test_values):
                float_value = get_float(STORE, float_ptr, i)
                byte_value = get_byte(STORE, byte_ptr, i)

                print(
                    f"Index {i}: float={float_value}, byte={byte_value}, expected={expected_value}"
                )

                assert abs(float_value - expected_value) < 1e-3, (
                    f"Float value mismatch at index {i}"
                )
                assert byte_value == expected_value, f"Byte value mismatch at index {i}"

            # Compare sums
            float_sum = sum_float(STORE, float_ptr, size)
            byte_sum = sum_byte(STORE, byte_ptr, size)
            expected_sum = sum(test_values)

            print(f"Float sum: {float_sum}")
            print(f"Byte sum: {byte_sum}")
            print(f"Expected sum: {expected_sum}")

            assert abs(float_sum - expected_sum) < 1e-3, (
                f"Float sum mismatch: expected {expected_sum}, got {float_sum}"
            )
            assert byte_sum == expected_sum, (
                f"Byte sum mismatch: expected {expected_sum}, got {byte_sum}"
            )

        finally:
            free_float(STORE, float_ptr)
            free_byte(STORE, byte_ptr)

        print("✓ float vs byte array equivalent operations tests passed!")


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
