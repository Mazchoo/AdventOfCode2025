"""Common wasm interaction functions"""

import os
from typing import Tuple

from wasmtime import Store, Module, Instance, Func, FuncType, ValType


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

    # Check what imports the module expects
    imports = module.imports
    print(f"Module imports: {[(imp.module, imp.name, imp.type) for imp in imports]}")

    def notify_memory_growth(memory_index: int):
        pass

    def fd_close(fd: int) -> int:
        return 0

    def fd_write(fd: int, iovs: int, iovs_len: int, nwritten: int) -> int:
        return 0

    def fd_seek(fd: int, offset: int, whence: int, newoffset: int) -> int:
        return 0

    notify_func = Func(store, FuncType([ValType.i32()], []), notify_memory_growth)
    fd_close_func = Func(store, FuncType([ValType.i32()], [ValType.i32()]), fd_close)
    fd_write_func = Func(
        store,
        FuncType(
            [ValType.i32(), ValType.i32(), ValType.i32(), ValType.i32()],
            [ValType.i32()],
        ),
        fd_write,
    )
    fd_seek_func = Func(
        store,
        FuncType(
            [ValType.i32(), ValType.i64(), ValType.i32(), ValType.i32()],
            [ValType.i32()],
        ),
        fd_seek,
    )

    instance = Instance(
        store, module, [notify_func, fd_close_func, fd_write_func, fd_seek_func]
    )

    print(
        f"Module loaded successfully. Exports: {list(instance.exports(store).keys())}"
    )
    return store, instance
