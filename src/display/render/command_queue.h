#pragma once

#include "core/containers/resizable_array.h"
#include <core/containers/span.h>

#include <cstdint>
#include <display/render/command_buffer.h>

namespace veer::display::render
{
	class command_queue
	{
	public:

		command_queue( command_buffer::type _type);

		virtual void execute_command_buffers( veer::containers::span<command_buffer*> _command_buffers );

		virtual void signal(uint64_t _value);
		virtual void wait_for_value(uint64_t _value);

		virtual ~command_queue() = 0;
	protected:

	protected:
		// Type of command buffer the queue is made to execute
		command_buffer::type m_type;
		uint64_t m_last_signaled_fence_value{0u};

	private:
		struct executed_buffer
		{
			uint64_t m_fence_value_execution;
			command_buffer* m_buffer;
		};

		containers::resizable_array<executed_buffer> m_executed_buffers;
	};
}