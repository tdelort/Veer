if(BUILD_TESTS)
	target_sources(veer
		PRIVATE
		vec_tests.cpp
		mat_tests.cpp
		transform_tests.cpp
	)
endif()