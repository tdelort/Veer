#pragma once

#include <core/core.h>
#include <core/math/vec.h>

namespace veer::display::window
{
	class window
	{
	public:
		virtual ~window() = 0;

		virtual bool is_open() const = 0;
		virtual void poll_events() = 0;

		virtual veer::math::vec2u get_size() const = 0;
		virtual void set_size( veer::math::vec2u _size ) & = 0;

#if defined( VEER_OS_WINDOWS )
		using os_window_handle = HWND;
#endif // defined( _PC )

		virtual os_window_handle get_os_window_handle() = 0;
	};
}