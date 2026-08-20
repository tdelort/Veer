add_subdirectory(containers)
add_subdirectory(math)

target_sources(${PROJECT_NAME}
	PRIVATE
        veer_system_allocator.cpp
        debug.cpp
)