if(BUILD_TESTS)
	target_sources(${PROJECT_NAME}
		PRIVATE
		vec_tests.cpp
		mat_tests.cpp
	)
endif()