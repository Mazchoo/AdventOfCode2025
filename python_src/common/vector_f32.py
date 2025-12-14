"""Float 32 vector with wasm bindings"""

from wasmtime import Store, Instance
from python_src.common.class_wrapper import WasmPtr


class VectorF32(WasmPtr):
    """Wrapper around f32 vector on the wasm side"""

    call_names = ("sum_float_vector", "set_vector_element", "get_vector_element")

    def __init__(self, store: Store, instance: Instance, size: int):
        """
        Create a new float vector with the specified size

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            size: Size of the vector to create
        """

        super().__init__(
            store,
            instance,
            "create_float_vector",
            self.call_names,
            "free_float_vector",
            size,
        )
        self.size = size

    def sum(self) -> float:
        """Calculate the sum of all elements in the vector"""
        return self("sum_float_vector")

    def set(self, index: int, value: float) -> int:
        """
        Set element at index to value

        Args:
            index: Index to set
            value: Value to set

        Returns:
            1 if successful, 0 if index out of bounds
        """
        return self("set_vector_element", index, value)

    def get(self, index: int) -> float:
        """
        Get element at index

        Args:
            index: Index to get

        Returns:
            Value at index
        """
        return self("get_vector_element", index)

    def __len__(self) -> int:
        """Return the size of the vector"""
        return self.size

    def __setitem__(self, index: int, value: float):
        """Support vec[index] = value syntax"""
        success = self.set(index, value)
        if success == 0:
            raise IndexError(f"Vector index {index} out of range for size {self.size}")

    def __getitem__(self, index: int) -> float:
        """Support vec[index] syntax"""
        if index < 0 or index >= self.size:
            raise IndexError(f"Vector index {index} out of range for size {self.size}")
        return self.get(index)
