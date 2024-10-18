#include "application.h"

#include "Display/rendering_service.h"
#include "Display/windowing_service.h"
#include "Display/swap_chain.h"

#include "Display/glfw/glfw_windowing_service.h"
#include "Display/DX12/dx12_rendering_service.h"

// TODO remove
#include "Display/DX12/dx12_command_buffer.h"
#include "Display/DX12/dx12_swap_chain.h"

#include <chrono>

namespace veer
{
	application::application( const application_config& _config )
	{
		m_application_config = _config;

		switch (_config.m_windowing_service_choice)
		{
			case application_config::windowing_service_type::glfw:
				m_window_service = std::make_unique<glfw_windowing_service>();
				break;
			default:
			{
				VEER_LOG_ERROR("Windowing service backend not set in application_config");
				return;
			}
		}

		switch (_config.m_rendering_service_choice)
		{
			case application_config::rendering_service_type::dx12:
				m_render_service = std::make_unique<dx12_rendering_service>();
				break;
			default:
			{
				VEER_LOG_ERROR("Windowing service backend not set in application_config");
				return;
			}
		}
	}

	application::~application()
	{
		m_window_service.release();
		m_render_service.release();
	}

	const application_config& application::get_config() const
	{
		return m_application_config;
	}

	void on_window_dimensions_changed( vec2u _dims )
	{
		VEER_LOG( "Window dimensions : ( " << _dims[0] << ", " << _dims[1] << ")" );
	}

	int application::run()
	{
		//windowing_service& ws = windowing_service::instance();


		render_device& device = m_render_service->get_render_device();

		const vec2u initial_win_size(640u, 480u);
		window* window = m_window_service->create_window( initial_win_size );
		VEER_ASSERT(window != nullptr, ("Wrong handle to window"));

		vec2u current_win_size = window->get_size();

		std::unique_ptr<swap_chain> swap_chain = device.create_swap_chain( *m_render_service, *window, current_win_size);

		command_queue& graphics_command_queue = device.get_command_queue(command_buffer::type::Graphics);

		uint64_t frame_index = 0;

		uint64_t fence_values = 0;
		uint64_t frame_fence_values[swap_chain::s_swap_chain_buffer_count] = {};
		
		std::chrono::high_resolution_clock clock;
		auto t0_global = clock.now();
		auto t0_frame = t0_global;

		// TODO create event handler
		while(window->is_open()) 
		{
			auto tmp = clock.now();
			auto dt = tmp - t0_frame;
			t0_frame = tmp;

			long dts = std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();
			float fps = dts > 0.f ? 1000 / dts : -1.f;
			if ( !(frame_index & 0xF) ) // once every 16 frames
				VEER_LOG_DEBUG("dt : " << dts << " | fps : " << fps);

			window->poll_events();

			const vec2u this_frame_win_size = window->get_size();
			// TODO : improve vec
			if ( this_frame_win_size[0] != current_win_size[0] || this_frame_win_size[1] != current_win_size[1] )
			{
				// Wait for all previous work
				fence_values++;
				graphics_command_queue.signal(fence_values);
				graphics_command_queue.wait_for_value(fence_values);

				current_win_size = this_frame_win_size;
				swap_chain = nullptr; // destroy old swap_chain
				swap_chain = device.create_swap_chain(*m_render_service, *window, current_win_size);
			}
			
			std::unique_ptr<command_buffer> test_frame_command_buffer = m_render_service->start_recording_command_buffer(command_buffer::type::Graphics);

			ComPtr<ID3D12Resource> current_backbuffer_resource = static_cast<dx12_swap_chain*>(swap_chain.get())->get_backbuffer_resource( frame_index );
			D3D12_CPU_DESCRIPTOR_HANDLE current_backbuffer_cpu_handle = static_cast<dx12_swap_chain*>(swap_chain.get())->get_backbuffer_cpu_handle( frame_index );
			{
				// Transition RT to RenderTarget state (needed for Clear call)
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = current_backbuffer_resource.Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
 
				static_cast<dx12_command_buffer*>(test_frame_command_buffer.get())->get_api_handle()->ResourceBarrier(1, &barrier);

				// Clear RT
				float clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
				static_cast<dx12_command_buffer*>(test_frame_command_buffer.get())->get_api_handle()->ClearRenderTargetView(current_backbuffer_cpu_handle, clearColor, 0, nullptr);
			}

			{
				// Transition RT to Present state (needed for Present call)
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = current_backbuffer_resource.Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				static_cast<dx12_command_buffer*>(test_frame_command_buffer.get())->get_api_handle()->ResourceBarrier(1, &barrier);
			}

			m_render_service->stop_recording_command_buffer( *test_frame_command_buffer );

			command_buffer* test_frame_command_buffer_raw_ptr = test_frame_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::span<command_buffer*>(&test_frame_command_buffer_raw_ptr));

			swap_chain->present( 0 );

			// Signal current back buffer usage is ended
			fence_values++;
			frame_fence_values[frame_index % swap_chain::s_swap_chain_buffer_count] = fence_values;
			graphics_command_queue.signal(fence_values);

			frame_index++;

			// Wait for next frame back buffer end of next frame back buffer
			graphics_command_queue.wait_for_value(frame_fence_values[frame_index % swap_chain::s_swap_chain_buffer_count]);

		}
		return 0;
	}
}
