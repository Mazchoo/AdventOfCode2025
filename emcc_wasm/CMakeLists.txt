
cmake_minimum_required(VERSION 3.16)
project(MathWasm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Verify we're using Emscripten
if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "This project requires Emscripten. Please use emcmake to configure.")
endif()

# Download nanoflann header if it doesn't exist
set(NANOFLANN_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../cpp_src/external/nanoflann.hpp")
if(NOT EXISTS ${NANOFLANN_HEADER})
    message(STATUS "Downloading nanoflann.hpp...")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../cpp_src/external")
    file(DOWNLOAD
        "https://raw.githubusercontent.com/jlblancoc/nanoflann/master/include/nanoflann.hpp"
        ${NANOFLANN_HEADER}
        SHOW_PROGRESS
        STATUS DOWNLOAD_STATUS
    )
    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if(NOT STATUS_CODE EQUAL 0)
        message(WARNING "Failed to download nanoflann.hpp automatically. Please download it manually from https://raw.githubusercontent.com/jlblancoc/nanoflann/master/include/nanoflann.hpp and place it in the ../cpp_src/external/ directory.")
    else()
        message(STATUS "nanoflann.hpp downloaded successfully!")
    endif()
else()
    message(STATUS "nanoflann.hpp already exists")
endif()

# Define the executable
add_executable(mod ../cpp_src/mod.cpp)

# Add include directory for headers
target_include_directories(mod PRIVATE
    ../cpp_src
    ../cpp_src/external
)

# Set emscripten-specific flags for WASM
set_target_properties(mod PROPERTIES
    SUFFIX ".wasm"
    LINK_FLAGS "-s WASM=1 -s STANDALONE_WASM=1 -s EXPORT_ALL=1 --no-entry -O3 -ffast-math -funroll-loops -s ALLOW_MEMORY_GROWTH=1 -msimd128"
)

# Add compile flags for float optimization and SIMD
target_compile_options(mod PRIVATE
    -O3
    -ffast-math
    -funroll-loops
    -fno-math-errno
    -ffinite-math-only
    -msimd128
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
