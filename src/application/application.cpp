#include "application.h"
#include "application_config.h"
#include "display/render/backends/dx12/dx12_render_device_resource.h"
#include "display/render/technique.h"
#include "display/render/shader_source.h"

#include <d3d12.h>
#include <display/render/rendering_service.h>
#include <display/window/windowing_service.h>

// TODO remove
#include <display/render/swap_chain.h>

// TODO remove
#include <display/window/glfw/glfw_windowing_service.h>
#include <display/render/backends/dx12/dx12_rendering_service.h>
#include <display/render/backends/dx12/dx12_render_device.h>
#include <display/render/backends/dx12/dx12_command_buffer.h>
#include <D3D12MemAlloc.h>

#include <dxgiformat.h>
#include <memory>
#include <stdlib.h>

#include <core/core.h>

// TODO remove
#include <comdef.h>

namespace veer
{
	application::application( const application_config& _config )
	{
		m_application_config = _config;

		switch (_config.m_windowing_service_choice)
		{
			case application_config::windowing_service_type::glfw:
				m_window_service = std::make_unique<veer::display::window::glfw_windowing_service>();
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
				m_render_service = std::make_unique<veer::display::render::dx12_rendering_service>();
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
		// controlling deletion order
		m_window_service.reset();
		m_render_service.reset();
	}

	const application_config& application::get_config() const
	{
		return m_application_config;
	}

	void on_window_dimensions_changed( veer::math::vec2u _dims )
	{
		VEER_LOG( "Window dimensions : ( " << _dims[0] << ", " << _dims[1] << ")" );
	}




	int application::run()
	{
		#if defined(_DEBUG)
		VEER_LOG( "_DEBUG is defined" );
		OutputDebugString( "Test debug string ");
		#endif // defined(_DEBUG)
		//windowing_service& ws = windowing_service::instance();

		veer::display::render::render_device& device = m_render_service->get_render_device();

		const veer::math::vec2u initial_win_size(640u, 480u);
		veer::display::window::window* window = m_window_service->create_window(initial_win_size);
		VEER_ASSERT(window != nullptr, ("Wrong handle to window"));


		veer::math::vec2u current_win_size = window->get_size();

		std::unique_ptr<veer::display::render::swap_chain> swap_chain = device.create_swap_chain(*window, current_win_size);

		veer::display::render::command_queue& graphics_command_queue = device.get_command_queue(veer::display::render::command_buffer::type::Graphics);

		uint64_t fence_values = 0;
		uint64_t frame_index = 0;
		size_t backbuffer_index = swap_chain->get_backbuffer_index();
		uint64_t backbuffers_fence_values[veer::display::render::swap_chain::s_swap_chain_buffer_count] = {};

		{
			swap_chain->present( 0 );

			// Signal current back buffer usage is ended
			fence_values++;
			backbuffers_fence_values[backbuffer_index] = fence_values;
			graphics_command_queue.signal(fence_values);
		}

		// TODO : abstractize

		std::unique_ptr<veer::display::render::graphics_technique> fullscreen_uv_technique = nullptr;

		{
			static constexpr const char* s_shader_source = R"(
				struct VsInput
				{
					float3 pos : POSITION;
					float2 uv : TEXCOORD;
				};
				struct VsOutput
				{
					float4 pos : SV_POSITION;
					float2 uv : TEXCOORD;
				};

				VsOutput mainVS( VsInput input )
				{
					VsOutput output;
					output.pos = float4( input.pos.xyz, 1.f );
					output.uv = input.uv;
					return output;
				}

				float4 mainPS( VsOutput input ) : SV_TARGET
				{
					const float4 color = float4( input.uv, 0.f, 1.f );
					return color;
				}
			)";
			static constexpr size_t s_shader_source_len = std::char_traits<char>::length(s_shader_source);

			static constexpr veer::containers::span<const char> s_shader_source_span( s_shader_source, s_shader_source_len );


			// source code only needs to live up until PSO creation 
			veer::display::render::shader_stage_source_container_t shader_source_code;

			// shader pre-compilation
			{
				veer::display::render::shader_compiler& shader_compiler = m_render_service->get_shader_compiler();

				{
					veer::display::render::shader_code_memory_blob_t& vs_source_code = shader_source_code[static_cast<size_t>(veer::display::render::shader_stage_type::vertex)];
					vs_source_code = shader_compiler.compile( s_shader_source_span, "mainVS", "TestFullscreenQuad", veer::display::render::shader_stage_type::vertex );
				}
				
				{
					veer::display::render::shader_code_memory_blob_t& ps_source_code = shader_source_code[static_cast<size_t>(veer::display::render::shader_stage_type::pixel)];
					ps_source_code = shader_compiler.compile( s_shader_source_span, "mainPS", "TestFullscreenQuad", veer::display::render::shader_stage_type::pixel );
				}
			}


			veer::display::render::shader_signature::input_elem position("POSITION", 0, veer::display::render::render_device_data_format::r32g32b32_float);
			veer::display::render::shader_signature::input_elem texcoord("TEXCOORD", 0, veer::display::render::render_device_data_format::r32g32_float);
			veer::containers::static_array<veer::display::render::shader_signature::input_elem, 2> shader_input = { position, texcoord };

			veer::containers::static_array<veer::display::render::shader_signature::output_elem, 1> shader_output = 
			{
				{ veer::display::render::render_device_data_format::r8g8b8a8_unorm }
			};

			veer::display::render::blend_state shader_blend_state = {}; // default 
			veer::display::render::rasterizer_state rasterizer_state = {}; // default ? 

			fullscreen_uv_technique = device.create_graphics_technique( shader_source_code, { shader_input, shader_output }, { { shader_blend_state }, rasterizer_state } );
		}

		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
		{
			// 	vec3f m_pos;
			// 	vec2f m_uv;
			static const size_t vertex_size = 5u;
			static const size_t vertex_count = 4u;
			static const size_t vertex_buffer_size = vertex_count * vertex_size;
			static const size_t vertex_buffer_size_in_bytes = vertex_buffer_size * sizeof(float);

			static const float vertex_buffer[vertex_buffer_size] =
			{
				-1.f, -1.f, 0.5f,   0.f, 0.f,
				-1.f, 1.f, 0.5f,   0.f, 1.f,
				1.f, 1.f, 0.5f,   1.f, 1.f,
				1.f, -1.f, 0.5f,   1.f, 0.f,
			};


			static const size_t index_buffer_size = 6;
			static const size_t index_buffer_size_in_bytes = index_buffer_size * sizeof(unsigned int);

			static const unsigned int index_buffer[index_buffer_size] =
			{
				0, 1, 2,
				2, 3, 0
			};

			veer::display::render::dx12_render_device& dx12_device = static_cast<veer::display::render::dx12_render_device&>(device);

			D3D12_RESOURCE_DESC vb_desc = {};
			vb_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			vb_desc.Alignment = 0;
			vb_desc.Width = vertex_buffer_size_in_bytes;
			vb_desc.Height = 1;
			vb_desc.DepthOrArraySize = 1;
			vb_desc.MipLevels = 1;
			vb_desc.Format = DXGI_FORMAT_UNKNOWN;
			vb_desc.SampleDesc.Count = 1;
			vb_desc.SampleDesc.Quality = 0;
			vb_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			vb_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_RESOURCE_DESC ib_desc = {};
			ib_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			ib_desc.Alignment = 0;
			ib_desc.Width = index_buffer_size_in_bytes;
			ib_desc.Height = 1;
			ib_desc.DepthOrArraySize = 1;
			ib_desc.MipLevels = 1;
			ib_desc.Format = DXGI_FORMAT_UNKNOWN;
			ib_desc.SampleDesc.Count = 1;
			ib_desc.SampleDesc.Quality = 0;
			ib_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			ib_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			 

			D3D12MA::ALLOCATION_DESC default_alloc_desc = {};
			default_alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

			D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
			upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
			 

			HRESULT hr;
			D3D12MA::Allocation* vb_resource_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &vb_desc, D3D12_RESOURCE_STATE_COMMON, NULL, &vb_resource_alloc, IID_NULL, NULL);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer resource. Error (" << hr << ")");

			D3D12MA::Allocation* vb_upload_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &upload_alloc_desc, &vb_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &vb_upload_alloc, IID_NULL, NULL);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer upload resource. Error (" << hr << ")");


