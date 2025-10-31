"""Test the speed of exported objects"""

from time import perf_counter

import numpy as np

from python_src.common.wasm_handle import load_wasm_module

STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")

ARRAY_SIZE = 1_000_000

input_float_array = np.array(range(ARRAY_SIZE), dtype=np.float32)

start = perf_counter()
input_float_array.sum()
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
print(f"C style wasm measurement {(end - start):.3}")

free_func(STORE, array)

input_byte_array = np.array([1] * ARRAY_SIZE, dtype=np.uint8)

start = perf_counter()
input_byte_array.sum()
end = perf_counter()
print(f"Numpy byte measurement {(end - start):.3}")

create_func = INSTANCE.exports(STORE)["create_byte_array"]
free_func = INSTANCE.exports(STORE)["free_byte_array"]
sum_func = INSTANCE.exports(STORE)["sum_byte_array"]
sum_func_simd = INSTANCE.exports(STORE)["sum_byte_array_simd"]

byte_array = create_func(STORE, ARRAY_SIZE)

start = perf_counter()
sum_func(STORE, byte_array, ARRAY_SIZE)
end = perf_counter()
print(f"C style byte wasm measurement {(end - start):.3}")

start = perf_counter()
sum_func_simd(STORE, byte_array, ARRAY_SIZE)
end = perf_counter()
print(f"C style byte wasm simd measurement {(end - start):.3}")

free_func(STORE, byte_array)
