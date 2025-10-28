"""ToDo The cmake file has be generated each time using the exported functions in mod.cpp"""

import os
import re
from typing import List, Optional

CMAKE_TEMPLATE = """
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
add_executable(mod {sources_str})

# Add include directory for headers
target_include_directories(mod PRIVATE ../cpp_src)

# Set emscripten-specific flags
set_target_properties(mod PROPERTIES
    SUFFIX ".js"
    LINK_FLAGS "{link_flags}"
)

# Output files will be mod.js and math.wasm
set_target_properties(mod PROPERTIES
    OUTPUT_NAME "mod"
)

# Custom target for build info
add_custom_command(TARGET mod POST_BUILD
    COMMAND ${{CMAKE_COMMAND}} -E echo "Build successful!"
    COMMAND ${{CMAKE_COMMAND}} -E echo "Generated files:"
    COMMAND ${{CMAKE_COMMAND}} -E echo "  - mod.js"
    COMMAND ${{CMAKE_COMMAND}} -E echo "  - mod.wasm"
    WORKING_DIRECTORY ${{CMAKE_CURRENT_BINARY_DIR}}
)"""


def find_source_files(src_dir: str, wasm_dir: str) -> List[str]:
    """Find all .cpp, .h, and .hpp files in the src directory."""
    source_files = []
    extensions = {".cpp", ".h", ".hpp"}

    if not os.path.exists(src_dir):
        return source_files

    for root, dirs, files in os.walk(src_dir):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                # Get relative path from wasm directory to cpp_src
                rel_path = os.path.relpath(
                    os.path.join(root, file), wasm_dir
                )
                source_files.append(
                    rel_path.replace("\\", "/")
                )  # Use forward slashes for CMake

    return sorted(source_files)


def extract_exported_functions(mod_cpp_path: str) -> List[str]:
    """Extract exported function names from mod.cpp."""
    exported_functions = []

    if not os.path.exists(mod_cpp_path):
        return exported_functions

    with open(mod_cpp_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Look for functions marked with EXPORT_FUNC
    # Pattern: EXPORT_FUNC followed by function definition
    pattern = r"EXPORT_FUNC\s+(?:[\w\s\*]+\s+)?(\w+)\s*\("
    matches = re.findall(pattern, content)

    # Add underscore prefix for emscripten exported functions
    exported_functions = [f"'_{func}'" for func in matches]

    return exported_functions


def generate_cmake_lists(wasm_dir: str = "wasm") -> str:
    """Generate CMakeLists.txt content."""

    # Paths - now reference cpp_src directory
    cpp_src_dir = os.path.join(os.path.dirname(wasm_dir), "cpp_src")
    mod_cpp_path = os.path.join(cpp_src_dir, "mod.cpp")
    src_dir = os.path.join(cpp_src_dir, "src")

    # Get source files and exported functions
    source_files = find_source_files(src_dir, wasm_dir)
    exported_functions = extract_exported_functions(mod_cpp_path)

    # Build the source file list for add_executable with relative paths from wasm dir
    mod_cpp_rel = os.path.relpath(mod_cpp_path, wasm_dir).replace("\\", "/")
    all_sources = [mod_cpp_rel] + source_files
    sources_str = " ".join(all_sources)

    # Build exported functions string
    exported_funcs_str = (
        "[" + ",".join(exported_functions) + "]" if exported_functions else "[]"
    )

    # Build link flags to avoid long lines
    link_flags = (
        f"-s EXPORTED_FUNCTIONS={exported_funcs_str} "
        "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] "
        "-s MODULARIZE=1 -s EXPORT_NAME=MathModule "
        "-s ENVIRONMENT=web -s ALLOW_MEMORY_GROWTH=1 -O3"
    )

    return CMAKE_TEMPLATE.format(sources_str=sources_str, link_flags=link_flags)


def write_cmake_lists(wasm_dir: str = "wasm", output_path: Optional[str] = None):
    """Generate and write CMakeLists.txt file."""
    if output_path is None:
        output_path = os.path.join(wasm_dir, "CMakeLists.txt")

    content = generate_cmake_lists(wasm_dir)

    with open(output_path, "w", encoding="utf-8") as f:
        f.write(content)

    print(f"Generated CMakeLists.txt at: {output_path}")

    # Print summary
    cpp_src_dir = os.path.join(os.path.dirname(wasm_dir), "cpp_src")
    mod_cpp_path = os.path.join(cpp_src_dir, "mod.cpp")
    src_dir = os.path.join(cpp_src_dir, "src")
    source_files = find_source_files(src_dir, wasm_dir)
    exported_functions = extract_exported_functions(mod_cpp_path)

    print(f"Found {len(source_files)} source files in src/:")
    for file in source_files:
        print(f"  - {file}")

    print(f"Found {len(exported_functions)} exported functions:")
    for func in exported_functions:
        print(f"  - {func}")


if __name__ == "__main__":
    # Generate CMakeLists.txt when run directly
    write_cmake_lists()
