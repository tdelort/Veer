#ifndef DISPLAY_RENDER_DEVICE_H_INCLUDED
#define DISPLAY_RENDER_DEVICE_H_INCLUDED

#include <core/core.h>

#include <display/render/swap_chain.h>
#include <display/render/command_buffer.h>
#include <display/render/command_queue.h>

namespace veer
{
	class rendering_service;

	class render_device
	{
	public:
		virtual std::unique_ptr<swap_chain> create_swap_chain(window& _window, vec2u _size) = 0;
		virtual void delete_swap_chain(std::unique_ptr<swap_chain> _swap_chain) = 0;

		virtual command_queue& get_command_queue(command_buffer::type _corresponding_command_buffer_type) = 0;
		virtual ~render_device() = 0 {};
	};
}

#endif // DISPLAY_RENDER_DEVICE_H_INCLUDED
