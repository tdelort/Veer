add_subdirectory(backends)

target_sources(${PROJECT_NAME}
	PRIVATE
        command_queue.cpp
        render_device.cpp
        rendering_service.cpp
        shader_compiler.cpp
        swap_chain.cpp
        technique.cpp
)