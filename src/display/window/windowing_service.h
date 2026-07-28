#pragma once

#include "window.h"

#include <core/service.h>
#include <core/math/vec.h>
#include <core/containers/resizable_array.h>

namespace veer::display::window
{
	class windowing_service : public service_interface
	{
	public:
		virtual ~windowing_service() = 0;

		virtual window& create_window( veer::math::vec2u _dims ) = 0;

		virtual window& get_window( size_t _index ) const = 0;
		virtual size_t get_window_count() = 0;

		virtual void destroy_window( size_t _index ) = 0;

	protected:
		veer::containers::resizable_array<std::unique_ptr<window>> m_windows;
	};
}
