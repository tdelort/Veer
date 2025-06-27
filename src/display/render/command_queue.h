#ifndef DISPLAY_COMMAND_QUEUE_H_INCLUDED
#define DISPLAY_COMMAND_QUEUE_H_INCLUDED

#include <core/containers/span.h>

#include <display/render/command_buffer.h>

namespace veer
{
	class command_queue
	{
	public:

		command_queue( command_buffer::type _type);

		virtual void execute_command_buffers( span<command_buffer*> _command_buffers ) = 0;

		virtual void signal(uint64_t _value) = 0;
		virtual void wait_for_value(uint64_t _value) = 0;

		virtual ~command_queue() = 0 {};
	protected:
		// Type of command buffer the queue is made to execute
		command_buffer::type m_type;
	};
}

#endif // DISPLAY_COMMAND_QUEUE_H_INCLUDED
