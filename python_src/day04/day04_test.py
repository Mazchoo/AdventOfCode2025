"""Day 04"""

from pathlib import Path

import pytest
import numpy as np

from python_src.common.wasm_handle import load_wasm_module
from python_src.day04.rolls_image import RollsImage

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


def test_RollsImage_produces_numpy_image():
    """Test RollsImage.from_payload(STORE, INSTANCE, str) -> np.ndarray"""

    path = "./python_src/day04/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)
    assert isinstance(rolls_image.to_numpy(), np.ndarray)


def test_RollsImage_remove_generation_sample_input():
    """Test RollsImage.remove_generation(), edits np.ndarray in place"""

    path = "./python_src/day04/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)
    rolls_image.remove_generation()

    image_data = rolls_image.to_numpy()
    assert (image_data == 2).sum() == 13


def test_RollsImage_remove_generation():
    """Test RollsImage.remove_generation(), edits np.ndarray in place"""

    path = "./python_src/day04/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)
    rolls_image.remove_generation()

    image_data = rolls_image.to_numpy()
    assert (image_data == 2).sum() == 1486


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
