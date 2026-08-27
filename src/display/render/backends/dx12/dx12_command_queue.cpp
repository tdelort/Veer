#include <display/render/command_queue.h>

#include <core/debug.h>
#include <core/core.h>
#include <core/containers/resizable_array.h>

#include <display/render/render_device.h>


namespace veer::display::render
{

	command_queue::command_queue(render_device& _device, command_buffer::type _type)
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = command_buffer::s_convert(_type);
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		HRESULT hr = _device.get_api_handle()->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue_api_handle));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create command queue (" << hr << ")");

		hr = _device.get_api_handle()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create command queue fence (" << hr << ")");
	}

	command_queue::~command_queue()
	{
		VEER_ASSERT(m_queued_command_lists.empty(), "Some command lists were queued but not flushed");
		// m_fence and m_command_queue_api_handle are ComPtr
	}

	void command_queue::enqueue(command_buffer&& _command_buffer)
	{
		ID3D12GraphicsCommandList* command_list = _command_buffer.release_handle();
		VEER_ASSERT(command_list != nullptr, "command_buffer object has already been executed");
		const HRESULT hr = command_list->Close();
		VEER_ASSERT(SUCCEEDED(hr), "Failed to properly close command list (" << hr << ")");

		m_queued_command_lists.push_back(command_list);

		command_queue_base::enqueue(std::forward<command_buffer&&>(_command_buffer));
	}

	// TODO : . Ping pong between backbuffer index (given as a parameter) for m_fence_value
	void command_queue::signal(uint64_t _value)
	{
		command_queue_base::signal(_value);

		HRESULT hr = m_command_queue_api_handle->Signal(m_fence.Get(), _value);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to signal command queue fence (" << hr << ")");
	}

	void command_queue::wait_for_value(uint64_t _value)
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

		command_queue_base::wait_for_value(_value);
	}

	ComPtr<ID3D12CommandQueue> command_queue::get_api_handle()
	{
		return m_command_queue_api_handle;
	}

	void command_queue::flush()
	{
		m_command_queue_api_handle->ExecuteCommandLists((UINT)m_queued_command_lists.size(), m_queued_command_lists.data());
		m_queued_command_lists.clear();
	}




	copy_command_queue::copy_command_queue(render_device& _device)
		: command_queue(_device, command_buffer::type::copy)
	{

	}

	copy_command_queue::copy_command_queue(render_device& _device, command_buffer::type _type)
		: command_queue(_device, _type)
	{

	}

	copy_command_queue::~copy_command_queue()
	{

	}


	compute_command_queue::compute_command_queue(render_device& _device)
		: copy_command_queue(_device, command_buffer::type::compute)
	{

	}

	compute_command_queue::compute_command_queue(render_device& _device, command_buffer::type _type)
		: copy_command_queue(_device, _type)
	{

	}

	compute_command_queue::~compute_command_queue()
	{

	}


	graphics_command_queue::graphics_command_queue(render_device& _device)
		: compute_command_queue(_device, command_buffer::type::graphics)
	{

	}

	graphics_command_queue::~graphics_command_queue()
	{

	}
}
