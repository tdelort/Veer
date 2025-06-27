#ifndef DISPLAY_COMMAND_BUFFER_H_INCLUDED
#define DISPLAY_COMMAND_BUFFER_H_INCLUDED

#include "resource_synchronisation.h"

#include <core/math/vec.h>

namespace veer
{
	class render_device_resource;

	class command_buffer
	{
	public:
		enum class type
		{
			Graphics,
			Compute,
			Copy,
		};

		command_buffer( command_buffer::type _type);

		// TODO : this is the low level api, I should implement something to handle state tracking and transitions when multi threaded rendering in an higher level class 
		// TODO : impl dx12
		virtual void transition_barrier(render_device_resource* _resource, resource_sync_state _from_state, resource_sync_state _to_state) = 0;

		// TODO : impl dx12
		virtual void clear_render_target(render_device_resource* _render_target_resource, vec4f _color) =0;

		virtual ~command_buffer() = 0 {};
	protected:
		type m_type;
	};
}

#endif // DISPLAY_COMMAND_BUFFER_H_INCLUDED
