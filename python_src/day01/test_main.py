from pathlib import Path
from typing import Optional, Callable

import pytest
from wasmtime import Store

from python_src.common.wasm_handle import load_wasm_module

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")
count_total_zeros = INSTANCE.exports(STORE)["count_total_zeros"]

create_byte_array = INSTANCE.exports(STORE)["create_byte_array"]
free_byte_array = INSTANCE.exports(STORE)["free_byte_array"]
memory = INSTANCE.exports(STORE)["memory"]


def get_payload_result(path: str, store: Store, wasm_func: Callable) -> Optional[int]:
    """Load string from file and pass string pointer to wasm"""
    sample_input = Path(path).open(encoding='utf-8').read()
    input_bytes = sample_input.encode('utf-8')
    input_len = len(input_bytes)

    # Allocate memory in WASM
    ptr = create_byte_array(store, input_len)
    result = None

    try:
        # Write the string bytes to WASM memory
        mem_data = memory.data_ptr(store)
        for i, byte in enumerate(input_bytes):
            mem_data[ptr + i] = byte

        # Call the function with pointer and length
        result = wasm_func(store, ptr, input_len)
    finally:
        # Free the allocated memory
        free_byte_array(store, ptr)

    return result


def test_day01_pt1_sample():
    """Test the count_total_zeros function: count_total_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/sample.txt"
    result = get_payload_result(path, STORE, count_total_zeros)
    assert result == 3


def test_day01_pt1():
    """Test the count_total_zeros function: count_total_zeros(char*, int) -> uint32_t"""

    path = "./python_src/day01/input/full.txt"
    result = get_payload_result(path, STORE, count_total_zeros)
    assert result == 1100


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
