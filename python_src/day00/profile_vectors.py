"""Test the speed of exported objects"""

from time import perf_counter

import numpy as np

from python_src.common.wasm_handle import load_wasm_module

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

ARRAY_SIZE = 1_000_000

input_array = np.array(range(ARRAY_SIZE), dtype=np.float32)

start = perf_counter()
input_array.sum()
end = perf_counter()
print(f"Numpy base measurement {(end - start):.3}")

create_func = INSTANCE.exports(STORE)["create_float_vector"]
free_func = INSTANCE.exports(STORE)["free_float_vector"]
sum_func = INSTANCE.exports(STORE)["sum_float_vector"]

# Default argments, about 4x slower
vector = create_func(STORE, ARRAY_SIZE)

start = perf_counter()
sum_func(STORE, vector)
end = perf_counter()
print(f"Vector wasm measurement {(end - start):.3}")

free_func(STORE, vector)


create_func = INSTANCE.exports(STORE)["create_float_array"]
free_func = INSTANCE.exports(STORE)["free_float_array"]
sum_func = INSTANCE.exports(STORE)["sum_float_array"]

array = create_func(STORE, ARRAY_SIZE)

start = perf_counter()
sum_func(STORE, array, ARRAY_SIZE)
end = perf_counter()
print(f"Vector wasm measurement {(end - start):.3}")

free_func(STORE, vector)
