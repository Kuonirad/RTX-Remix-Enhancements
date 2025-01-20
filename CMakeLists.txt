cmake_minimum_required(VERSION 3.15)
project(RTX-Remix-Enhancements CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Add source files
add_library(rtx_remix_enhancements
    src/core/kuonirad_hpc_pipeline.cpp
    src/core/kuonirad_hpc_pipeline_options.cpp
    src/core/ai_inference.cpp
    src/pde/pde_solver.cpp
    src/raytracing/wavefront_kernel.cpp
    src/omniverse/usd_integration.cpp
)

# Include directories
target_include_directories(rtx_remix_enhancements PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against required libraries
target_link_libraries(rtx_remix_enhancements PUBLIC
    Threads::Threads
)

# Enable compiler warnings
if(MSVC)
    target_compile_options(rtx_remix_enhancements PRIVATE /W4)
else()
    target_compile_options(rtx_remix_enhancements PRIVATE -Wall -Wextra -std=c++20)
endif()
