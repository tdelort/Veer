#pragma once

#include <core/core.h>

#include <display/render/swap_chain.h>
#include <display/render/technique.h>
#include <display/render/shader_source.h>
#include <display/render/command_buffer.h>
#include <display/render/command_queue.h>
#include <memory>

namespace veer::display::window
{
	class window;
}

namespace veer::display::render
{
	class rendering_service;

	class render_device
	{
	public:
		virtual ~render_device() = 0;

		virtual std::unique_ptr<swap_chain> create_swap_chain(veer::display::window::window& _window, veer::math::vec2u _size) = 0;

        virtual std::unique_ptr<graphics_technique> create_graphics_technique(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state) = 0;
        virtual std::unique_ptr<compute_technique> create_compute_technique(const shader_stage_source_container_t& _source_code) = 0;

		virtual command_queue& get_command_queue(command_buffer::type _corresponding_command_buffer_type) = 0;
	};
}