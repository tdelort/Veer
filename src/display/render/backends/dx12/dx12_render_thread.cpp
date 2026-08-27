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
		get_current_command_allocator()->Reset();
	}

	void render_thread::end_frame()
	{
	}

	render_thread::command_list_handle_pool_t& render_thread::get_command_list_handle_pool(D3D12_COMMAND_LIST_TYPE _type)
	{
		switch (_type) 
		{
			case D3D12_COMMAND_LIST_TYPE_DIRECT:
				return m_graphics_command_list_handles;
			case D3D12_COMMAND_LIST_TYPE_COMPUTE:
				return m_compute_command_list_handles;
			case D3D12_COMMAND_LIST_TYPE_COPY:
				return m_copy_command_list_handles;
			default:
				break;
        }

		VEER_LOG_ERROR("Unsupported type of command list");
		return m_graphics_command_list_handles;
	}

	ID3D12GraphicsCommandList* render_thread::alloc_api_command_list(D3D12_COMMAND_LIST_TYPE _type)
	{
		command_list_handle_pool_t& pool = get_command_list_handle_pool(_type);

		ID3D12GraphicsCommandList* command_list = nullptr;
		ID3D12CommandAllocator* command_allocator = get_current_command_allocator();
		if (pool.empty())
		{
			const HRESULT hr = m_device.get_api_handle()->CreateCommandList(0, _type, command_allocator, nullptr, IID_PPV_ARGS(&command_list));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list (" << hr << ")");
		}
		else
		{
			command_list = static_cast<ID3D12GraphicsCommandList*>(pool.back());
			pool.pop_back();
			command_list->Reset(command_allocator, nullptr);
		}

		return command_list;
	}

	void render_thread::free_api_command_list(ID3D12GraphicsCommandList* _ptr)
	{
		VEER_ASSERT(_ptr != nullptr, "nullptr argument");

		command_list_handle_pool_t& pool = get_command_list_handle_pool(_ptr->GetType());
		pool.push_back(_ptr);
	}

	ID3D12CommandAllocator* render_thread::get_current_command_allocator()
	{
		return m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count].Get();
	}
}

