#include "command_queue.h"
#include "command_buffer.h"

namespace veer::display::render
{
	command_queue::command_queue(command_buffer::type _type)
		: m_type{ _type }
	{

	}

	command_queue::~command_queue()
	{

	}

	void command_queue::execute_command_buffers( veer::containers::span<command_buffer*> _command_buffers )
	{
		for(command_buffer* buffer : _command_buffers )
			m_executed_buffers.emplace_back( m_last_signaled_fence_value, buffer );
	}

	void command_queue::signal(uint64_t _value)
	{
		m_last_signaled_fence_value = _value;
	}

	void command_queue::wait_for_value(uint64_t _value)
	{
		for(containers::resizable_array<executed_buffer>::iterator it = m_executed_buffers.begin(); it != m_executed_buffers.end(); /*left blank intentionally*/)
		{
			if ( it->m_fence_value_execution < _value )
			{
				it->m_buffer->on_execution();
				it = m_executed_buffers.erase(it);
			}
			else 
			{
				++it;
			}
		}
	}
}
