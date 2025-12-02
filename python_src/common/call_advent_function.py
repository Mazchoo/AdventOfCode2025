"""Common wrapper around string sent to wasm"""

from typing import Callable, Optional
from pathlib import Path

from wasmtime import Store, Instance


# ToDo - see if there is a more efficient way of doing this
def get_payload_result(
    path: str, store: Store, instance: Instance, wasm_func: Callable
) -> Optional[int]:
    """Load string from file and pass string pointer to wasm"""
    create_byte_array = instance.exports(store)["create_byte_array"]
    free_byte_array = instance.exports(store)["free_byte_array"]
    memory = instance.exports(store)["memory"]

    sample_input = Path(path).open(encoding="utf-8").read()
    input_bytes = sample_input.encode("utf-8")
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
