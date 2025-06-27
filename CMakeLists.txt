cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VeerEngine VERSION 0.1.0)

add_library(${PROJECT_NAME} STATIC )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES

    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)


# GLFW build 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLFW_PATH libraries/glfw)
set(GLFW_TARGET glfw)
add_subdirectory(${GLFW_PATH})

# D3D12MA build 
set(D3D12MA_PATH libraries/D3D12MemoryAllocator)
set(D3D12MA_TARGET D3D12MemoryAllocator)
add_subdirectory(${D3D12MA_PATH})

# this project build 
add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include

    # TODO : meh, when I have a clear seperation between private and public API ( in include vs src ) I should get this back to PRIVATE
    PUBLIC 
        src

    PRIVATE 
        libraries/glfw-3.3.8.bin.WIN64/include
        libraries/D3D12MemoryAllocator/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC 
        "libraries/glfw-3.3.8.bin.WIN64/lib-vc2017"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${GLFW_TARGET}
        ${D3D12MA_TARGET}
        d3d12.lib
        dxgi.lib
        dxguid.lib
        d3dcompiler.lib
)
