# C# WASM Testing with Wasmtime

This project demonstrates how to load and test WebAssembly (WASM) modules in C# using the Wasmtime runtime. It imports the WASM module from `../emcc_wasm/build/mod.wasm` and provides comprehensive tests similar to the Python implementation.

## Project Structure

- **`WasmTest.csproj`** - Project file with .NET 8.0 and Wasmtime dependencies
- **`WasmHandle.cs`** - WASM module wrapper class for loading and function access
- **`WasmTimeTests.cs`** - xUnit test suite testing exported WASM functions

## Prerequisites

- .NET 8.0 SDK
- WASM module at `../emcc_wasm/build/mod.wasm`

## Quick Start

```bash
# Navigate to the C# project directory
cd cs_src

# Check build
dotnet build

# Run tests with detailed output
dotnet test --logger "console;verbosity=detailed"
```

## Tested Functions

The test suite validates these WASM exported functions:

### `add(int, int) -> int`
Tests integer addition with various positive, negative, and zero values.

### `multiply(int, int) -> int` 
Tests integer multiplication including edge cases with zero and negative numbers.

### `factorial(int) -> int`
Tests factorial calculation for values 0-10, comparing against C# implementation.

### `power(double, int) -> double`
Tests floating-point power function with various base and exponent combinations, using approximate equality for floating-point comparison.

## Key Features

- **Resource Management**: Proper disposal of WASM resources using `IDisposable`
- **Error Handling**: File existence validation and function export verification
- **Test Output**: Detailed logging of test cases and results
- **Cross-Platform**: Compatible with Windows, macOS, and Linux

## Example Test Output

```
=== Testing add function ===
add(5, 3) = 8
add(10, -2) = 8
add(0, 0) = 0
✓ add function tests passed!

=== Testing factorial function ===
factorial(5) = 120
factorial(0) = 1
✓ factorial function tests passed!
```

## Dependencies

- **Wasmtime**: .NET bindings for the Wasmtime WebAssembly runtime
- **xUnit**: Testing framework with test output helpers
- **Microsoft.NET.Test.Sdk**: .NET testing infrastructure

## Implementation Notes

- The `WasmHandle` class manages the Wasmtime engine, store, and instance lifecycle
- Emscripten memory growth notifications are handled via a no-op callback function
- Floating-point comparisons use epsilon-based equality checking
- The WASM file is automatically copied to the output directory during build

## Comparison with Python Implementation

This C# implementation provides equivalent functionality to `python_src/day00/test_wasm_time.py`:
- Same test cases and expected results
- Similar error handling and logging
- Equivalent function call patterns adapted for C# syntax