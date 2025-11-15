"""Uint8 c style array with wasm bindings"""

from wasmtime import Store, Instance
from python_src.common.class_wrapper import WasmPtr


class ByteArray(WasmPtr):
    """Wrapper around uint8_t C-style array on the wasm side"""

    def __init__(self, store: Store, instance: Instance, size: int):
        """
        Create a new byte C array with the specified size

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            size: Size of the array to create
        """
        call_names = [
            "sum_byte_array",
            "sum_byte_array_simd",
            "set_byte_array_element",
            "get_byte_array_element",
        ]

        super().__init__(
            store,
            instance,
            "create_byte_array",
            call_names,
            "free_byte_array",
            size,
        )
        self.size = size

    def sum(self) -> int:
        """Calculate the sum of all elements in the array"""
        return self("sum_byte_array", self.size)

    def sum_simd(self) -> int:
        """Calculate the sum of all elements in the array"""
        return self("sum_byte_array_simd", self.size)

    def set(self, index: int, value: int) -> int:
        """
        Set element at index to value

        Args:
            index: Index to set
            value: Value to set (must be 0-255 for uint8_t)

        Returns:
            1 if successful, 0 if index out of bounds
        """
        if index >= self.size or index < 0:
            raise IndexError(f"Index {index} out of bounds on array size {self.size}")
        if not (0 <= value <= 255):
            raise ValueError(f"Value {value} out of range for uint8_t (0-255)")
        return self("set_byte_array_element", index, value)

    def get(self, index: int) -> int:
        """
        Get element at index

        Args:
            index: Index to get

        Returns:
            Value at index (uint8_t: 0-255)
        """
        if index >= self.size or index < 0:
            raise IndexError(f"Index {index} out of bounds on array size {self.size}")
        return self("get_byte_array_element", index)

    def __len__(self) -> int:
        """Return the size of the array"""
        return self.size

    def __setitem__(self, index: int, value: int):
        """Support arr[index] = value syntax"""
        self.set(index, value)

    def __getitem__(self, index: int) -> int:
        """Support arr[index] syntax"""
        return self.get(index)
