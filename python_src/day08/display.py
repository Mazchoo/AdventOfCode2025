"""Visualisation functions this day"""

from pathlib import Path

import open3d as o3d

from python_src.common.wasm_handle import load_wasm_module
from python_src.day08.junction_boxes import JunctionBoxes

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

path = "./python_src/day08/input/full.txt"
sample_input = Path(path).open(encoding="utf-8").read()
points = JunctionBoxes.from_payload(STORE, INSTANCE, sample_input)

pcd = o3d.geometry.PointCloud()
pcd.points = o3d.utility.Vector3dVector(points)

# Display
o3d.visualization.draw_geometries([pcd])
