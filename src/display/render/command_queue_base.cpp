#include "command_queue_base.h"

#include <display/render/command_buffer.h>
#include <display/render/submit_context.h>

namespace veer::display::render
{
	command_queue_base::~command_queue_base()
	{

	}
	
	void command_queue_base::enqueue(command_buffer&& _command_buffer)
	{
		m_executed_buffers.emplace_back(m_last_signaled_fence_value, std::forward<command_buffer&&>(_command_buffer));
	}

	
	void command_queue_base::signal(uint64_t _value)
	{
		VEER_ASSERT(_value > m_last_signaled_fence_value, "Trying to signal a fence value (" << _value << ") lower than last signaled value (" << m_last_signaled_fence_value << ")");

		m_last_signaled_fence_value = _value;
	}

	
	void command_queue_base::wait_for_value(uint64_t _value)
	{
		for(containers::resizable_array<executed_buffer>::iterator it = m_executed_buffers.begin(); it != m_executed_buffers.end(); /*left blank intentionally*/)
		{
			if (it->m_fence_value_execution < _value)
			{
				it->m_buffer.on_execution();
				it = m_executed_buffers.erase(it);
			}
			else 
			{
				++it;
			}
		}
	}
}
