#include "glfw_windowing_service.h"

#include "glfw_window.h"

#include <memory>
#include <iterator>

namespace veer
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

	window* glfw_windowing_service::create_window( vec2u _dims )
	{
		std::unique_ptr<glfw_window> win_ptr =  std::make_unique<glfw_window>( _dims );
		std::unique_ptr<window>& win_ptr_ref = m_windows.emplace_back( std::move( win_ptr ) );
		return win_ptr_ref.get();
	}

	window* glfw_windowing_service::get_window( size_t _index ) const
	{
		return m_windows[_index].get();
	}

	size_t glfw_windowing_service::get_window_count()
	{
		return m_windows.size();
	}

	void glfw_windowing_service::destroy_window( size_t _index )
	{
		auto it = m_windows.begin();
		std::advance(it, _index);
		// TODO Assert it != end
		m_windows.erase( it );
	}
}
