"""Day08 specific point cloud class with junction boxes functionality"""

from wasmtime import Store, Instance
from python_src.common.point_cloud import PointCloud
from python_src.common.call_advent_function import call_wasm_with_string


class JunctionBoxes(PointCloud):
    """Day08-specific point cloud wrapper with junction boxes functionality"""

    call_names = ()

    def __init__(self, store: Store, instance: Instance, num_points: int):
        """
        Create a new blank point cloud with the specified number of points

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            num_points: Number of points in cloud
        """
        # Call parent constructor first
        super().__init__(store, instance, num_points)

        # Add day08-specific function to call_funcs
        self.call_funcs.update({n: instance.exports(store)[n] for n in self.call_names})

    @classmethod
    def from_payload(
        cls, store: Store, instance: Instance, payload: str
    ) -> "JunctionBoxes":
        """
        Create a point cloud from a string payload (day08 format)

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            payload: String payload to parse into a point cloud

        Returns:
            JunctionBoxes instance created from the payload
        """
        # Create instance with special constructor
        pc = cls.__new__(cls)
        pc.store = store
        pc.instance = instance

        pc.call_funcs = {
            name: instance.exports(store)[name] for name in PointCloud.call_names
        }
        pc.call_funcs.update({n: instance.exports(store)[n] for n in cls.call_names})

        # Call create_point_cloud_day8 with payload using proper memory allocation
        create_func = instance.exports(store)["create_point_cloud_day8"]
        pc.handle = call_wasm_with_string(store, instance, payload, create_func)

        pc.free_func = instance.exports(store)["free_point_cloud"]

        # Get number of points from the created point cloud
        pc._num_points = pc("get_point_cloud_num_points")

        return pc
