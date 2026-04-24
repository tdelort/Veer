#pragma once

#include <core/core.h>
#include <display/window/windowing_service.h>
#include <display/window/window.h>

struct GLFWwindow;

namespace veer::display::window
{
	class glfw_windowing_service : public service_generic<windowing_service>
	{
	public:
		glfw_windowing_service();
		virtual ~glfw_windowing_service() override;

		window* create_window(veer::math::vec2u _dims) override;

		window* get_window(size_t _index) const override;
		size_t get_window_count() override;

		void destroy_window(size_t _index) override;
	};
}
