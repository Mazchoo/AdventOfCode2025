"""Day 04"""

from pathlib import Path

import pytest
import numpy as np

from python_src.common.wasm_handle import load_wasm_module
from python_src.day07.laser_image import LaserImage

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


def test_LaserImage_produces_numpy_image():
    """Test RollsImage.from_payload(STORE, INSTANCE, str) -> np.ndarray"""

    path = "./python_src/day07/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    laser_image = LaserImage.from_payload(STORE, INSTANCE, sample_input)
    assert isinstance(laser_image.to_numpy(), np.ndarray)


def test_RollsImage_remove_generation_sample_input():
    """Test RollsImage.remove_generation(), edits np.ndarray in place"""

    path = "./python_src/day07/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    laser_image = LaserImage.from_payload(STORE, INSTANCE, sample_input)
    result = laser_image.simulate_splitting_lasers()

    assert result == 21


def test_RollsImage_remove_generation():
    """Test RollsImage.remove_generation(), edits np.ndarray in place"""

    path = "./python_src/day07/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    laser_image = LaserImage.from_payload(STORE, INSTANCE, sample_input)
    result = laser_image.simulate_splitting_lasers()

    assert result == 1579


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
