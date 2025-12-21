"""Visualisation functions this day"""

from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap, BoundaryNorm

from python_src.common.wasm_handle import load_wasm_module
from python_src.day07.laser_image import LaserImage

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

cmap = ListedColormap(["black", "orange", "blue", "red"])
norm = BoundaryNorm(boundaries=[-0.5, 0.5, 1.5, 2.5, 3.5], ncolors=4)


path = "./python_src/day07/input/full.txt"
sample_input = Path(path).open(encoding="utf-8").read()
laser_image = LaserImage.from_payload(STORE, INSTANCE, sample_input)

# Show initial input
plt.imshow(laser_image.to_numpy(), cmap=cmap, norm=norm)
plt.show()

# Show input after remove generation
result = laser_image.simulate_splitting_lasers()
plt.imshow(laser_image.to_numpy(), cmap=cmap, norm=norm)
print(f"Number of splits: {result}")
plt.show()
