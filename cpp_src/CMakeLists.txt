cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(MathWasm VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the project GUID for Visual Studio (matches the existing .sln file)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Automatically find all source files, excluding CMake build directories
file(GLOB_RECURSE SOURCES
    "*.cpp"
    "*.c"
)

file(GLOB_RECURSE HEADERS
    "*.h"
    "*.hpp"
)

# Remove CMake generated files from the source list
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles/.*")
list(FILTER HEADERS EXCLUDE REGEX "CMakeFiles/.*")

# Also exclude any build directories
list(FILTER SOURCES EXCLUDE REGEX "build/.*")
list(FILTER HEADERS EXCLUDE REGEX "build/.*")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_ProjectGuid "{A7F4F3C2-9B8D-4E1A-8F2C-3D4E5F6A7B8C}"
    VS_GLOBAL_VCProjectVersion "16.0"
    VS_GLOBAL_Keyword "Win32Proj"
    VS_GLOBAL_RootNamespace "MathWasm"
    VS_GLOBAL_WindowsTargetPlatformVersion "10.0"
)

# Include current directory for headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set compiler options for different configurations
if(MSVC)
    # Debug configuration
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG;_CONSOLE;NATIVE_DEBUG>
        $<$<CONFIG:Release>:NDEBUG;_CONSOLE;NATIVE_DEBUG>
        $<$<PLATFORM_ID:Win32>:WIN32>
    )
    
    # Set warning level and other MSVC-specific options
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W3  # Warning level 3
        $<$<CONFIG:Debug>:/Od>  # Disable optimization for debug
        $<$<CONFIG:Release>:/O2>  # Optimize for speed in release
    )
    
    # Set subsystem to console
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_ConfigurationType "Application"
        VS_GLOBAL_UseDebugLibraries "$<$<CONFIG:Debug>:true>"
        VS_GLOBAL_PlatformToolset "v143"
        VS_GLOBAL_CharacterSet "Unicode"
        VS_GLOBAL_WholeProgramOptimization "$<$<CONFIG:Release>:true>"
    )
    
    # Set linker options
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Print information about discovered files
message(STATUS "Found source files:")
foreach(SOURCE_FILE ${SOURCES})
    message(STATUS "  ${SOURCE_FILE}")
endforeach()

message(STATUS "Found header files:")
foreach(HEADER_FILE ${HEADERS})
    message(STATUS "  ${HEADER_FILE}")
endforeach()

# Create source groups for Visual Studio filters (similar to .vcxproj.filters)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

# Set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Configure build configurations to match the original project
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()