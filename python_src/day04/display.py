"""Visualisation functions this day"""

from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap, BoundaryNorm

from python_src.common.wasm_handle import load_wasm_module
from python_src.day04.rolls_image import RollsImage

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

cmap = ListedColormap(["red", "purple", "blue"])
norm = BoundaryNorm(boundaries=[-0.5, 0.5, 1.5, 2.5], ncolors=3)


path = "./python_src/day04/input/full.txt"
sample_input = Path(path).open(encoding="utf-8").read()
rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)

# Show initial input
plt.imshow(rolls_image.to_numpy(), cmap=cmap, norm=norm)
plt.show()

# Show input after remove generation
rolls_image.remove_generation()
plt.imshow(rolls_image.to_numpy(), cmap=cmap, norm=norm)
plt.show()

rolls_image = RollsImage.from_payload(STORE, INSTANCE, sample_input)
i = 0
while rolls_image.remove_dead_cells():
    i += 1
    print(f"Generation {i}")

plt.imshow(rolls_image.to_numpy(), cmap=cmap, norm=norm)
plt.show()
