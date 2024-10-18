#ifndef DISPLAY_WINDOWING_SERVICE_H_INCLUDED
#define DISPLAY_WINDOWING_SERVICE_H_INCLUDED

#include "Core/service.h"
#include "Core/Math/vec.h"
#include "Display/window.h"

namespace veer
{
	class windowing_service : public service_interface
	{
	public:
		virtual ~windowing_service() = 0 {};

		virtual window* create_window( vec2u _dims ) = 0;

		virtual window* get_window( size_t _index ) const = 0;
		virtual size_t get_window_count() = 0;

		virtual void destroy_window( size_t _index ) = 0;

	protected:
		std::vector<std::unique_ptr<window>> m_windows;
	};
}

#endif // DISPLAY_WINDOWING_SYSTEM_H_INCLUDED
