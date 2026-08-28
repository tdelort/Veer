#include <display/render/render_thread.h>

#include "dx12_pch.h"

#include <display/render/swap_chain.h>
#include <display/render/command_buffer.h>
#include <display/render/render_device.h>

namespace veer::display::render
{
	ID3D12CommandAllocator* s_alloc_command_allocator(const render_device& _device, D3D12_COMMAND_LIST_TYPE _type)
	{
		ID3D12CommandAllocator* allocator = nullptr;
		const HRESULT hr = _device.get_api_handle()->CreateCommandAllocator(_type, IID_PPV_ARGS(&allocator));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list allocator (" << hr << ")");
		return allocator;
	}

	render_thread::render_thread(const render_device& _render_device)
		: m_device(_render_device), m_current_frame_index(0u)
	{
		for (command_list_allocators_t& allocators : m_command_list_allocators)
		{
			allocators.m_graphics = s_alloc_command_allocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
			allocators.m_compute = s_alloc_command_allocator(m_device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
			allocators.m_copy = s_alloc_command_allocator(m_device, D3D12_COMMAND_LIST_TYPE_COPY);
		}
	}


	render_thread::~render_thread()
	{
		// controlling cleanup order
		for (command_list_allocators_t& allocators : m_command_list_allocators)
		{
			allocators.m_graphics->Reset();
			allocators.m_compute->Reset();
			allocators.m_copy->Reset();

			allocators.m_graphics->Release();
			allocators.m_compute->Release();
			allocators.m_copy->Release();
		}
	}


	void render_thread::start_frame(uint64_t _frame_index)
	{
		m_current_frame_index = _frame_index;

		command_list_allocators_t& allocators = get_current_command_allocator();
		allocators.m_graphics->Reset();
		allocators.m_compute->Reset();
		allocators.m_copy->Reset();
	}

	void render_thread::end_frame()
	{
	}

	ID3D12GraphicsCommandList* render_thread::alloc_api_command_list(D3D12_COMMAND_LIST_TYPE _type)
	{
		command_list_handle_pool_t* pool = m_command_list_pools.get_from_type(_type);
		VEER_ASSERT(pool != nullptr, "Type " << _type << " of command list not supported");

		command_list_allocators_t& command_allocators = get_current_command_allocator();
		ID3D12CommandAllocator* command_allocator = *command_allocators.get_from_type(_type);

		ID3D12GraphicsCommandList* command_list = nullptr;
		if (pool->empty())
		{
			const HRESULT hr = m_device.get_api_handle()->CreateCommandList(0, _type, command_allocator, nullptr, IID_PPV_ARGS(&command_list));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list (" << hr << ")");
		}
		else
		{
			command_list = pool->back();
			pool->pop_back();
			command_list->Reset(command_allocator, nullptr);
		}

		return command_list;
	}

	void render_thread::free_api_command_list(ID3D12GraphicsCommandList* _ptr)
	{
		VEER_ASSERT(_ptr != nullptr, "nullptr argument");

		D3D12_COMMAND_LIST_TYPE type = _ptr->GetType();
		command_list_handle_pool_t* pool = m_command_list_pools.get_from_type(type);
		VEER_ASSERT(pool != nullptr, "Type " << type << " of command list not supported");

		pool->push_back(_ptr);
	}

	render_thread::command_list_allocators_t& render_thread::get_current_command_allocator()
	{
		return m_command_list_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count];
	}
}

