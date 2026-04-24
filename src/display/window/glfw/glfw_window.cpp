#include "glfw_window.h"

#include <application/application.h>

#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

namespace veer::display::window
{
#if 0
	void glfw_window::s_glfw_on_frame_buffer_size_changed_callback(GLFWwindow* _window, int _w, int _h)
	{
		windowing_service* sys = application::instance().get_service_locator().get<windowing_service>();

		for (size_t i = 0; i < sys->get_window_count(); ++i )
		{
			window* win = sys->get_window( i );
			glfw_window* glfw_win = static_cast<glfw_window*>(win);

			if (_window == glfw_win->m_api_handle )
			{
				glfw_win->get_on_resize_event().call( vec2u(_w, _h) );
			}
		}
	}
#endif // 0

	// Window
	glfw_window::glfw_window( veer::math::vec2u _size )
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// TODO : handle resizable window
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		GLFWwindow* api_handle = glfwCreateWindow( _size[0], _size[1], "VEER window", nullptr, nullptr);
		m_api_handle = api_handle;

		//glfwSetFramebufferSizeCallback( m_api_handle, s_glfw_on_frame_buffer_size_changed_callback );

		veer::math::vec2i frame_buffer_size;
		glfwGetFramebufferSize(m_api_handle, &frame_buffer_size[0], &frame_buffer_size[1]);

		// TODO : improve vec
		m_window_to_frame_buffer_size_ratio = veer::math::vec2f( _size[0] == 0 ? 1.f : frame_buffer_size[0] / _size[0], _size[1] == 0 ? 1.f :frame_buffer_size[1] / _size[1]);
	}

	bool glfw_window::is_open() const
	{
		return !glfwWindowShouldClose( m_api_handle );
	}

	window::os_window_handle glfw_window::get_os_window_handle()
	{
#if defined( VEER_OS_WINDOWS )
		return glfwGetWin32Window(m_api_handle);
#endif // defined( VEER_OS_WINDOWS )
	}

	void glfw_window::poll_events()
	{
		glfwPollEvents();
	}

	veer::math::vec2u glfw_window::get_size() const
	{
		int w, h;
		glfwGetFramebufferSize(m_api_handle, &w, &h);
		return veer::math::vec2u((unsigned int)std::max(0, w), (unsigned int)std::max(0, h));
	}

	void glfw_window::set_size(veer::math::vec2u _size) &
	{

	}

	glfw_window::~glfw_window()
	{
		glfwDestroyWindow( m_api_handle );
		m_api_handle = nullptr;
	}
}
