#ifndef DISPLAY_WINDOW_H_INCLUDED
#define DISPLAY_WINDOW_H_INCLUDED

#include <core/core.h>
#include <core/callback_set.h>
#include <core/Math/vec.h>

namespace veer
{
	class window
	{
	public:
		//using on_resize_callback_set_type = callback_set<vec2u>;

		virtual ~window() = 0 {};

		virtual bool is_open() const = 0;
		virtual void poll_events() = 0;

		virtual vec2u get_size() const = 0;
		virtual void set_size( vec2u _size ) & = 0;

#if defined( VEER_OS_WINDOWS )
		using os_window_handle = HWND;
#endif // defined( _PC )

		virtual os_window_handle get_os_window_handle() = 0;

		//on_resize_callback_set_type& get_on_resize_event();

	protected:

		//on_resize_callback_set_type m_on_resize;
	};
}

#endif // DISPLAY_WINDOW_H_INCLUDED
