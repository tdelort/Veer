#include "glfw_windowing_service.h"

#include "glfw_window.h"
#include <core/core.h>

namespace veer::display::window
{
	glfw_windowing_service::glfw_windowing_service() 
	{
		glfwInit();
	}

	glfw_windowing_service::~glfw_windowing_service() 
	{
		m_windows.clear();
		glfwTerminate();	
	}

	window& glfw_windowing_service::create_window(veer::math::vec2u _dims)
	{
		unique_ptr<glfw_window> win_ptr = unique_ptr<glfw_window>::make(_dims);
		unique_ptr<window>& win_ptr_ref = m_windows.emplace_back(std::move(win_ptr));
		return *win_ptr_ref;
	}

	window& glfw_windowing_service::get_window(size_t _index) const
	{
		return *m_windows[_index];
	}

	size_t glfw_windowing_service::get_window_count()
	{
		return m_windows.size();
	}

	void glfw_windowing_service::destroy_window(size_t _index)
	{
		auto it = m_windows.begin();
		std::advance(it, _index);
		// TODO Assert it != end
		m_windows.erase(it);
	}
}
