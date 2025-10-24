@echo off
echo Building C++ to WebAssembly using CMake...

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure with emscripten
echo Configuring with emcmake...
emcmake cmake ..
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

REM Build the project
echo Building with emmake...
cmake --build .
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

echo Build successful!
