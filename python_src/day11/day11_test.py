"""Day 11"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

count_number_paths = INSTANCE.exports(STORE)["count_number_paths"]


def test_day11_pt1_sample():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day11/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_number_paths)
    assert result == 5


def test_day11_pt1():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day11/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, count_number_paths)
    assert result == 788


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
