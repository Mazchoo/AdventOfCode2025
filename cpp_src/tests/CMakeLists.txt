# Test executable
add_executable(tests
    test_main.cpp
    test_day00.cpp
    test_day01.cpp
)

# Include directories
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set as a test project in Visual Studio
set_target_properties(tests PROPERTIES
    FOLDER "Tests"
)

# Ensure debug symbols are generated for debugging
if(MSVC)
    target_compile_options(tests PRIVATE
        $<$<CONFIG:Debug>:/Zi>  # Generate debug info
        $<$<CONFIG:Debug>:/Od>  # Disable optimization for debugging
    )
    set_target_properties(tests PROPERTIES
        LINK_FLAGS_DEBUG "/DEBUG:FULL"
    )
endif()