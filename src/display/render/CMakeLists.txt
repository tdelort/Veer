add_subdirectory(backends)

target_sources(${PROJECT_NAME}
	PRIVATE
        command_queue_base.cpp
        shader_compiler.cpp
        submit_context.cpp
        swap_chain.cpp
        technique.cpp
)