"""Common wrapper around string sent to wasm"""

import ctypes
from typing import Callable, Optional
from pathlib import Path

from wasmtime import Store, Instance


def call_wasm_with_string(
    store: Store, instance: Instance, payload: str, wasm_func: Callable
) -> Optional[int]:
    """
    Allocate WASM memory, copy string bytes, call function with pointer/length

    Args:
        store: Wasmtime store instance
        instance: Wasmtime instance with exported functions
        payload: String payload to pass to WASM
        wasm_func: WASM function to call with (store, ptr, len) signature

    Returns:
        Result from the WASM function call
    """
    create_byte_array = instance.exports(store)["create_byte_array"]
    free_byte_array = instance.exports(store)["free_byte_array"]
    memory = instance.exports(store)["memory"]

    input_bytes = payload.encode("utf-8")
    input_len = len(input_bytes)

    # Allocate memory in WASM
    ptr = create_byte_array(store, input_len)
    result = None

    try:
        # Write the string bytes to WASM memory using ctypes.memmove for efficiency
        mem_data = memory.data_ptr(store)

        ctypes.memmove(
            ctypes.c_void_p(ctypes.addressof(mem_data.contents) + ptr),
            input_bytes,
            input_len,
        )

        # Call the function with pointer and length
        result = wasm_func(store, ptr, input_len)
    finally:
        # Free the allocated memory
        free_byte_array(store, ptr)

    return result


def get_payload_result(
    path: str, store: Store, instance: Instance, wasm_func: Callable
) -> Optional[int]:
    """Load string from file and pass string pointer to wasm"""
    sample_input = Path(path).open(encoding="utf-8").read()
    return call_wasm_with_string(store, instance, sample_input, wasm_func)
