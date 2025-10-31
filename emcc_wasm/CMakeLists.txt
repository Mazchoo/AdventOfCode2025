
cmake_minimum_required(VERSION 3.16)
project(MathWasm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Verify we're using Emscripten
if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "This project requires Emscripten. Please use emcmake to configure.")
endif()

# Define the executable
add_executable(mod ../cpp_src/mod.cpp)

# Add include directory for headers
target_include_directories(mod PRIVATE ../cpp_src)

# Set emscripten-specific flags for WASM
set_target_properties(mod PROPERTIES
    SUFFIX ".wasm"
    LINK_FLAGS "-s WASM=1 -s STANDALONE_WASM=1 -s EXPORT_ALL=1 --no-entry -O3 -ffast-math -funroll-loops -s ALLOW_MEMORY_GROWTH=1"
)

# Add compile flags for float optimization
target_compile_options(mod PRIVATE
    -O3
    -ffast-math
    -funroll-loops
    -fno-math-errno
    -ffinite-math-only
)

# Output file will be mod.wasm
set_target_properties(mod PROPERTIES
    OUTPUT_NAME "mod"
)

# Custom target for build info
add_custom_command(TARGET mod POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build successful!"
    COMMAND ${CMAKE_COMMAND} -E echo "Generated files:"
    COMMAND ${CMAKE_COMMAND} -E echo "  - mod.wasm"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)