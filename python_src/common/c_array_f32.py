"""Float 32 c style array with wasm bindings"""

from wasmtime import Store, Instance
from python_src.common.class_wrapper import WasmPtr


class CArrayF32(WasmPtr):
    """Wrapper around f32 C-style array on the wasm side"""

    def __init__(self, store: Store, instance: Instance, size: int):
        """
        Create a new float C array with the specified size

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            size: Size of the array to create
        """
        call_names = ["sum_float_array", "set_array_element", "get_array_element"]

        super().__init__(
            store,
            instance,
            "create_float_array",
            call_names,
            "free_float_array",
            size,
        )
        self.size = size

    def sum(self) -> float:
        """Calculate the sum of all elements in the array"""
        return self("sum_float_array", self.size)

    def set(self, index: int, value: float) -> int:
        """
        Set element at index to value

        Args:
            index: Index to set
            value: Value to set

        Returns:
            1 if successful, 0 if index out of bounds
        """
        if index >= self.size or index < 0:
            raise IndexError(f"Index {index} out of bounds on array size {self.size}")
        return self("set_array_element", index, value)

    def get(self, index: int) -> float:
        """
        Get element at index

        Args:
            index: Index to get

        Returns:
            Value at index
        """
        if index >= self.size or index < 0:
            raise IndexError(f"Index {index} out of bounds on array size {self.size}")
        return self("get_array_element", index)

    def __len__(self) -> int:
        """Return the size of the array"""
        return self.size

    def __setitem__(self, index: int, value: float):
        """Support arr[index] = value syntax"""
        self.set(index, value)

    def __getitem__(self, index: int) -> float:
        """Support arr[index] syntax"""
        return self.get(index)
