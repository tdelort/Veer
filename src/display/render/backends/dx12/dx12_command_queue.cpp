#include "dx12_command_queue.h"

#include "display/render/command_queue.h"
#include "dx12_render_device.h"

#include <core/debug.h>
#include <core/core.h>
#include <core/containers/resizable_array.h>


namespace veer::display::render
{
	dx12_command_queue::dx12_command_queue(dx12_render_device& _device, command_buffer::type _type)
		: command_queue(_type)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = command_buffer::s_convert(_type);
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ComPtr<ID3D12Device2> dx12_device = _device.get_api_handle();
		HRESULT hr = dx12_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue_api_handle));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create command queue (" << hr << ")");

		hr = dx12_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create command queue fence (" << hr << ")");
	}

	dx12_command_queue::~dx12_command_queue()
	{
	}

	void dx12_command_queue::execute_command_buffers(veer::containers::span<command_buffer*> _command_buffers)
	{
		veer::containers::resizable_array<ID3D12CommandList*> dx12_command_lists;
		dx12_command_lists.reserve(_command_buffers.size());
		for (command_buffer* command_buffer : _command_buffers)
		{
			dx12_command_lists.push_back(command_buffer->get_api_handle());
		}

		VEER_LOG( "ExecuteCommandLists( " << dx12_command_lists.size() << ",  )" );
		m_command_queue_api_handle->ExecuteCommandLists((UINT)dx12_command_lists.size(), dx12_command_lists.data());

		command_queue::execute_command_buffers(_command_buffers);
	}

	// TODO : . Ping pong between backbuffer index (given as a parameter) for m_fence_value
	void dx12_command_queue::signal(uint64_t _value)
	{
		VEER_ASSERT(_value > m_last_signaled_fence_value, "Trying to signal a fence value (" << _value << ") lower than last signaled value (" << m_last_signaled_fence_value << ")");

		HRESULT hr = m_command_queue_api_handle->Signal(m_fence.Get(), _value);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to signal command queue fence (" << hr << ")");

		command_queue::signal(_value);
	}

	void dx12_command_queue::wait_for_value(uint64_t _value)
	{
		// Wait until the GPU has completed commands up to this fence point.
		if(m_fence->GetCompletedValue() < _value)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);

			// Fire event when GPU hits current fence.  
			HRESULT hr = m_fence->SetEventOnCompletion(_value, event);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to set event on command queue fence (" << hr << ")");

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		command_queue::wait_for_value(_value);
	}

	ComPtr<ID3D12CommandQueue> dx12_command_queue::get_api_handle()
	{
		return m_command_queue_api_handle;
	}

}
