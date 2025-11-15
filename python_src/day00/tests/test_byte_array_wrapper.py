#!/usr/bin/env python3
"""
Test script for ByteArray wrapper class using the WasmPtr pattern.
This tests the Python wrapper around the WASM byte array functions.
"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.byte_array import ByteArray


STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


class TestByteArrayWrapper:
    """Tests for the ByteArray wrapper class using WasmPtr pattern"""

    def test_create_and_basic_operations(self):
        """Test creating ByteArray and basic operations"""
        print("\n=== Testing ByteArray wrapper creation and basic operations ===")

        size = 5
        array = ByteArray(STORE, INSTANCE, size)

        try:
            # Test basic properties
            assert len(array) == size, f"Expected length {size}, got {len(array)}"
            print(f"Array created with size: {len(array)}")

            # Test setting and getting values using methods
            test_values = [0, 127, 255, 42, 100]

            for i, value in enumerate(test_values):
                success = array.set(i, value)
                assert success == 1, f"Failed to set value at index {i}"

                retrieved = array.get(i)
                assert retrieved == value, f"Expected {value}, got {retrieved}"
                print(f"array.set({i}, {value}) -> array.get({i}) = {retrieved}")

            print("✓ ByteArray basic operations test passed!")

        finally:
            # The destructor should handle cleanup automatically
            del array

    def test_indexing_syntax(self):
        """Test Python indexing syntax with ByteArray"""
        print("\n=== Testing ByteArray indexing syntax ===")

        size = 4
        array = ByteArray(STORE, INSTANCE, size)

        try:
            # Test setting values using [] syntax
            test_values = [10, 20, 30, 40]

            for i, value in enumerate(test_values):
                array[i] = value
                print(f"array[{i}] = {value}")

            # Test getting values using [] syntax
            for i, expected_value in enumerate(test_values):
                retrieved = array[i]
                assert retrieved == expected_value, (
                    f"Expected {expected_value}, got {retrieved}"
                )
                print(f"array[{i}] = {retrieved}")

            # Test out of bounds access
            try:
                array[size] = 99
                assert False, "Should have raised IndexError for out of bounds set"
            except IndexError as e:
                print(f"✓ Out of bounds set correctly raised: {e}")

            try:
                _ = array[size]
                assert False, "Should have raised IndexError for out of bounds get"
            except IndexError as e:
                print(f"✓ Out of bounds get correctly raised: {e}")

            # Test negative index
            try:
                _ = array[-1]
                assert False, "Should have raised IndexError for negative index"
            except IndexError as e:
                print(f"✓ Negative index correctly raised: {e}")

            print("✓ ByteArray indexing syntax test passed!")

        finally:
            del array

    def test_value_range_validation(self):
        """Test that ByteArray validates uint8_t range (0-255)"""
        print("\n=== Testing ByteArray value range validation ===")

        size = 3
        array = ByteArray(STORE, INSTANCE, size)

        try:
            # Test valid values
            valid_values = [0, 127, 255]
            for i, value in enumerate(valid_values):
                array[i] = value
                assert array[i] == value, f"Valid value {value} failed"
                print(f"✓ Valid value {value} accepted")

            # Test invalid values (too low)
            try:
                array[0] = -1
                assert False, "Should have raised ValueError for negative value"
            except ValueError as e:
                print(f"✓ Negative value correctly rejected: {e}")

            # Test invalid values (too high)
            try:
                array[0] = 256
                assert False, "Should have raised ValueError for value > 255"
            except ValueError as e:
                print(f"✓ Value > 255 correctly rejected: {e}")

            print("✓ ByteArray value range validation test passed!")

        finally:
            del array

    def test_sum_functionality(self):
        """Test sum functionality of ByteArray"""
        print("\n=== Testing ByteArray sum functionality ===")

        size = 6
        array = ByteArray(STORE, INSTANCE, size)

        try:
            # Set test values
            test_values = [10, 25, 100, 200, 15, 5]
            expected_sum = sum(test_values)

            for i, value in enumerate(test_values):
                array[i] = value

            # Test sum method
            result_sum = array.sum()
            print(f"array.sum() = {result_sum}")
            print(f"Expected sum = {expected_sum}")

            assert result_sum == expected_sum, (
                f"Expected {expected_sum}, got {result_sum}"
            )

            # Test with all zeros
            for i in range(size):
                array[i] = 0

            zero_sum = array.sum()
            assert zero_sum == 0, f"Expected 0, got {zero_sum}"
            print(f"Sum of zeros: {zero_sum}")

            # Test with maximum values
            for i in range(size):
                array[i] = 255

            max_sum = array.sum()
            expected_max_sum = 255 * size
            assert max_sum == expected_max_sum, (
                f"Expected {expected_max_sum}, got {max_sum}"
            )
            print(f"Sum of max values: {max_sum}")

            print("✓ ByteArray sum functionality test passed!")

        finally:
            del array

    def test_wrapper_vs_raw_comparison(self):
        """Test that ByteArray wrapper produces same results as raw functions"""
        print("\n=== Testing ByteArray wrapper vs raw functions ===")

        # Get raw functions
        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]
        get_func = INSTANCE.exports(STORE)["get_byte_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_byte_array"]

        size = 4
        test_values = [25, 75, 125, 175]

        # Test with wrapper
        array = ByteArray(STORE, INSTANCE, size)

        # Test with raw functions
        raw_ptr = create_func(STORE, size)

        try:
            # Set same values in both
            for i, value in enumerate(test_values):
                array[i] = value
                set_func(STORE, raw_ptr, i, value)

            # Compare individual elements
            for i, expected_value in enumerate(test_values):
                wrapper_value = array[i]
                raw_value = get_func(STORE, raw_ptr, i)

                print(
                    f"Index {i}: wrapper={wrapper_value}, raw={raw_value}, expected={expected_value}"
                )

                assert wrapper_value == expected_value, (
                    f"Wrapper value mismatch at index {i}"
                )
                assert raw_value == expected_value, f"Raw value mismatch at index {i}"
                assert wrapper_value == raw_value, (
                    f"Wrapper and raw values differ at index {i}"
                )

            # Compare sums
            wrapper_sum = array.sum()
            raw_sum = sum_func(STORE, raw_ptr, size)
            expected_sum = sum(test_values)

            print(f"Wrapper sum: {wrapper_sum}")
            print(f"Raw sum: {raw_sum}")
            print(f"Expected sum: {expected_sum}")

            assert wrapper_sum == expected_sum, (
                f"Wrapper sum mismatch: expected {expected_sum}, got {wrapper_sum}"
            )
            assert raw_sum == expected_sum, (
                f"Raw sum mismatch: expected {expected_sum}, got {raw_sum}"
            )
            assert wrapper_sum == raw_sum, (
                f"Wrapper and raw sums differ: wrapper={wrapper_sum}, raw={raw_sum}"
            )

            print("✓ ByteArray wrapper vs raw functions test passed!")

        finally:
            del array
            free_func(STORE, raw_ptr)

    def test_empty_array(self):
        """Test ByteArray with size 0"""
        print("\n=== Testing ByteArray with size 0 ===")

        array = ByteArray(STORE, INSTANCE, 0)

        try:
            assert len(array) == 0, f"Expected length 0, got {len(array)}"

            # Sum should be 0 for empty array
            result = array.sum()
            assert result == 0, f"Expected 0 for empty array, got {result}"
            print(f"Empty array sum: {result}")

            # Test out of bounds access on empty array
            try:
                _ = array[0]
                assert False, "Should have raised IndexError for empty array access"
            except IndexError as e:
                print(f"✓ Empty array access correctly raised: {e}")

            print("✓ ByteArray empty array test passed!")

        finally:
            del array

    def test_bytearray_vs_float_array_comparison(self):
        """Test that ByteArray wrapper can work alongside CArrayF32 wrapper"""
        print("\n=== Testing ByteArray vs CArrayF32 wrapper comparison ===")

        from python_src.common.c_array_f32 import CArrayF32

        size = 5
        # Use values that work for both byte (0-255) and float
        test_values = [10, 25, 100, 200, 15]

        # Create both wrappers
        byte_array = ByteArray(STORE, INSTANCE, size)
        float_array = CArrayF32(STORE, INSTANCE, size)

        try:
            # Set the same values in both
            for i, value in enumerate(test_values):
                byte_array[i] = value
                float_array[i] = float(value)

            # Compare individual elements
            for i, expected_value in enumerate(test_values):
                byte_value = byte_array[i]
                float_value = float_array[i]

                print(
                    f"Index {i}: byte={byte_value}, float={float_value}, expected={expected_value}"
                )

                assert byte_value == expected_value, f"Byte value mismatch at index {i}"
                assert abs(float_value - expected_value) < 1e-3, (
                    f"Float value mismatch at index {i}"
                )

            # Compare sums
            byte_sum = byte_array.sum()
            float_sum = float_array.sum()
            expected_sum = sum(test_values)

            print(f"Byte sum: {byte_sum}")
            print(f"Float sum: {float_sum}")
            print(f"Expected sum: {expected_sum}")

            assert byte_sum == expected_sum, (
                f"Byte sum mismatch: expected {expected_sum}, got {byte_sum}"
            )
            assert abs(float_sum - expected_sum) < 1e-3, (
                f"Float sum mismatch: expected {expected_sum}, got {float_sum}"
            )

            print("✓ ByteArray vs CArrayF32 comparison test passed!")

        finally:
            del byte_array
            del float_array

    def test_large_array(self):
        """Test ByteArray with larger size to verify performance and correctness"""
        print("\n=== Testing ByteArray with large size ===")

        size = 1000
        array = ByteArray(STORE, INSTANCE, size)

        try:
            assert len(array) == size, f"Expected length {size}, got {len(array)}"

            # Fill array with pattern values
            for i in range(size):
                array[i] = i % 256  # Cycle through 0-255

            # Verify some random indices
            test_indices = [0, 100, 500, 999]
            for i in test_indices:
                retrieved = array[i]
                expected = i % 256
                assert retrieved == expected, (
                    f"Value mismatch at index {i}: expected {expected}, got {retrieved}"
                )

            # Test sum (this will test large sums)
            result_sum = array.sum()
            # Calculate expected sum manually
            expected_sum = sum((i % 256) for i in range(size))
            print(f"Large array sum: {result_sum}")
            print(f"Expected sum: {expected_sum}")

            assert result_sum == expected_sum, (
                f"Sum mismatch: expected {expected_sum}, got {result_sum}"
            )

            print("✓ ByteArray large array test passed!")

        finally:
            del array

    def test_bounds_checking_behavior(self):
        """Test bounds checking behavior matches array implementation"""
        print("\n=== Testing ByteArray bounds checking behavior ===")

        # Get raw functions for comparison
        create_func = INSTANCE.exports(STORE)["create_byte_array"]
        free_func = INSTANCE.exports(STORE)["free_byte_array"]
        set_func = INSTANCE.exports(STORE)["set_byte_array_element"]

        size = 5
        array = ByteArray(STORE, INSTANCE, size)
        raw_ptr = create_func(STORE, size)

        try:
            # Test out of bounds behavior - based on the test file,
            # raw array functions may not check bounds properly
            # But our wrapper should still provide bound checking

            # Test setting out of bounds with wrapper (should raise IndexError)
            try:
                array[size] = 99
                assert False, "Should have raised IndexError for out of bounds set"
            except IndexError as e:
                print(f"✓ Wrapper out of bounds set correctly raised: {e}")

            # Test the raw function behavior for comparison
            raw_success = set_func(STORE, raw_ptr, size, 99)
            print(f"Raw function out of bounds set returned: {raw_success}")
            # Based on the test file, this might return 1 (success) even for out of bounds

            # Test getting out of bounds with wrapper (should raise IndexError)
            try:
                _ = array[size]
                assert False, "Should have raised IndexError for out of bounds get"
            except IndexError as e:
                print(f"✓ Wrapper out of bounds get correctly raised: {e}")

            print("✓ ByteArray bounds checking test passed!")

        finally:
            del array
            free_func(STORE, raw_ptr)

    def test_edge_values(self):
        """Test ByteArray with edge values (0, 255)"""
        print("\n=== Testing ByteArray with edge values ===")

        size = 3
        array = ByteArray(STORE, INSTANCE, size)

        try:
            # Test minimum, maximum, and middle values
            test_cases = [
                (0, 0),  # Minimum value
                (1, 255),  # Maximum value
                (2, 128),  # Middle value
            ]

            for index, value in test_cases:
                array[index] = value
                retrieved = array[index]
                assert retrieved == value, (
                    f"Edge value test failed: expected {value}, got {retrieved}"
                )
                print(f"✓ Edge value test passed: array[{index}] = {value}")

            # Test sum of edge values
            expected_sum = 0 + 255 + 128
            result_sum = array.sum()
            assert result_sum == expected_sum, (
                f"Edge values sum mismatch: expected {expected_sum}, got {result_sum}"
            )
            print(f"✓ Edge values sum test passed: {result_sum}")

            print("✓ ByteArray edge values test passed!")

        finally:
            del array


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
