#pragma once

#include <core/service.h>
#include <display/render/render_device.h>
#include <display/render/shader_compiler.h>

namespace veer::display::render
{
	class command_buffer;

	class rendering_service : public service_interface
	{
	public:
		virtual ~rendering_service() = 0;
		render_device& get_render_device() const;

		shader_compiler& get_shader_compiler() const; 

		// start rendering_tread api
		// these function will make more sence in a rendering_thread api when going multithreaded
		virtual void start_frame( uint64_t _frame_index );
		virtual void end_frame();

		virtual void open_command_buffer(command_buffer& command_buffer) = 0;
		virtual void close_command_buffer(command_buffer& command_buffer) = 0;
		// end rendering_tread api

	protected:
		std::unique_ptr<render_device> m_device;
		std::unique_ptr<shader_compiler> m_shader_compiler;
		uint64_t m_current_frame_index;
	};
}