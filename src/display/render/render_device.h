#pragma once

#include <core/core.h>

#include <display/render/command_buffer.h>
#include <display/render/shader_source.h>

namespace veer::display::window
{
	class window;
}

namespace veer::display::render
{
	class command_queue;
	class swap_chain;
	class graphics_technique;
	class compute_technique;
	class render_device_buffer;
	class buffer_desc;

	class render_device
	{
	public:
		virtual ~render_device() = 0;

		template<typename T, typename... ARGS> 
		std::unique_ptr<T> alloc(ARGS&&... _args) { return alloc_internal(std::forward<ARGS&&>(_args)...); }

		virtual command_queue& get_command_queue(command_buffer::type _corresponding_command_buffer_type) = 0;

	protected:
		// alloc only viable candidates
		virtual std::unique_ptr<swap_chain> alloc_internal(veer::display::window::window& _window, veer::math::vec2u _size) = 0;
        virtual std::unique_ptr<graphics_technique> alloc_internal(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state) = 0;
        virtual std::unique_ptr<compute_technique> alloc_internal(const shader_stage_source_container_t& _source_code) = 0;
	};
}