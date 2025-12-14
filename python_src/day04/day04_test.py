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
    rolls_removed = rolls_image.remove_generation()

    image_data = rolls_image.to_numpy()
    assert rolls_removed
    assert (image_data == 2).sum() == 13


def test_RollsImage_remove_generation():
    """Test RollsImage.remove_generation(), edits np.ndarray in place"""

    path = "./python_src/day04/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)
    rolls_removed = rolls_image.remove_generation()

    image_data = rolls_image.to_numpy()
    assert rolls_removed
    assert (image_data == 2).sum() == 1486


def test_RollsImage_remove_dead_cells_until_finished_sample_input():
    """Test RollsImage.remove_dead_cells(), edits np.ndarry returns number removed"""

    path = "./python_src/day04/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)

    expected_removals = [13, 12, 7, 5, 2, 1, 1, 1, 1, 0]
    for expected_nr in expected_removals:
        assert rolls_image.remove_dead_cells() == expected_nr


def test_RollsImage_remove_dead_cells_until_finished():
    """Test RollsImage.remove_dead_cells(), edits np.ndarry returns number removed"""

    path = "./python_src/day04/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)

    total_removals = 0
    while nr_removed := rolls_image.remove_dead_cells():
        total_removals += nr_removed

    assert total_removals == 9024


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
