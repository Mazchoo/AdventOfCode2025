"""Day 01"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")
count_total_zeros = INSTANCE.exports(STORE)["count_total_zeros"]
count_passing_zeros = INSTANCE.exports(STORE)["count_passing_zeros"]


def test_day01_pt1_sample():
    """Test count_total_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_total_zeros)
    assert result == 3


def test_day01_pt1():
    """Test count_total_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_total_zeros)
    assert result == 1100


def test_day01_pt2_sample():
    """Test count_passing_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_passing_zeros)
    assert result == 6


def test_day01_pt2():
    """Test count_passing_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_passing_zeros)
    assert result == 6358


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
