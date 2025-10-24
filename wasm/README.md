# WebAssembly Math Library

This project builds a C++ math library to WebAssembly using Emscripten and CMake.

## Prerequisites

- [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html)
- CMake (version 3.16 or higher)

## Building with CMake

### Option 1: Use the build script (Recommended)
```bash
# Run the CMake build script
./build-cmake.bat
```

### Option 2: Manual CMake build
```bash
# Create and enter build directory
mkdir build
cd build

# Configure with emscripten
emcmake cmake ..

# Build the project
cmake --build .

# Files will be generated in the build directory
```

## Generated Files

The build process generates:
- `mod.js` - JavaScript module
- `mod.wasm` - WebAssembly binary

## Exported Functions

ToDo - update this to reflect AoC functions

The library exports the following functions:
- `add(a, b)` - Addition
- `multiply(a, b)` - Multiplication
- `factorial(n)` - Factorial calculation
- `power(base, exponent)` - Power calculation

## Usage Example

```javascript
// Load the module
import MathModule from './math.js';

MathModule().then(Module => {
    // Use the exported functions
    const add = Module.cwrap('add', 'number', ['number', 'number']);
    const result = add(5, 3); // Returns 8
});
```

## Legacy Build

The original build script (`build.bat`) is still available for comparison, but the CMake build is now the recommended approach.