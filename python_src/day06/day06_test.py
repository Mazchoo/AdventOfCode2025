"""Day 05"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

calculate_homework_total = INSTANCE.exports(STORE)["calculate_homework_total"]
calculate_homework_total_vertical = INSTANCE.exports(STORE)["calculate_homework_total_vertical"]


def test_day06_pt1_sample():
    """Test calculate_homework_total(char*, int) -> uint64_t"""

    path = "./python_src/day06/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, calculate_homework_total)
    assert result == 4277556


def test_day06_pt1():
    """Test calculate_homework_total(char*, int) -> uint64_t"""

    path = "./python_src/day06/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, calculate_homework_total)
    assert result == 8108520669952


def test_day06_pt2_sample():
    """Test calculate_homework_total_vertical(char*, int) -> uint64_t"""

    path = "./python_src/day06/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, calculate_homework_total_vertical)
    assert result == 3263827


def test_day06_pt2():
    """Test calculate_homework_total_vertical(char*, int) -> uint64_t"""

    path = "./python_src/day06/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, calculate_homework_total_vertical)
    assert result == 11708563470209


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
