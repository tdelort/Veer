add_subdirectory(backends)

target_sources(veer
	PRIVATE
        command_queue_base.cpp
        constant_buffer.cpp
        constant_buffer_definition.cpp
        shader_compiler.cpp
        submit_context.cpp
        swap_chain.cpp
        technique.cpp
)