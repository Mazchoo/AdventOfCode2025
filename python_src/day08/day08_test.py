"""Day 04"""

from pathlib import Path

import pytest

from python_src.common.wasm_handle import load_wasm_module
from python_src.day08.junction_boxes import JunctionBoxes

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")


def test_JunctionBoxes_produces_expected_components_sample_input():
    """Test JunctionBoxes.connect_closest_points() returns result"""

    path = "./python_src/day08/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    graph = JunctionBoxes.from_payload(STORE, INSTANCE, sample_input)
    result = graph.connect_closest_points(10)

    assert result == 40


def test_JunctionBoxes_produces_expected_components():
    """Test JunctionBoxes.connect_closest_points() returns result"""

    path = "./python_src/day08/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    graph = JunctionBoxes.from_payload(STORE, INSTANCE, sample_input)
    result = graph.connect_closest_points(1000)

    assert result == 75680


def test_LaserImage_calculate_nr_splitting_paths_sample_input():
    """Test LaserImage.calculate_nr_splitting_paths() returns result"""

    path = "./python_src/day08/input/sample.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    graph = JunctionBoxes.from_payload(STORE, INSTANCE, sample_input)
    result = graph.connect_until_saturated()

    assert result == 25272


def test_LaserImage_calculate_nr_splitting_pathslasers():
    """Test LaserImage.calculate_nr_splitting_paths() returns result"""

    path = "./python_src/day08/input/full.txt"
    sample_input = Path(path).open(encoding="utf-8").read()
    graph = JunctionBoxes.from_payload(STORE, INSTANCE, sample_input)
    result = graph.connect_until_saturated()

    assert result == 8995844880


if __name__ == "__main__":
    pytest.main([__file__, "-x", "--verbose"])
