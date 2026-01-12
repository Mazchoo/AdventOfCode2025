"""Day 09"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

get_largest_rect = INSTANCE.exports(STORE)["get_largest_rect"]


def test_day09_pt1_sample():
    """Test get_payload_result(char*, int) -> uint64_t"""

    path = "./python_src/day09/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_largest_rect)
    assert result == 50


def test_day09_pt1():
    """Test get_payload_result(char*, int) -> uint64_t"""

    path = "./python_src/day09/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, get_largest_rect)
    assert result == 4745816424


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