			D3D12MA::Allocation* ib_resource_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &ib_desc, D3D12_RESOURCE_STATE_COMMON, NULL, &ib_resource_alloc, IID_NULL, NULL);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create index buffer resource. Error (" << hr << ")");

			D3D12MA::Allocation* ib_upload_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &upload_alloc_desc, &ib_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &ib_upload_alloc, IID_NULL, NULL);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create index buffer upload resource. Error (" << hr << ")");

			uint8_t* data_vertex;
			vb_upload_alloc->GetResource()->Map(0u, nullptr, reinterpret_cast<void**>(&data_vertex));
			memcpy(data_vertex, vertex_buffer, vertex_buffer_size_in_bytes);
			vb_upload_alloc->GetResource()->Unmap(0u, nullptr);

			uint8_t* data_index;
			ib_upload_alloc->GetResource()->Map(0u, nullptr, reinterpret_cast<void**>(&data_index));
			memcpy(data_index, index_buffer, index_buffer_size_in_bytes);
			ib_upload_alloc->GetResource()->Unmap(0u, nullptr);

			// now, copy from upload heap to gpu resource
			std::unique_ptr<veer::display::render::command_buffer> upload_command_buffer = m_render_service->start_recording_command_buffer(veer::display::render::command_buffer::type::Graphics);
			veer::display::render::dx12_command_buffer* dx12_upload_command_buffer = static_cast<veer::display::render::dx12_command_buffer*>(upload_command_buffer.get());

			{
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = vb_resource_alloc->GetResource();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				dx12_upload_command_buffer->get_api_handle()->ResourceBarrier(1, &barrier);
			}

			{
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = ib_resource_alloc->GetResource();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				dx12_upload_command_buffer->get_api_handle()->ResourceBarrier(1, &barrier);
			}
			
			dx12_upload_command_buffer->get_api_handle()->CopyResource(vb_resource_alloc->GetResource(), vb_upload_alloc->GetResource());
			dx12_upload_command_buffer->get_api_handle()->CopyResource(ib_resource_alloc->GetResource(), ib_upload_alloc->GetResource());

			{
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = vb_resource_alloc->GetResource();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				dx12_upload_command_buffer->get_api_handle()->ResourceBarrier(1, &barrier);
			}

			{
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Transition.pResource = ib_resource_alloc->GetResource();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				dx12_upload_command_buffer->get_api_handle()->ResourceBarrier(1, &barrier);
			}

			m_render_service->stop_recording_command_buffer( *upload_command_buffer );

			veer::display::render::command_buffer* upload_command_buffer_raw_ptr = upload_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::containers::span<veer::display::render::command_buffer*>( upload_command_buffer_raw_ptr ));

			fence_values++;
			graphics_command_queue.signal(fence_values);
			graphics_command_queue.wait_for_value(fence_values);

			// clear upload resources
			ib_upload_alloc->Release();
			vb_upload_alloc->Release();

			index_buffer_view.BufferLocation = ib_resource_alloc->GetResource()->GetGPUVirtualAddress();
			index_buffer_view.Format = DXGI_FORMAT_R32_UINT; // 32-bit unsigned integer (this is what a dword is, double word, a word is 2 bytes)
			index_buffer_view.SizeInBytes = index_buffer_size_in_bytes;

			vertex_buffer_view.BufferLocation = vb_resource_alloc->GetResource()->GetGPUVirtualAddress();
			vertex_buffer_view.StrideInBytes = sizeof( float ) * vertex_size; 
			vertex_buffer_view.SizeInBytes = vertex_buffer_size_in_bytes;
		//#else // 0
		//	index_buffer_view.BufferLocation = ib_upload_alloc->GetResource()->GetGPUVirtualAddress();
		//	index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		//	index_buffer_view.SizeInBytes = index_buffer_size_in_bytes;

		//	vertex_buffer_view.BufferLocation = vb_upload_alloc->GetResource()->GetGPUVirtualAddress();
		//	vertex_buffer_view.StrideInBytes = /*sizeof( float ) * */ 4 * vertex_size; 
		//	vertex_buffer_view.SizeInBytes = vertex_buffer_size_in_bytes;
		//#endif // 0
		}
		
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

			const veer::math::vec2u this_frame_win_size = window->get_size();
			// TODO : improve vec
			if ( this_frame_win_size[0] != current_win_size[0] || this_frame_win_size[1] != current_win_size[1] )
			{
				// Wait for all previous work
				fence_values++;
				graphics_command_queue.signal(fence_values);
				graphics_command_queue.wait_for_value(fence_values);

				current_win_size = this_frame_win_size;
				swap_chain = nullptr; // destroy old swap_chain
				swap_chain = device.create_swap_chain( *window, current_win_size);

				for (size_t i = 0; i < veer::display::render::swap_chain::s_swap_chain_buffer_count; ++i)
				{
					backbuffers_fence_values[i] = backbuffers_fence_values[backbuffer_index];
				}
			}
			
			std::unique_ptr<veer::display::render::command_buffer> test_frame_command_buffer = m_render_service->start_recording_command_buffer(veer::display::render::command_buffer::type::Graphics);

			veer::display::render::render_device_resource& backbuffer_resource = swap_chain->get_current_backbuffer();
			{
				// Transition RT to RenderTarget state (needed for Clear call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, veer::display::render::render_device_resource_sync_state::Present, veer::display::render::render_device_resource_sync_state::RenderTarget);
 

				// Clear RT
				veer::math::vec4f clear_color( 0.4f, 0.6f, 0.9f, 1.0f );
				// test_frame_command_buffer->clear_render_target(backbuffer_resource, clear_color);

				// Draw !!

				veer::display::render::dx12_command_buffer* dx12_test_frame_command_buffer = static_cast<veer::display::render::dx12_command_buffer*>(test_frame_command_buffer.get());

				test_frame_command_buffer->set_technique(*fullscreen_uv_technique);


				const veer::display::render::dx12_descriptor& backbuffer_cpu_handle = static_resource_cast<veer::display::render::dx12_render_device_resource>( backbuffer_resource ).get_cpu_handle();
				dx12_test_frame_command_buffer->get_api_handle()->OMSetRenderTargets(1, &backbuffer_cpu_handle.m_handle, FALSE, nullptr);

				// Record commands.
				dx12_test_frame_command_buffer->get_api_handle()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				dx12_test_frame_command_buffer->get_api_handle()->IASetVertexBuffers(0, 1, &vertex_buffer_view);
				dx12_test_frame_command_buffer->get_api_handle()->IASetIndexBuffer( &index_buffer_view );

				{
					// draw function
					D3D12_VIEWPORT viewport;
					viewport.TopLeftX = 0;
					viewport.TopLeftY = 0;
					viewport.Width = current_win_size[0];
					viewport.Height = current_win_size[1];
					viewport.MinDepth = 0.0f;
					viewport.MaxDepth = 1.0f;

					D3D12_RECT scissor_rect;
					scissor_rect.left = 0;
					scissor_rect.top = 0;
					scissor_rect.right = current_win_size[0];
					scissor_rect.bottom = current_win_size[1];
					
					dx12_test_frame_command_buffer->get_api_handle()->RSSetViewports(1, &viewport);
					dx12_test_frame_command_buffer->get_api_handle()->RSSetScissorRects(1, &scissor_rect);
				
					dx12_test_frame_command_buffer->get_api_handle()->DrawIndexedInstanced( 6, 1, 0, 0, 0 );
				}
			}

			{
				// Transition RT to Present state (needed for Present call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, veer::display::render::render_device_resource_sync_state::RenderTarget, veer::display::render::render_device_resource_sync_state::Present);
			}

			m_render_service->stop_recording_command_buffer( *test_frame_command_buffer );

			veer::display::render::command_buffer* test_frame_command_buffer_raw_ptr = test_frame_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::containers::span<veer::display::render::command_buffer*>(test_frame_command_buffer_raw_ptr));

			{
				swap_chain->present( 0 );

				// Signal current back buffer usage is ended
				fence_values++;
				backbuffers_fence_values[backbuffer_index] = fence_values;
				graphics_command_queue.signal(fence_values);
			}

			frame_index++;
			backbuffer_index = swap_chain->get_backbuffer_index();

			// Wait for next frame back buffer end of next frame back buffer
			graphics_command_queue.wait_for_value(backbuffers_fence_values[backbuffer_index]);
		}

		// Wait for all previous work
		fence_values++;
		graphics_command_queue.signal(fence_values);
		graphics_command_queue.wait_for_value(fence_values);

		swap_chain = nullptr;

		return 0;
	}
}
