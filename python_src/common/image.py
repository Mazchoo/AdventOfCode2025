"""Single channel image uint8 vector with wasm bindings"""

import ctypes

import numpy as np
from wasmtime import Store, Instance
from python_src.common.class_wrapper import WasmPtr


class Image(WasmPtr):
    """Wrapper around binary image (uint8) on the wasm side"""

    call_names = (
        "get_image_pitch",
        "get_image_height",
        "get_image_size",
        "get_image_element",
        "set_image_element",
        "get_image_pixel",
        "set_image_pixel",
        "get_image_data_ptr",
    )

    def __init__(self, store: Store, instance: Instance, width: int, height: int):
        """
        Create a new blank binary image with the specified dimensions

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            width: Width of the image (pitch)
            height: Height of the image
        """
        super().__init__(
            store,
            instance,
            "create_blank_image",
            self.call_names,
            "free_image",
            width,
            height,
        )
        self._width = width
        self._height = height

    @property
    def width(self) -> int:
        """Get the width (pitch) of the image"""
        return self._width

    @property
    def height(self) -> int:
        """Get the height of the image"""
        return self._height

    @property
    def size(self) -> int:
        """Get the total size of the image"""
        return self("get_image_size")

    def get_element(self, index: int) -> int:
        """
        Get element at linear index

        Args:
            index: Linear index into the image data

        Returns:
            Value at index (uint8)
        """
        return self("get_image_element", index)

    def set_element(self, index: int, value: int) -> int:
        """
        Set element at linear index

        Args:
            index: Linear index into the image data
            value: Value to set (uint8)

        Returns:
            1 if successful, 0 if index out of bounds
        """
        return self("set_image_element", index, value)

    def get_pixel(self, row: int, col: int) -> int:
        """
        Get pixel value at specified row and column

        Args:
            row: Row index
            col: Column index

        Returns:
            Pixel value (uint8)
        """
        return self("get_image_pixel", row, col)

    def set_pixel(self, row: int, col: int, value: int) -> int:
        """
        Set pixel value at specified row and column

        Args:
            row: Row index
            col: Column index
            value: Value to set (uint8)

        Returns:
            1 if successful, 0 if out of bounds
        """
        return self("set_image_pixel", row, col, value)

    def to_numpy(self) -> np.ndarray:
        """
        Get image data as a numpy array

        Returns:
            2D numpy array of shape (height, width) with dtype uint8
        """
        # Get the raw data pointer
        data_ptr = self("get_image_data_ptr")

        # Get the memory from the instance
        memory = self.instance.exports(self.store)["memory"]

        # Create numpy array view of the WASM memory
        size = self.size
        memory_data = memory.data_ptr(self.store)

        # Create a numpy array from the memory buffer using ctypes
        buffer_start = ctypes.addressof(memory_data.contents) + data_ptr
        buffer = (ctypes.c_uint8 * size).from_address(buffer_start)

        arr = np.frombuffer(buffer, dtype=np.uint8).reshape(self._height, self._width)

        return arr.copy()  # Return a copy to avoid memory issues

    def __len__(self) -> int:
        """Return the total size of the image"""
        return self.size

    def __setitem__(self, key, value: int):
        """
        Support img[index] = value or img[row, col] = value syntax

        Args:
            key: Either a single index or a tuple of (row, col)
            value: Value to set (uint8)
        """
        if isinstance(key, tuple):
            if len(key) != 2:
                raise IndexError(
                    "Image indexing requires either a single index or (row, col)"
                )
            row, col = key
            success = self.set_pixel(row, col, value)
            if success == 0:
                raise IndexError(
                    f"Image pixel ({row}, {col}) out of range for size {self._width}x{self._height}"
                )
        else:
            success = self.set_element(key, value)
            if success == 0:
                raise IndexError(f"Image index {key} out of range for size {self.size}")

    def __getitem__(self, key) -> int:
        """
        Support img[index] or img[row, col] syntax

        Args:
            key: Either a single index or a tuple of (row, col)

        Returns:
            Value at the specified location (uint8)
        """
        if isinstance(key, tuple):
            if len(key) != 2:
                raise IndexError(
                    "Image indexing requires either a single index or (row, col)"
                )
            row, col = key
            if row < 0 or row >= self._height or col < 0 or col >= self._width:
                raise IndexError(
                    f"Image pixel ({row}, {col}) out of range for size {self._width}x{self._height}"
                )
            return self.get_pixel(row, col)
        else:
            if key < 0 or key >= self.size:
                raise IndexError(f"Image index {key} out of range for size {self.size}")
            return self.get_element(key)
