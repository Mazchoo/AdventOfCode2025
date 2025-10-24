@echo off
echo Building C++ to WebAssembly...

REM Compile with emscripten
emcc mod.cpp ^
  -o math.js ^
  -s EXPORTED_FUNCTIONS="['_add', '_multiply', '_factorial', '_power']" ^
  -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" ^
  -s MODULARIZE=1 ^
  -s EXPORT_NAME="MathModule" ^
  -s ENVIRONMENT=web ^
  -s ALLOW_MEMORY_GROWTH=1 ^
  -O3

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Generated files:
    dir math.js math.wasm 2>nul
) else (
    echo Build failed!
    exit /b 1
)