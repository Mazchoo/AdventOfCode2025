"""Day04 specific image class with rolls/generation functionality"""

from wasmtime import Store, Instance
from python_src.common.image import Image
from python_src.common.call_advent_function import call_wasm_with_string


class RollsImage(Image):
    """Day04-specific image wrapper with generation removal functionality"""

    def __init__(self, store: Store, instance: Instance, width: int, height: int):
        """
        Create a new blank binary image with the specified dimensions

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            width: Width of the image (pitch)
            height: Height of the image
        """
        # Call parent constructor first
        super().__init__(store, instance, width, height)

        # Add day04-specific function to call_funcs
        self.call_funcs["remove_generation_from_image"] = instance.exports(store)[
            "remove_generation_from_image"
        ]

    @classmethod
    def from_payload(
        cls, store: Store, instance: Instance, payload: str
    ) -> "RollsImage":
        """
        Create a binary image from a string payload (day04 format)

        Args:
            store: Wasmtime store instance
            instance: Wasmtime instance with exported functions
            payload: String payload to parse into an image

        Returns:
            RollsImage instance created from the payload
        """
        # Create instance with special constructor
        img = cls.__new__(cls)
        img.store = store
        img.instance = instance

        img.call_funcs = {
            name: instance.exports(store)[name] for name in Image.call_names
        }
        img.call_funcs["remove_generation_from_image"] = instance.exports(store)[
            "remove_generation_from_image"
        ]

        # Call create_image_day4 with payload using proper memory allocation
        create_func = instance.exports(store)["create_image_day4"]
        img.handle = call_wasm_with_string(store, instance, payload, create_func)

        img.free_func = instance.exports(store)["free_image"]

        # Get dimensions from the created image
        img._width = img("get_image_pitch")
        img._height = img("get_image_height")

        return img

    def remove_generation(self):
        """Apply remove_generation algorithm to the image"""
        self("remove_generation_from_image")
