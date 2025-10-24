#!/usr/bin/env python3
"""
Test script for loading and calling WASM functions using wasmtime.
This script loads the mod.wasm binary and tests the exported functions:
- add(int, int) -> int
- multiply(int, int) -> int
- factorial(int) -> int
- power(double, int) -> double
"""

import pytest

from src.common.wasm_handle import load_wasm_module


STORE, INSTANCE = load_wasm_module("./wasm/build/mod.wasm")


def test_add_function():
    """Test the add function: add(int, int) -> int"""
    print("\n=== Testing add function ===")
    add_func = INSTANCE.exports(STORE)["add"]

    test_cases = [(5, 3), (10, -2), (0, 0), (-5, -10), (100, 200)]

    for a, b in test_cases:
        result = add_func(STORE, a, b)  # type: ignore
        print(f"add({a}, {b}) = {result}")
        assert result == a + b, f"Expected {a + b}, got {result}"

    print("✓ add function tests passed!")


def test_multiply_function():
    """Test the multiply function: multiply(int, int) -> int"""
    print("\n=== Testing multiply function ===")
    multiply_func = INSTANCE.exports(STORE)["multiply"]

    test_cases = [(5, 3), (10, -2), (0, 5), (-5, -10), (7, 8)]

    for a, b in test_cases:
        result = multiply_func(STORE, a, b)  # type: ignore
        print(f"multiply({a}, {b}) = {result}")
        assert result == a * b, f"Expected {a * b}, got {result}"

    print("✓ multiply function tests passed!")


def test_factorial_function():
    """Test the factorial function: factorial(int) -> int"""
    print("\n=== Testing factorial function ===")
    factorial_func = INSTANCE.exports(STORE)["factorial"]

    def python_factorial(n):
        """Python implementation for comparison"""
        if n <= 1:
            return 1
        result = 1
        for i in range(2, n + 1):
            result *= i
        return result

    test_cases = [0, 1, 2, 3, 4, 5, 6, 7, 8, 10]

    for n in test_cases:
        result = factorial_func(STORE, n)  # type: ignore
        expected = python_factorial(n)
        print(f"factorial({n}) = {result}")
        assert result == expected, f"Expected {expected}, got {result}"

    print("✓ factorial function tests passed!")


def test_power_function():
    """Test the power function: power(double, int) -> double"""
    print("\n=== Testing power function ===")
    power_func = INSTANCE.exports(STORE)["power"]  # type: ignore

    test_cases = [
        (2.0, 3),
        (5.0, 2),
        (10.0, 0),
        (3.14, 1),
        (2.5, 4),
        (-2.0, 3),
        (1.5, 5),
    ]

    for base, exponent in test_cases:
        result = power_func(STORE, base, exponent)  # type: ignore
        expected = base**exponent
        print(f"power({base}, {exponent}) = {result}")
        # Use approximate comparison for floating point
        assert abs(result - expected) < 1e-10, f"Expected {expected}, got {result}"

    print("✓ power function tests passed!")


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
