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
add_executable(math math.cpp)

# Set emscripten-specific flags
set_target_properties(math PROPERTIES
    SUFFIX ".js"
    LINK_FLAGS "-s EXPORTED_FUNCTIONS=['_add','_multiply','_factorial','_power'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s MODULARIZE=1 -s EXPORT_NAME=MathModule -s ENVIRONMENT=web -s ALLOW_MEMORY_GROWTH=1 -O3"
)

# Output files will be math.js and math.wasm
set_target_properties(math PROPERTIES
    OUTPUT_NAME "math"
)

# Custom target for build info
add_custom_command(TARGET math POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build successful!"
    COMMAND ${CMAKE_COMMAND} -E echo "Generated files:"
    COMMAND ${CMAKE_COMMAND} -E echo "  - math.js"
    COMMAND ${CMAKE_COMMAND} -E echo "  - math.wasm"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)