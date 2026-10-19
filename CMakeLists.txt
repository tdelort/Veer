cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VeerEngine VERSION 0.1.0)

add_library(veer STATIC )
option(BUILD_TESTS "Build tests" OFF)

target_compile_features(veer PUBLIC cxx_std_20)
set_target_properties(
    veer PROPERTIES

    CMAKE_CXX_STANDARD 20
    CMAKE_CXX_STANDARD_REQUIRED TRUE
    CMAKE_CXX_SCAN_FOR_MODULES ON
    CMAKE_CXX_EXTENSIONS OFF

    DEBUG_POSTFIX "D"
    RELWITHDEBINFO_POSTFIX "RD"
)

target_compile_definitions(veer PRIVATE
  $<$<CONFIG:Debug>:_DEBUG>
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Xclang -gcodeview -D_DEBUG -DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


# GLFW build 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLFW_PATH libraries/glfw)
set(GLFW_TARGET glfw)
add_subdirectory(${GLFW_PATH})

# don't need it yet
# find_package(OpenGL REQUIRED)

# D3D12MA build 
set(D3D12MA_PATH libraries/D3D12MA)
set(D3D12MA_TARGET D3D12MemoryAllocator)
add_subdirectory(${D3D12MA_PATH})

# this project build 
add_subdirectory(src)

target_include_directories(veer
    PUBLIC 
        src

    # allow to internally link directly without prefixing "veer"
    PRIVATE 
        src/veer

    PRIVATE 
        libraries/glfw-3.3.8.bin.WIN64/include
        libraries/D3D12MemoryAllocator/include
)

target_link_directories(veer
    PUBLIC 
        "libraries/glfw-3.3.8.bin.WIN64/lib-vc2017"
)

target_link_libraries(veer
    PUBLIC
        # don't need it yet
        # OpenGL::GL
        ${GLFW_TARGET}
        ${D3D12MA_TARGET}
        d3d12.lib
        dxgi.lib
        dxguid.lib
)
