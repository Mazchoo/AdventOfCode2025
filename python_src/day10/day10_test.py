"""Day 10"""

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.call_advent_function import get_payload_result

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

fewest_button_presses = INSTANCE.exports(STORE)["fewest_button_presses"]
fewest_presses_to_configuration = INSTANCE.exports(STORE)[
    "fewest_presses_to_configuration"
]


def test_day10_pt1_sample():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day10/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, fewest_button_presses)
    assert result == 7


def test_day10_pt1():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day10/input/full.txt"
    result = get_payload_result(path, STORE, INSTANCE, fewest_button_presses)
    assert result == 578


def test_day10_pt2_sample():
    """Test get_payload_result(char*, int) -> uint32_t"""

    path = "./python_src/day10/input/sample.txt"
    result = get_payload_result(path, STORE, INSTANCE, fewest_presses_to_configuration)
    assert result == 33


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
