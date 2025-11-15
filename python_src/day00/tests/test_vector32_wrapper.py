#!/usr/bin/env python3
"""
Test script for VectorF32 wrapper class using the WasmPtr pattern.
This tests the Python wrapper around the WASM float vector functions.
"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.vector_f32 import VectorF32


STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


class TestVectorF32Wrapper:
    """Tests for the VectorF32 wrapper class using WasmPtr pattern"""

    def test_create_and_basic_operations(self):
        """Test creating VectorF32 and basic operations"""
        print("\n=== Testing VectorF32 wrapper creation and basic operations ===")

        size = 5
        vector = VectorF32(STORE, INSTANCE, size)

        try:
            # Test basic properties
            assert len(vector) == size, f"Expected length {size}, got {len(vector)}"
            print(f"Vector created with size: {len(vector)}")

            # Test setting and getting values using methods
            test_values = [1.5, -2.7, 0.0, 3.14159, -100.25]

            for i, value in enumerate(test_values):
                success = vector.set(i, value)
                assert success == 1, f"Failed to set value at index {i}"

                retrieved = vector.get(i)
                assert abs(retrieved - value) < 1e-3, (
                    f"Expected {value}, got {retrieved}"
                )
                print(f"vector.set({i}, {value}) -> vector.get({i}) = {retrieved}")

            print("✓ VectorF32 basic operations test passed!")

        finally:
            # The destructor should handle cleanup automatically
            del vector

    def test_indexing_syntax(self):
        """Test Python indexing syntax with VectorF32"""
        print("\n=== Testing VectorF32 indexing syntax ===")

        size = 4
        vector = VectorF32(STORE, INSTANCE, size)

        try:
            # Test setting values using [] syntax
            test_values = [10.5, 20.25, 30.75, 40.125]

            for i, value in enumerate(test_values):
                vector[i] = value
                print(f"vector[{i}] = {value}")

            # Test getting values using [] syntax
            for i, expected_value in enumerate(test_values):
                retrieved = vector[i]
                assert abs(retrieved - expected_value) < 1e-3, (
                    f"Expected {expected_value}, got {retrieved}"
                )
                print(f"vector[{i}] = {retrieved}")

            # Test out of bounds access
            try:
                vector[size] = 99.0
                assert False, "Should have raised IndexError for out of bounds set"
            except IndexError as e:
                print(f"✓ Out of bounds set correctly raised: {e}")

            try:
                _ = vector[size]
                assert False, "Should have raised IndexError for out of bounds get"
            except IndexError as e:
                print(f"✓ Out of bounds get correctly raised: {e}")

            # Test negative index
            try:
                _ = vector[-1]
                assert False, "Should have raised IndexError for negative index"
            except IndexError as e:
                print(f"✓ Negative index correctly raised: {e}")

            print("✓ VectorF32 indexing syntax test passed!")

        finally:
            del vector

    def test_sum_functionality(self):
        """Test sum functionality of VectorF32"""
        print("\n=== Testing VectorF32 sum functionality ===")

        size = 6
        vector = VectorF32(STORE, INSTANCE, size)

        try:
            # Set test values
            test_values = [1.0, 2.5, -3.0, 4.25, 0.5, -1.75]
            expected_sum = sum(test_values)

            for i, value in enumerate(test_values):
                vector[i] = value

            # Test sum method
            result_sum = vector.sum()
            print(f"vector.sum() = {result_sum}")
            print(f"Expected sum = {expected_sum}")

            assert abs(result_sum - expected_sum) < 1e-3, (
                f"Expected {expected_sum}, got {result_sum}"
            )

            # Test with all zeros
            for i in range(size):
                vector[i] = 0.0

            zero_sum = vector.sum()
            assert abs(zero_sum) < 1e-6, f"Expected 0.0, got {zero_sum}"
            print(f"Sum of zeros: {zero_sum}")

            print("✓ VectorF32 sum functionality test passed!")

        finally:
            del vector

    def test_wrapper_vs_raw_comparison(self):
        """Test that VectorF32 wrapper produces same results as raw functions"""
        print("\n=== Testing VectorF32 wrapper vs raw functions ===")

        # Get raw functions
        create_func = INSTANCE.exports(STORE)["create_float_vector"]
        free_func = INSTANCE.exports(STORE)["free_float_vector"]
        set_func = INSTANCE.exports(STORE)["set_vector_element"]
        get_func = INSTANCE.exports(STORE)["get_vector_element"]
        sum_func = INSTANCE.exports(STORE)["sum_float_vector"]

        size = 4
        test_values = [2.5, -7.25, 0.125, 15.75]

        # Test with wrapper
        vector = VectorF32(STORE, INSTANCE, size)

        # Test with raw functions
        raw_ptr = create_func(STORE, size)

        try:
            # Set same values in both
            for i, value in enumerate(test_values):
                vector[i] = value
                set_func(STORE, raw_ptr, i, value)

            # Compare individual elements
            for i, expected_value in enumerate(test_values):
                wrapper_value = vector[i]
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
            wrapper_sum = vector.sum()
            raw_sum = sum_func(STORE, raw_ptr)
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

            print("✓ VectorF32 wrapper vs raw functions test passed!")

        finally:
            del vector
            free_func(STORE, raw_ptr)

    def test_empty_vector(self):
        """Test VectorF32 with size 0"""
        print("\n=== Testing VectorF32 with size 0 ===")

        vector = VectorF32(STORE, INSTANCE, 0)

        try:
            assert len(vector) == 0, f"Expected length 0, got {len(vector)}"

            # Sum should be 0 for empty vector
            result = vector.sum()
            assert abs(result) < 1e-6, f"Expected 0.0 for empty vector, got {result}"
            print(f"Empty vector sum: {result}")

            # Test out of bounds access on empty vector
            try:
                _ = vector[0]
                assert False, "Should have raised IndexError for empty vector access"
            except IndexError as e:
                print(f"✓ Empty vector access correctly raised: {e}")

            print("✓ VectorF32 empty vector test passed!")

        finally:
            del vector


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
