#!/usr/bin/env python3
"""
Test script for CArrayF32 wrapper class using the WasmPtr pattern.
This tests the Python wrapper around the WASM float array functions.
"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.c_array_f32 import CArrayF32


STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


class TestCArrayF32Wrapper:
    """Tests for the CArrayF32 wrapper class using WasmPtr pattern"""

    def test_create_and_basic_operations(self):
        """Test creating CArrayF32 and basic operations"""
        print("\n=== Testing CArrayF32 wrapper creation and basic operations ===")

        size = 5
        array = CArrayF32(STORE, INSTANCE, size)

        try:
            # Test basic properties
            assert len(array) == size, f"Expected length {size}, got {len(array)}"
            print(f"Array created with size: {len(array)}")

            # Test setting and getting values using methods
            test_values = [1.5, -2.7, 0.0, 3.14159, -100.25]

            for i, value in enumerate(test_values):
                success = array.set(i, value)
                assert success == 1, f"Failed to set value at index {i}"

                retrieved = array.get(i)
                assert abs(retrieved - value) < 1e-3, (
                    f"Expected {value}, got {retrieved}"
                )
                print(f"array.set({i}, {value}) -> array.get({i}) = {retrieved}")

            print("✓ CArrayF32 basic operations test passed!")

        finally:
            # The destructor should handle cleanup automatically
            del array

    def test_indexing_syntax(self):
        """Test Python indexing syntax with CArrayF32"""
        print("\n=== Testing CArrayF32 indexing syntax ===")

        size = 4
        array = CArrayF32(STORE, INSTANCE, size)

        try:
            # Test setting values using [] syntax
            test_values = [10.5, 20.25, 30.75, 40.125]

            for i, value in enumerate(test_values):
                array[i] = value
                print(f"array[{i}] = {value}")

            # Test getting values using [] syntax
            for i, expected_value in enumerate(test_values):
                retrieved = array[i]
                assert abs(retrieved - expected_value) < 1e-3, (
                    f"Expected {expected_value}, got {retrieved}"
                )
                print(f"array[{i}] = {retrieved}")

            # Test out of bounds access
            try:
                array[size] = 99.0
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

            print("✓ CArrayF32 indexing syntax test passed!")

        finally:
            del array

    def test_sum_functionality(self):
        """Test sum functionality of CArrayF32"""
        print("\n=== Testing CArrayF32 sum functionality ===")

        size = 6
        array = CArrayF32(STORE, INSTANCE, size)

        try:
            # Set test values
            test_values = [1.0, 2.5, -3.0, 4.25, 0.5, -1.75]
            expected_sum = sum(test_values)

            for i, value in enumerate(test_values):
                array[i] = value

            # Test sum method
            result_sum = array.sum()
            print(f"array.sum() = {result_sum}")
            print(f"Expected sum = {expected_sum}")

            assert abs(result_sum - expected_sum) < 1e-3, (
                f"Expected {expected_sum}, got {result_sum}"
            )

            # Test with all zeros
            for i in range(size):
                array[i] = 0.0

            zero_sum = array.sum()
            assert abs(zero_sum) < 1e-6, f"Expected 0.0, got {zero_sum}"
            print(f"Sum of zeros: {zero_sum}")

            print("✓ CArrayF32 sum functionality test passed!")

        finally:
            del array

    def test_wrapper_vs_raw_comparison(self):
        """Test that CArrayF32 wrapper produces same results as raw functions"""
        print("\n=== Testing CArrayF32 wrapper vs raw functions ===")

        # Get raw functions
        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]
        set_func = INSTANCE.exports(STORE)["set_array_element"]
        get_func = INSTANCE.exports(STORE)["get_array_element"]
        sum_func = INSTANCE.exports(STORE)["sum_float_array"]

        size = 4
        test_values = [2.5, -7.25, 0.125, 15.75]

        # Test with wrapper
        array = CArrayF32(STORE, INSTANCE, size)

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

                assert abs(wrapper_value - expected_value) < 1e-3, (
                    f"Wrapper value mismatch at index {i}"
                )
                assert abs(raw_value - expected_value) < 1e-3, (
                    f"Raw value mismatch at index {i}"
                )
                assert abs(wrapper_value - raw_value) < 1e-3, (
                    f"Wrapper and raw values differ at index {i}"
                )

            # Compare sums
            wrapper_sum = array.sum()
            raw_sum = sum_func(STORE, raw_ptr, size)
            expected_sum = sum(test_values)

            print(f"Wrapper sum: {wrapper_sum}")
            print(f"Raw sum: {raw_sum}")
            print(f"Expected sum: {expected_sum}")

            assert abs(wrapper_sum - expected_sum) < 1e-3, (
                f"Wrapper sum mismatch: expected {expected_sum}, got {wrapper_sum}"
            )
            assert abs(raw_sum - expected_sum) < 1e-3, (
                f"Raw sum mismatch: expected {expected_sum}, got {raw_sum}"
            )
            assert abs(wrapper_sum - raw_sum) < 1e-3, (
                f"Wrapper and raw sums differ: wrapper={wrapper_sum}, raw={raw_sum}"
            )

            print("✓ CArrayF32 wrapper vs raw functions test passed!")

        finally:
            del array
            free_func(STORE, raw_ptr)

    def test_empty_array(self):
        """Test CArrayF32 with size 0"""
        print("\n=== Testing CArrayF32 with size 0 ===")

        array = CArrayF32(STORE, INSTANCE, 0)

        try:
            assert len(array) == 0, f"Expected length 0, got {len(array)}"

            # Sum should be 0 for empty array
            result = array.sum()
            assert abs(result) < 1e-6, f"Expected 0.0 for empty array, got {result}"
            print(f"Empty array sum: {result}")

            # Test out of bounds access on empty array
            try:
                _ = array[0]
                assert False, "Should have raised IndexError for empty array access"
            except IndexError as e:
                print(f"✓ Empty array access correctly raised: {e}")

            print("✓ CArrayF32 empty array test passed!")

        finally:
            del array

    def test_carray_vs_vector_comparison(self):
        """Test that CArrayF32 wrapper produces same results as VectorF32 wrapper"""
        print("\n=== Testing CArrayF32 vs VectorF32 wrapper comparison ===")

        from python_src.common.vector_f32 import VectorF32

        size = 5
        test_values = [1.0, -2.5, 3.14, 0.0, -100.75]

        # Create both wrappers
        c_array = CArrayF32(STORE, INSTANCE, size)
        vector = VectorF32(STORE, INSTANCE, size)

        try:
            # Set the same values in both
            for i, value in enumerate(test_values):
                c_array[i] = value
                vector[i] = value

            # Compare individual elements
            for i, expected_value in enumerate(test_values):
                array_value = c_array[i]
                vector_value = vector[i]

                print(
                    f"Index {i}: array={array_value}, vector={vector_value}, expected={expected_value}"
                )

                assert abs(array_value - expected_value) < 1e-3, (
                    f"Array value mismatch at index {i}"
                )
                assert abs(vector_value - expected_value) < 1e-3, (
                    f"Vector value mismatch at index {i}"
                )
                assert abs(array_value - vector_value) < 1e-3, (
                    f"Array and vector values differ at index {i}"
                )

            # Compare sums
            array_sum = c_array.sum()
            vector_sum = vector.sum()
            expected_sum = sum(test_values)

            print(f"Array sum: {array_sum}")
            print(f"Vector sum: {vector_sum}")
            print(f"Expected sum: {expected_sum}")

            assert abs(array_sum - expected_sum) < 1e-3, (
                f"Array sum mismatch: expected {expected_sum}, got {array_sum}"
            )
            assert abs(vector_sum - expected_sum) < 1e-3, (
                f"Vector sum mismatch: expected {expected_sum}, got {vector_sum}"
            )
            assert abs(array_sum - vector_sum) < 1e-3, (
                f"Array and vector sums differ: array={array_sum}, vector={vector_sum}"
            )

            print("✓ CArrayF32 vs VectorF32 comparison test passed!")

        finally:
            del c_array
            del vector

    def test_large_array(self):
        """Test CArrayF32 with larger size to verify performance and correctness"""
        print("\n=== Testing CArrayF32 with large size ===")

        size = 1000
        array = CArrayF32(STORE, INSTANCE, size)

        try:
            assert len(array) == size, f"Expected length {size}, got {len(array)}"

            # Fill array with incremental values
            for i in range(size):
                array[i] = float(i + 1)

            # Verify some random indices
            test_indices = [0, 100, 500, 999]
            for i in test_indices:
                retrieved = array[i]
                expected = float(i + 1)
                assert abs(retrieved - expected) < 1e-3, (
                    f"Value mismatch at index {i}: expected {expected}, got {retrieved}"
                )

            # Test sum (should be sum of 1 to 1000 = 500500)
            result_sum = array.sum()
            expected_sum = sum(range(1, size + 1))
            print(f"Large array sum: {result_sum}")
            print(f"Expected sum: {expected_sum}")

            assert abs(result_sum - expected_sum) < 1e-3, (
                f"Sum mismatch: expected {expected_sum}, got {result_sum}"
            )

            print("✓ CArrayF32 large array test passed!")

        finally:
            del array

    def test_bounds_checking_behavior(self):
        """Test bounds checking behavior matches array implementation"""
        print("\n=== Testing CArrayF32 bounds checking behavior ===")

        # Get raw functions for comparison
        create_func = INSTANCE.exports(STORE)["create_float_array"]
        free_func = INSTANCE.exports(STORE)["free_float_array"]
        set_func = INSTANCE.exports(STORE)["set_array_element"]

        size = 5
        array = CArrayF32(STORE, INSTANCE, size)
        raw_ptr = create_func(STORE, size)

        try:
            # Test out of bounds behavior - based on the test file,
            # raw array functions may not check bounds properly
            # But our wrapper should still provide bound checking

            # Test setting out of bounds with wrapper (should raise IndexError)
            try:
                array[size] = 99.0
                assert False, "Should have raised IndexError for out of bounds set"
            except IndexError as e:
                print(f"✓ Wrapper out of bounds set correctly raised: {e}")

            # Test the raw function behavior for comparison
            raw_success = set_func(STORE, raw_ptr, size, 99.0)
            print(f"Raw function out of bounds set returned: {raw_success}")
            # Based on the test file, this might return 1 (success) even for out of bounds

            # Test getting out of bounds with wrapper (should raise IndexError)
            try:
                _ = array[size]
                assert False, "Should have raised IndexError for out of bounds get"
            except IndexError as e:
                print(f"✓ Wrapper out of bounds get correctly raised: {e}")

            print("✓ CArrayF32 bounds checking test passed!")

        finally:
            del array
            free_func(STORE, raw_ptr)


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
