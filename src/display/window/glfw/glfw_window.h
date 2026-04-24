#pragma once

// GLFW Headers
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <core/singleton.h>
#include <core/math/vec.h>
#include <display/window/windowing_service.h>
#include <display/window/window.h>

struct GLFWwindow;

namespace veer::display::window
{
	// TODO : most glfw function must be called from the main thread => no thread should ever have access to a window
	class glfw_window : public window
	{
	public:
		glfw_window( veer::math::vec2u _size );
		~glfw_window();

		bool is_open() const override;
		void poll_events() override;

		veer::math::vec2u get_size() const override;
		void set_size( veer::math::vec2u _size ) & override;

		os_window_handle get_os_window_handle() override;
	private:

		// here since it will handle finding the window from the api handle
		static void s_glfw_on_frame_buffer_size_changed_callback(GLFWwindow* _window, int _w, int _h);

		GLFWwindow* m_api_handle;

		veer::math::vec2f m_window_to_frame_buffer_size_ratio{ 1.f, 1.f };
	};
}