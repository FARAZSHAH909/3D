cmake_minimum_required(VERSION 3.15)
project(AntiGravity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/Pipeline/*.cpp" "${CMAKE_SOURCE_DIR}/Pipeline/*.c")

add_executable(AntiGravity WIN32 ${SRC_FILES})

target_include_directories(AntiGravity PRIVATE "${CMAKE_SOURCE_DIR}/Pipeline")

target_compile_definitions(AntiGravity PRIVATE UNICODE _UNICODE)
if (MSVC)
  target_compile_options(AntiGravity PRIVATE /EHsc)
endif()

# Link required DirectX libraries
target_link_libraries(AntiGravity PRIVATE d3d12 dxgi d3dcompiler)

# Output and debugger working directory
set_target_properties(AntiGravity PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Pipeline"
)

# Copy pipeline folder (shaders, models) to output directory after build so runtime finds resources
add_custom_command(TARGET AntiGravity POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Pipeline"
    "$<TARGET_FILE_DIR:AntiGravity>"
)
