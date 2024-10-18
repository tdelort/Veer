#include "dx12_pch.h"
#include "dx12_command_buffer.h"

#include <Display/DX12/dx12_render_device.h>

namespace veer
{
	D3D12_COMMAND_LIST_TYPE dx12_command_buffer::s_to_dx12_type(const command_buffer::type _type)
	{
		D3D12_COMMAND_LIST_TYPE dx12_type;
		switch (_type)
		{
		case command_buffer::type::Graphics:
			dx12_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case command_buffer::type::Compute:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			break;
		case command_buffer::type::Copy:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		default:
			VEER_ASSERT(false, "All command_buffer::type should be covered");
			break;
		}

		return dx12_type;
	}

	dx12_command_buffer::dx12_command_buffer(command_buffer::type _type)
		: command_buffer( _type )
	{
	}

	void dx12_command_buffer::open( dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator )
	{
		D3D12_COMMAND_LIST_TYPE dx12_type = dx12_command_buffer::s_to_dx12_type(m_type);
		VEER_ASSERT(m_type == command_buffer::type::Graphics, "We don't support other types yet");

		if (m_command_list_handle == nullptr)
		{
			HRESULT hr = _device.get_api_handle()->CreateCommandList(0, dx12_type, _command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_command_list_handle));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list (" << hr << ")");
		}
		else
		{
			m_command_list_handle->Reset(_command_allocator.Get(), nullptr);
		}
	}

	void dx12_command_buffer::close()
	{
		if (m_command_list_handle != nullptr)
		{
			HRESULT hr = m_command_list_handle->Close();
			VEER_ASSERT(SUCCEEDED(hr), "Failed to properly close command list (" << hr << ")");
		}
	}

	ComPtr<ID3D12GraphicsCommandList> dx12_command_buffer::get_api_handle()
	{
		return m_command_list_handle;
	}
}
