"""Test the speed of exported objects"""

from time import perf_counter

import numpy as np

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.vector_f32 import VectorF32
from python_src.common.c_array_f32 import CArrayF32


STORE, INSTANCE = load_wasm_module("./emcc_wasm/build/mod.wasm")
ARRAY_SIZE = 1_000_000

input_float_array = np.array(range(ARRAY_SIZE), dtype=np.float32)

start = perf_counter()
input_float_array.sum()
end = perf_counter()
print(f"Numpy base measurement {(end - start):.3}")

vector = VectorF32(STORE, INSTANCE, ARRAY_SIZE)

start = perf_counter()
vector.sum()
end = perf_counter()
print(f"Vector wasm measurement {(end - start):.3}")


create_func = INSTANCE.exports(STORE)["create_float_array"]
free_func = INSTANCE.exports(STORE)["free_float_array"]
sum_func = INSTANCE.exports(STORE)["sum_float_array"]

c_style_array = CArrayF32(STORE, INSTANCE, ARRAY_SIZE)

start = perf_counter()
c_style_array.sum()
end = perf_counter()
print(f"C style wasm measurement {(end - start):.3}")

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
