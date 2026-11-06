add_subdirectory(containers)
add_subdirectory(math)

target_sources(veer
	PRIVATE
        timestamp.cpp
        veer_system_allocator.cpp
        debug.cpp
)