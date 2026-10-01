add_subdirectory(backends)

target_sources(${PROJECT_NAME}
	PRIVATE
        command_queue_base.cpp
        constant_buffer.cpp
        constant_buffer_definition.cpp
        shader_compiler.cpp
        submit_context.cpp
        swap_chain.cpp
        technique.cpp
)