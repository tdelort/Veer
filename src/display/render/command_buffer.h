#pragma once

#include "render_device_resource_sync_state.h"

#include <core/math/vec.h>
#include <display/render/technique.h>

namespace veer::display::render
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

		command_buffer(command_buffer::type _type);
		virtual ~command_buffer() = 0;

		// TODO : this is the low level api, I should implement something to handle state tracking and transitions when multi threaded rendering in an higher level class 
		// TODO : impl dx12
		virtual void transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state) = 0;

		// TODO : impl dx12
		virtual void clear_render_target(render_device_resource& _render_target_resource, veer::math::vec4f _color) =0;

		// TODO : impl compute_command_buffer / graphics_command_buffer / copy_command_buffer / etc
		virtual void set_technique(graphics_technique& _technique) = 0;
		virtual void set_technique(compute_technique& _technique) = 0;

	protected:
		type m_type;
	};
}