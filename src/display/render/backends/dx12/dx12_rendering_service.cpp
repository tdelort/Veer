#include "dx12_pch.h"
#include "dx12_rendering_service.h"
#include "dx12_render_device.h"
#include "dx12_shader_compiler.h"

using namespace Microsoft::WRL;

namespace veer::display::render
{
	dx12_rendering_service::dx12_rendering_service()
	{
		// Device is created :3
		std::unique_ptr<dx12_render_device> device = std::make_unique<dx12_render_device>();

		// TODO : move to rendering_thread api
		{
			ComPtr<ID3D12Device2> device_handle = device->get_api_handle();
			for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
			{
				device_handle->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocators[i]));
			}
		}
		m_device = std::move( device );

		std::unique_ptr<dx12_shader_compiler> shader_compiler = std::make_unique<dx12_shader_compiler>();
		m_shader_compiler = std::move( shader_compiler );


		m_current_frame_index = 0;
	}


	dx12_rendering_service::~dx12_rendering_service()
	{
		// controlling cleanup order
		for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
		{
			m_command_allocators[i]->Reset();
			m_command_allocators[i].Reset();
		}

		m_device.reset();
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

	void dx12_rendering_service::open_command_buffer(command_buffer& command_buffer)
	{
		command_buffer.open(static_cast<dx12_render_device&>(get_render_device()), m_command_allocators[m_current_frame_index % swap_chain::s_swap_chain_buffer_count]);
	}

	void dx12_rendering_service::close_command_buffer(command_buffer& command_buffer)
	{
		command_buffer.close();
	}
}

