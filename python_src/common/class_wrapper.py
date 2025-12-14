"""Warpper around a single pointer to a wasm memory object"""

from typing import List

from wasmtime import Store, Instance


class WasmPtr:
    """
    Access pattern to wasm data with a single object moved to the Python Side
    This is not suitable for objects that need living references on wasm side as well
    """

    def __init__(
        self,
        store: Store,
        instance: Instance,
        create_name: str,
        call_names: List[str],
        free_name: str,
        *args,
    ):
        self.store = store
        self.instance = instance
        self.call_funcs = {name: instance.exports(store)[name] for name in call_names}
        self.handle = instance.exports(store)[create_name](store, *args)
        self.free_func = instance.exports(store)[free_name]

    def __call__(self, function_name: str, *args):
        """
        Call one of the call functions in call list
        Call function follows convention (self, *args) on Wasm Side
        """
        if function_name not in self.call_funcs:
            raise KeyError(
                f"Function name {function_name} not in {self.call_funcs.keys()}"
            )
        return self.call_funcs[function_name](self.store, self.handle, *args)

    def __del__(self):
        """Free memory on wasm side manually"""
        self.free_func(self.store, self.handle)
