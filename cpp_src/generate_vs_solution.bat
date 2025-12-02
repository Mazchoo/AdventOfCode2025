@echo off
REM Script to generate Visual Studio solution from CMakeLists.txt
REM This will create a build directory and generate .sln file

echo ========================================
echo Visual Studio Solution Generator
echo ========================================
echo.

REM Check if CMake is available
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake is not found in PATH
    echo Please install CMake from https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

REM Get CMake version
echo Checking CMake version...
cmake --version
echo.

REM Set build directory
set BUILD_DIR=build

REM Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" (
    echo Creating build directory: %BUILD_DIR%
    mkdir "%BUILD_DIR%"
) else (
    echo Build directory already exists: %BUILD_DIR%
)
echo.

REM Change to build directory
cd "%BUILD_DIR%"

REM Run CMake to generate Visual Studio solution
echo Generating Visual Studio solution...
echo Running: cmake -G "Visual Studio 17 2022" -A x64 ..
echo.

cmake -G "Visual Studio 17 2022" -A x64 ..

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake generation failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo SUCCESS!
echo ========================================
echo.
echo Visual Studio solution generated successfully!
echo Location: %CD%\MathWasm.sln
echo.
echo You can now:
echo   1. Open the solution: start MathWasm.sln
echo   2. Build from command line: cmake --build . --config Debug
echo   3. Run tests: ctest -C Debug
echo.

REM Return to original directory
cd ..

REM Ask if user wants to open the solution
set /p OPEN_SOLUTION="Do you want to open the solution in Visual Studio? (Y/N): "
if /i "%OPEN_SOLUTION%"=="Y" (
    echo Opening Visual Studio...
    start "" "%BUILD_DIR%\MathWasm.sln"
)

echo.
pause