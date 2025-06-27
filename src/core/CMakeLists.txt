add_subdirectory(containers)
add_subdirectory(math)

target_sources(${PROJECT_NAME}
	PRIVATE
        basic_allocator.cpp
        debug.cpp
)