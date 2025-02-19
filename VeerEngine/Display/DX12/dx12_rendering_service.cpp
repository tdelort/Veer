#include "dx12_pch.h"
#include "dx12_rendering_service.h"
#include "dx12_render_device.h"

#include <Display/DX12/dx12_command_buffer.h>

using namespace Microsoft::WRL;

namespace veer
{
	dx12_rendering_service::dx12_rendering_service()
	{
		// Device is created :3
		m_device = std::make_unique<dx12_render_device>();

		// TODO : move to rendering_thread api
		{
			ID3D12Device2* device_handle = static_cast<dx12_render_device*>(m_device.get())->get_api_handle();
			for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
			{
				device_handle->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocators[i]));
			}
		}

		m_current_frame_index = 0;
	}


	dx12_rendering_service::~dx12_rendering_service()
	{
		// TODO : CLEANUP !!!
	}


	void dx12_rendering_service::start_frame(uint64_t _frame_index)
	{
		rendering_service::start_frame(_frame_index);

		m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count]->Reset();
	}

	void dx12_rendering_service::end_frame()
	{
		rendering_service::end_frame();
	}

	std::unique_ptr<command_buffer> dx12_rendering_service::start_recording_command_buffer(command_buffer::type _type)
	{
		std::unique_ptr<dx12_command_buffer> command_buffer = std::make_unique<dx12_command_buffer>(_type);

		VEER_ASSERT(m_device != nullptr, "Render device is null while trying to start recording a command buffer");
		command_buffer->open(static_cast<dx12_render_device&>(*m_device), m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count]);

		return command_buffer;
	}

	void dx12_rendering_service::stop_recording_command_buffer(command_buffer& _command_buffer)
	{
		static_cast<dx12_command_buffer&>(_command_buffer).close();
	}
}

