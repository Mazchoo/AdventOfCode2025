"""Common wasm interaction functions"""

import os
from typing import Tuple

from wasmtime import Store, Module, Instance


def load_wasm_module(wasm_path: str) -> Tuple[Store, Instance]:
    """Load the WASM module from the given path."""
    if not os.path.exists(wasm_path):
        raise FileNotFoundError(f"WASM file not found: {wasm_path}")

    print(f"Loading WASM module from: {wasm_path}")

    # Create a store and load the module
    store = Store()
    with open(wasm_path, "rb") as f:
        module_bytes = f.read()

    module = Module(store.engine, module_bytes)
    instance = Instance(store, module, [])

    print(
        f"Module loaded successfully. Exports: {list(instance.exports(store).keys())}"
    )
    return store, instance
