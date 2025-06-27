add_subdirectory(backends)

target_sources(${PROJECT_NAME}
	PRIVATE
        command_buffer.cpp
        command_queue.cpp
        render_device_resource.cpp
        render_device.cpp
        rendering_service.cpp
        swap_chain.cpp
)