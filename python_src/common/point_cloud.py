"""3D point cloud (int32) vector with wasm bindings"""

import ctypes

import numpy as np
from wasmtime import Store, Instance
from python_src.common.class_wrapper import WasmPtr


class PointCloud(WasmPtr):
    """Wrapper around 3D point cloud (int32) on the wasm side"""

    call_names = (
        "get_point_cloud_size",
        "get_point_cloud_element",
        "set_point_cloud_element",
        "get_point_cloud_point",
        "set_point_cloud_point",
        "get_point_cloud_data_ptr",
        "get_point_cloud_num_edges",
        "get_point_cloud_edges_ptr",
    )

    def __init__(self, store: Store, instance: Instance, num_points: int):
        """
        Create a new blank point cloud with the specified number of points

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            num_points: Number of 3D points in the cloud
        """
        super().__init__(
            store,
            instance,
            "create_blank_point_cloud",
            self.call_names,
            "free_point_cloud",
            num_points,
        )
        self._num_points = num_points

    @property
    def num_points(self) -> int:
        """Get the number of points in the cloud"""
        return self._num_points

    @property
    def size(self) -> int:
        """Get the total size of the point cloud (num_points * 3)"""
        return self("get_point_cloud_size")

    def get_element(self, index: int) -> int:
        """
        Get element at linear index

        Args:
            index: Linear index into the point cloud data (0 to size-1)

        Returns:
            Value at index (int32)
        """
        return self("get_point_cloud_element", index)

    def set_element(self, index: int, value: int) -> int:
        """
        Set element at linear index

        Args:
            index: Linear index into the point cloud data
            value: Value to set (int32)

        Returns:
            1 if successful, 0 if index out of bounds
        """
        return self("set_point_cloud_element", index, value)

    def get_point(self, point_index: int) -> tuple[int, int, int]:
        """
        Get 3D point at specified index

        Args:
            point_index: Index of the point (0 to num_points-1)

        Returns:
            Tuple of (x, y, z) coordinates (int32)
        """
        x = self("get_point_cloud_point", point_index, 0)
        y = self("get_point_cloud_point", point_index, 1)
        z = self("get_point_cloud_point", point_index, 2)
        return (x, y, z)

    def set_point(self, point_index: int, x: int, y: int, z: int) -> int:
        """
        Set 3D point at specified index

        Args:
            point_index: Index of the point (0 to num_points-1)
            x: X coordinate (int32)
            y: Y coordinate (int32)
            z: Z coordinate (int32)

        Returns:
            1 if successful, 0 if out of bounds
        """
        success_x = self("set_point_cloud_point", point_index, 0, x)
        success_y = self("set_point_cloud_point", point_index, 1, y)
        success_z = self("set_point_cloud_point", point_index, 2, z)
        return min(success_x, success_y, success_z)

    def to_numpy(self) -> np.ndarray:
        """
        Get point cloud data as a numpy array

        Returns:
            2D numpy array of shape (num_points, 3) with dtype int32
        """
        # Get the raw data pointer
        data_ptr = self("get_point_cloud_data_ptr")

        # Get the memory from the instance
        memory = self.instance.exports(self.store)["memory"]

        # Create numpy array view of the WASM memory
        size = self.size
        memory_data = memory.data_ptr(self.store)

        # Create a numpy array from the memory buffer using ctypes
        buffer_start = ctypes.addressof(memory_data.contents) + data_ptr
        buffer = (ctypes.c_int32 * size).from_address(buffer_start)

        arr = np.frombuffer(buffer, dtype=np.int32).reshape(self._num_points, 3)

        return arr.copy()  # Return a copy to avoid memory issues

    def edges_to_numpy(self) -> np.ndarray:
        """
        Get edges data as a numpy array

        Returns:
            2D numpy array of shape (num_edges, 2) with dtype uint32
            Each row contains [first_vertex_index, second_vertex_index]
            Note: WASM uses 32-bit size_t, so edges are stored as uint32
        """
        # Get the number of edges
        num_edges = self("get_point_cloud_num_edges")

        if num_edges == 0:
            return np.array([], dtype=np.uint32).reshape(0, 2)

        # Get the raw edges pointer
        edges_ptr = self("get_point_cloud_edges_ptr")

        # Get the memory from the instance
        memory = self.instance.exports(self.store)["memory"]

        # Create numpy array view of the WASM memory
        memory_data = memory.data_ptr(self.store)

        # Create a numpy array from the memory buffer using ctypes
        # Each edge is a pair of uint32_t values (2 * num_edges total values)
        # WASM uses 32-bit size_t, so we read as uint32
        buffer_start = ctypes.addressof(memory_data.contents) + edges_ptr
        buffer = (ctypes.c_uint32 * (num_edges * 2)).from_address(buffer_start)

        arr = np.frombuffer(buffer, dtype=np.uint32).reshape(num_edges, 2)

        return arr.copy()  # Return a copy to avoid memory issues

    def __len__(self) -> int:
        """Return the number of points in the cloud"""
        return self._num_points

    def __setitem__(self, key, value):
        """
        Support cloud[index] = value or cloud[point_index] = (x, y, z) syntax

        Args:
            key: Either a single index for linear access or point index for 3D point
            value: Either a single int for linear access or tuple of (x, y, z)
        """
        if isinstance(value, (tuple, list)):
            if len(value) != 3:
                raise ValueError(
                    "Point cloud point must have exactly 3 coordinates (x, y, z)"
                )
            x, y, z = value
            success = self.set_point(key, x, y, z)
            if success == 0:
                raise IndexError(
                    f"Point cloud index {key} out of range for {self._num_points} points"
                )
        else:
            success = self.set_element(key, value)
            if success == 0:
                raise IndexError(
                    f"Point cloud element index {key} out of range for size {self.size}"
                )

    def __getitem__(self, key):
        """
        Support cloud[index] syntax for getting points

        Args:
            key: Point index (0 to num_points-1)

        Returns:
            Tuple of (x, y, z) coordinates for the point
        """
        if key < 0 or key >= self._num_points:
            raise IndexError(
                f"Point cloud index {key} out of range for {self._num_points} points"
            )
        return self.get_point(key)
