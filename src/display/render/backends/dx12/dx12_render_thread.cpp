#include <display/render/render_thread.h>

#include "dx12_pch.h"

#include <display/render/swap_chain.h>
#include <display/render/command_buffer.h>
#include <display/render/render_device.h>

namespace veer::display::render
{
	render_thread::render_thread(const render_device& _render_device)
		: m_device(_render_device), m_current_frame_index(0u)
	{
		for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
		{
			VEER_LOG("CreateCommandAllocator");
			m_device.get_api_handle()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocators[i]));
		}
	}


	render_thread::~render_thread()
	{
		// controlling cleanup order
		for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
		{
			m_command_allocators[i]->Reset();
			m_command_allocators[i].Reset();
		}
	}


	void render_thread::start_frame(uint64_t _frame_index)
	{
		m_current_frame_index = _frame_index;
		m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count]->Reset();
	}

	void render_thread::end_frame()
	{
	}

	void render_thread::open_command_buffer(command_buffer& _command_buffer)
	{
		_command_buffer.open(m_device, m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count]);
	}

	void render_thread::close_command_buffer(command_buffer& _command_buffer)
	{
		_command_buffer.close();
	}
}

