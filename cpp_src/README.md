# C++ Project Build Guide

This directory contains the C++ source code for the AdventOfCode2025 project with CMake build configuration and doctest unit testing framework.

## Prerequisites

- **CMake** (version 3.15 or higher)
  - Download from: https://cmake.org/download/
  - Make sure CMake is added to your system PATH

- **Visual Studio 2022** (or compatible version)
  - Visual Studio 2019 (v16) or Visual Studio 2017 (v15) also supported
  - Make sure C++ development tools are installed

- **Internet connection** (for first-time setup to download doctest)

## Project Structure

```
cpp_src/
├── CMakeLists.txt           # Main CMake configuration
├── generate_vs_solution.bat # Automated solution generator
├── README.md                # This file
├── main.cpp                 # Main application entry point
├── mod.cpp / mod.h          # Module files
├── src/
│   └── day00.hpp            # Day 00 implementation (header-only)
└── tests/
    ├── CMakeLists.txt       # Test configuration
    ├── test_day00.cpp       # Day 00 unit tests
    └── doctest.h            # Downloaded automatically by CMake
```

## Quick Start

### Option 1: Using the Batch Script (Recommended)

1. Open a command prompt in the `cpp_src` directory
2. Run the generator script:
   ```batch
   generate_vs_solution.bat
   ```
3. The script will:
   - Check for CMake installation
   - Create a `build` directory
   - Generate the Visual Studio solution
   - Optionally open Visual Studio for you

### Option 2: Manual CMake Generation

1. Open a command prompt in the `cpp_src` directory
2. Create and enter the build directory:
   ```batch
   mkdir build
   cd build
   ```
3. Generate the Visual Studio solution:
   ```batch
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```
   
   For other Visual Studio versions:
   - VS 2019: `cmake -G "Visual Studio 16 2019" -A x64 ..`
   - VS 2017: `cmake -G "Visual Studio 15 2017" -A x64 ..`

4. Open the generated solution:
   ```batch
   start MathWasm.sln
   ```

## Building the Project

### From Visual Studio

1. Open `build/MathWasm.sln`
2. Select your build configuration (Debug/Release)
3. Build → Build Solution (or press F7)
4. Run the main application (Ctrl+F5)

### From Command Line

Build the project:
```batch
cd build
cmake --build . --config Debug
```

Or for Release:
```batch
cmake --build . --config Release
```

Run the executable:
```batch
Debug\MathWasm.exe
```

## Running Tests

The project uses [doctest](https://github.com/doctest/doctest) - a lightweight C++ testing framework.

### First Time Setup

On the first CMake run, doctest.h will be automatically downloaded to the `tests/` directory. If the download fails due to network issues, you can manually download it:

1. Download from: https://raw.githubusercontent.com/doctest/doctest/v2.4.11/doctest/doctest.h
2. Save it as `cpp_src/tests/doctest.h`
3. Re-run CMake

### Running Tests from Command Line

After building, run all tests:
```batch
cd build
ctest -C Debug
```

Or run the test executable directly:
```batch
tests\Debug\tests.exe
```

For verbose output:
```batch
tests\Debug\tests.exe --success
```

List all test cases:
```batch
tests\Debug\tests.exe --list-test-cases
```

Run specific test:
```batch
tests\Debug\tests.exe --test-case="Testing add function"
```

### Running Tests from Visual Studio

The `tests` project is a standalone test executable that runs all your tests:

1. Open the solution in Visual Studio
2. Make sure you're in **Debug** configuration
3. Right-click on the `tests` project → Set as Startup Project
4. Press F5 (or Ctrl+F5 to run without debugging)
5. All tests will run and display results in the console

### Debugging a Specific Test

When a test fails or you want to debug it:

1. Open the test file (e.g., `test_day00.cpp`)
2. Find the failing test case
3. Set a breakpoint inside that test (click in the left margin or press F9)
4. Make sure `tests` is the startup project
5. Press F5 to start debugging
6. The debugger will run all tests until it hits your breakpoint
7. Use F10 (Step Over), F11 (Step Into), F5 (Continue) to debug

**You can also:**
- Set breakpoints in implementation files (e.g., `day00.hpp`) to debug the actual code
- Use the Watch window to inspect variables
- View the Call Stack to see how you got to the current point
- Use Immediate Window to evaluate expressions

**Note:** The `tests` project has its own `main()` function in `test_main.cpp` that runs all doctest tests. You don't need to modify the main project to run tests.

### Test Coverage

Current test suites:
- **Basic Functions**: Tests for `add()` function
- **FloatVector Class**: Constructor, getters, setters, sum operations
- **FloatVector Factory Functions**: Pointer-based operations
- **Float Array Functions**: Raw array operations
- **Byte Array Functions**: uint8_t array operations including SIMD

## Doctest Features

### Writing Tests

Tests are written using doctest macros:

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Description of test") {
    CHECK(1 + 1 == 2);
    REQUIRE(ptr != nullptr);  // Stops test if fails
}

TEST_SUITE("Suite Name") {
    TEST_CASE("Test in suite") {
        // Test code
    }
}
```

### Common Assertions

- `CHECK(expression)` - Continues on failure
- `REQUIRE(expression)` - Stops test on failure
- `CHECK_THROWS(expression)` - Expects exception
- `CHECK_NOTHROW(expression)` - Expects no exception
- `doctest::Approx(value)` - For floating-point comparisons

Example:
```cpp
CHECK(result == doctest::Approx(3.14159).epsilon(0.01));
```

## Troubleshooting

### CMake Not Found
- Ensure CMake is installed and added to PATH
- Restart your command prompt after installation
- Verify with: `cmake --version`

### Visual Studio Version Mismatch
- Edit `generate_vs_solution.bat` to use your VS version
- Or manually specify generator: `cmake -G "Visual Studio 16 2019" -A x64 ..`

### Doctest Download Failed
- Check your internet connection
- Download manually from the URL provided in the error message
- Place `doctest.h` in the `tests/` directory
- Re-run CMake

### Build Errors
- Clean the build directory: `rmdir /s /q build`
- Regenerate: `generate_vs_solution.bat`
- Check that all source files are present

### Test Failures
- Review test output for specific failures
- Run with verbose mode: `tests.exe --success`
- Check implementation in `src/day00.hpp`

## Additional Resources

- **CMake Documentation**: https://cmake.org/documentation/
- **Doctest Documentation**: https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md
- **Visual Studio C++ Docs**: https://docs.microsoft.com/en-us/cpp/

## Project Configuration

### CMake Options

The project is configured with:
- C++17 standard
- Debug and Release configurations
- Automatic source file discovery
- Visual Studio-specific optimizations
- Console subsystem

### Compiler Flags

- **Debug**: `/Od` (no optimization), `/W3` (warning level 3)
- **Release**: `/O2` (optimize for speed), whole program optimization

## Contributing

When adding new functionality:

1. Add implementation to appropriate header/source files
2. Create corresponding test file in `tests/`
3. Update `tests/CMakeLists.txt` if adding new test executables
4. Run all tests to ensure nothing breaks
5. Update this README if adding new build steps

## License

See the LICENSE file in the project root directory.