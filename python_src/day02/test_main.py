"""Day 02"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")
count_repeated_digits = INSTANCE.exports(STORE)["count_repeated_digits"]


def test_day02_pt1_sample():
    """Test the count_repeated_digits function: count_repeated_digits(char*, int) -> uint32_t"""

    path = "./python_src/day02/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_repeated_digits)
    assert result == 1227775554


def test_day02_pt1():
    """Test the count_repeated_digits function: count_repeated_digits(char*, int) -> uint32_t"""

    path = "./python_src/day02/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_repeated_digits)
    assert result == 19605500130


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
