"""Day 05"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

count_ids_in_range = INSTANCE.exports(STORE)["count_ids_in_range"]
count_all_valid_ids = INSTANCE.exports(STORE)["count_all_valid_ids"]


def test_day02_pt1_sample():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day05/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_ids_in_range)
    assert result == 3


def test_day02_pt1():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day05/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_ids_in_range)
    assert result == 517


def test_day02_pt2_sample():
    """Test get_payload_result(char*, int) -> uint64_t"""

    path = "./python_src/day05/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_all_valid_ids)
    assert result == 14


def test_day02_pt2():
    """Test get_payload_result(char*, int) -> uint64_t"""

    path = "./python_src/day05/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_all_valid_ids)
    assert result == 336173027056994


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
