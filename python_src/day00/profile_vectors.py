"""Test the speed of exported objects"""

from time import perf_counter

import numpy as np

from python_src.common.wasm_handle import load_wasm_module
from python_src.common.vector_f32 import VectorF32
from python_src.common.c_array_f32 import CArrayF32
from python_src.common.byte_array import ByteArray


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

byte_array = ByteArray(STORE, INSTANCE, ARRAY_SIZE)

start = perf_counter()
byte_array.sum()
end = perf_counter()
print(f"C style byte wasm measurement {(end - start):.3}")

start = perf_counter()
byte_array.sum_simd()
end = perf_counter()
print(f"C style byte wasm simd measurement {(end - start):.3}")
