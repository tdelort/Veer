add_subdirectory(backends)

target_sources(${PROJECT_NAME}
	PRIVATE
        command_buffer.cpp
        command_queue.cpp
        render_device_resource.cpp
        render_device_buffer.cpp
        render_device_texture.cpp
        render_device.cpp
        rendering_service.cpp
        shader_compiler.cpp
        swap_chain.cpp
        technique.cpp
)