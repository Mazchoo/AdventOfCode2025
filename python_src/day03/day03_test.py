"""Day 02"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")
get_highest_digit_pair = INSTANCE.exports(STORE)["get_highest_digit_pair"]
get_highest_12_digits = INSTANCE.exports(STORE)["get_highest_12_digits"]


def test_day02_pt1_sample():
    """Test get_highest_digit_pair(char*, int) -> uint32_t"""

    path = "./python_src/day03/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_highest_digit_pair)
    assert result == 357


def test_day02_pt1():
    """Test get_highest_digit_pair(char*, int) -> uint32_t"""

    path = "./python_src/day03/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_highest_digit_pair)
    assert result == 17343


def test_day02_pt2_sample():
    """Test get_highest_digit_pair(char*, int) -> uint64_t"""

    path = "./python_src/day03/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_highest_12_digits)
    assert result == 3121910778619


def test_day02_pt2():
    """Test get_highest_digit_pair(char*, int) -> uint64_t"""

    path = "./python_src/day03/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_highest_12_digits)
    assert result == 172664333119298


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
