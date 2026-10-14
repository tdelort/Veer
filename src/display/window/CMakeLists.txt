add_subdirectory(glfw)

target_sources(veer
	PRIVATE
        window.cpp
        windowing_service.cpp
)
