cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VeerEngine VERSION 0.1.0)

add_library(${PROJECT_NAME} SHARED )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES

    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include

    PRIVATE 
        src

    PRIVATE 
        libraries/glfw-3.3.8.bin.WIN64/include
        libraries/D3D12MemoryAllocator/include
        ${VULKAN_SDK}/Include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE 
        ${VULKAN_SDK}/Lib
        "libraries/glfw-3.3.8.bin.WIN64/lib-vc2017"
)

target_link_libraries(${PROJECT_NAME}
    vulkan-1.lib
    glfw3.lib
    d3d12.lib
    dxgi.lib
    dxguid.lib
)
