#include "application.h"
#include "application_config.h"

#include <core/core.h>
#include <core/containers/span.h>
#include <core/containers/static_array.h>
#include <core/math/vec.h>

#include <display/render/base_types.h>
#include <display/render/render_device_buffer.h>
#include <display/render/render_device_texture_2d.h>
#include <display/render/technique.h>
#include <display/render/shader_source.h>
#include <display/render/rendering_service.h>
#include <display/render/swap_chain.h>
#include <display/render/command_queue.h>
#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_rendering_service.h>
#include <display/render/backends/dx12/dx12_render_device.h>
#endif // defined(D3D12_RENDER_BACKEND)

#include <display/window/windowing_service.h>
//#if defined(GLFW_WINDOW_BACKEND)
#include <display/window/glfw/glfw_windowing_service.h>
//#endif // defined(GLFW_WINDOW_BACKEND)

#include <memory>
#include <stdlib.h>

namespace veer
{
	application::application( const application_config& _config )
	{
		m_application_config = _config;
		m_window_service = std::make_unique<veer::display::window::glfw_windowing_service>();

#if defined(D3D12_RENDER_BACKEND)
		m_render_service = std::make_unique<veer::display::render::dx12_rendering_service>();
#endif // defined(D3D12_RENDER_BACKEND)
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

		std::unique_ptr<veer::display::render::swap_chain> swap_chain = device.alloc<veer::display::render::swap_chain>(*window, current_win_size);

		veer::display::render::command_queue& graphics_command_queue = device.get_command_queue(veer::display::render::command_buffer::type::graphics);

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


			veer::display::render::shader_signature::input_elem position("POSITION", 0, veer::display::render::render_device_data_format::r32g32b32_float, 0u);
			veer::display::render::shader_signature::input_elem texcoord("TEXCOORD", 0, veer::display::render::render_device_data_format::r32g32_float, 12u);
			veer::containers::static_array<veer::display::render::shader_signature::input_elem, 2> shader_input = { position, texcoord };

			veer::containers::static_array<veer::display::render::shader_signature::output_elem, 1> shader_output = 
			{
				{ veer::display::render::render_device_data_format::r8g8b8a8_unorm }
			};

			veer::display::render::shader_signature shader_signature(shader_input, shader_output);

			veer::display::render::blend_state shader_blend_state = {}; // default 
			veer::display::render::rasterizer_state rasterizer_state = {}; // default ? 
			veer::display::render::shader_render_state shader_render_state({ shader_blend_state }, rasterizer_state);

			fullscreen_uv_technique = device.alloc<veer::display::render::graphics_technique>(shader_source_code, shader_signature, shader_render_state);
		}



		static const size_t index_buffer_size = 6;
		static const size_t index_buffer_size_in_bytes = index_buffer_size * sizeof(uint32_t);
		std::unique_ptr<veer::display::render::render_device_buffer> index_buffer = nullptr; 

		static const size_t vertex_size = 5u;
		static const size_t vertex_count = 4u;
		static const size_t vertex_buffer_size = vertex_count * vertex_size;
		static const size_t vertex_buffer_size_in_bytes = vertex_buffer_size * sizeof(float);
		std::unique_ptr<veer::display::render::render_device_buffer> vertex_buffer = nullptr;
		{
			static containers::static_array<float, vertex_buffer_size> vertex_buffer_data =
			{
				-1.f, -1.f, 0.5f,   0.f, 0.f,
				-1.f,  1.f, 0.5f,   0.f, 1.f,
				 1.f,  1.f, 0.5f,   1.f, 1.f,
				 1.f, -1.f, 0.5f,   1.f, 0.f,
			};

			static containers::static_array<uint32_t, index_buffer_size> index_buffer_data =
			{
				0, 1, 2,
				2, 3, 0
			};



			veer::display::render::buffer_desc index_buffer_desc
			{
				.m_flags = veer::display::render::buffer_desc::usage_flags::index,
				.m_size = index_buffer_size,
				.m_stride = sizeof(uint32_t) 
			};

			index_buffer = std::make_unique<veer::display::render::render_device_buffer>(device, index_buffer_desc);
			index_buffer->set_data(containers::span<uint32_t>(index_buffer_data));


			veer::display::render::buffer_desc vertex_buffer_desc
			{
				.m_flags = veer::display::render::buffer_desc::usage_flags::vertex,
				.m_size = vertex_buffer_size,
				.m_stride = vertex_size * sizeof(float) 
			};

			vertex_buffer = std::make_unique<veer::display::render::render_device_buffer>(device, vertex_buffer_desc);
			vertex_buffer->set_data(containers::span<float>(vertex_buffer_data));



			// now, copy from upload heap to gpu resource
			std::unique_ptr<veer::display::render::copy_command_buffer> upload_command_buffer = std::make_unique<veer::display::render::copy_command_buffer>();
			{
				m_render_service->open_command_buffer(*upload_command_buffer);

				index_buffer->upload(*upload_command_buffer);
				vertex_buffer->upload(*upload_command_buffer);

				// not needed when everything is abstracted
				upload_command_buffer->transition_barrier(*index_buffer, index_buffer->get_sync_state_tracking().get_resource_state(), veer::display::render::render_device_resource_sync_state::IndexBuffer);
				upload_command_buffer->transition_barrier(*vertex_buffer, vertex_buffer->get_sync_state_tracking().get_resource_state(), veer::display::render::render_device_resource_sync_state::VertexAndConstantBuffer);

				m_render_service->close_command_buffer(*upload_command_buffer);
			}

			veer::display::render::command_buffer* upload_command_buffer_raw_ptr = upload_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::containers::span<veer::display::render::command_buffer*>(upload_command_buffer_raw_ptr));

			fence_values++;
			graphics_command_queue.signal(fence_values);
			graphics_command_queue.wait_for_value(fence_values);

			// temporary upload resources should be released automatically here
		}
		
		std::chrono::high_resolution_clock clock;
		auto t0_global = clock.now();
		auto t0_frame = t0_global;

		std::unique_ptr<veer::display::render::graphics_command_buffer> test_frame_command_buffer = std::make_unique<veer::display::render::graphics_command_buffer>();

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
				swap_chain = device.alloc<veer::display::render::swap_chain>( *window, current_win_size);

				for (size_t i = 0; i < veer::display::render::swap_chain::s_swap_chain_buffer_count; ++i)
				{
					backbuffers_fence_values[i] = backbuffers_fence_values[backbuffer_index];
				}
			}
			
			veer::display::render::render_device_texture_2d& backbuffer_resource = swap_chain->get_current_backbuffer();
			{
				m_render_service->open_command_buffer(*test_frame_command_buffer);

				// Transition RT to RenderTarget state (needed for Clear call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, backbuffer_resource.get_sync_state_tracking().get_resource_state(), veer::display::render::render_device_resource_sync_state::RenderTarget);
 
				// Clear RT
				// veer::math::vec4f clear_color( 0.4f, 0.6f, 0.9f, 1.0f );
				// test_frame_command_buffer->clear_render_target(backbuffer_resource, clear_color);

				// Draw !!
				{
					test_frame_command_buffer->set_technique(*fullscreen_uv_technique);

					test_frame_command_buffer->set_render_output(nullptr, &backbuffer_resource);

					test_frame_command_buffer->set_vertex_buffer(*vertex_buffer);
					test_frame_command_buffer->set_index_buffer(*index_buffer);

					display::render::viewport viewport
					{
						.m_position = math::vec2u(0u, 0u),
						.m_size = current_win_size, 
						.m_depth = math::vec2f(0.f, 1.f)
					};
					test_frame_command_buffer->set_viewports(containers::span<display::render::viewport>(viewport));

					display::render::rect scissor_rect
					{
						.m_min = math::vec2u(0u, 0u),
						.m_max = current_win_size
					};
					test_frame_command_buffer->set_scissors(scissor_rect);

					test_frame_command_buffer->draw_indexed_instanced(index_buffer_size, 1u);

// possible "submit context" usage idea
#if 0
					// const veer::display::render::graphics_submit_context& gfx_ctx = test_frame_command_buffer->alloc_graphics_submit_context();

					// gfx_ctx->set_technique(*fullscreen_uv_technique);
					// gfx_ctx->set_mesh()
					// {
					// 	// Record commands.
					// 	dx12_test_frame_command_buffer->get_api_handle()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					// 	dx12_test_frame_command_buffer->get_api_handle()->IASetVertexBuffers(0, 1, &vertex_buffer_view);
					// 	dx12_test_frame_command_buffer->get_api_handle()->IASetIndexBuffer( &index_buffer_view );
					// }

					// gfx_ctx->set_viewport()
					// gfx_ctx->set_scissors()
					// {
					// 	// draw function
					// 	D3D12_VIEWPORT viewport;
					// 	viewport.TopLeftX = 0;
					// 	viewport.TopLeftY = 0;
					// 	viewport.Width = current_win_size[0];
					// 	viewport.Height = current_win_size[1];
					// 	viewport.MinDepth = 0.0f;
					// 	viewport.MaxDepth = 1.0f;

					// 	D3D12_RECT scissor_rect;
					// 	scissor_rect.left = 0;
					// 	scissor_rect.top = 0;
					// 	scissor_rect.right = current_win_size[0];
					// 	scissor_rect.bottom = current_win_size[1];
					// 	
					// 	dx12_test_frame_command_buffer->get_api_handle()->RSSetViewports(1, &viewport);
					// 	dx12_test_frame_command_buffer->get_api_handle()->RSSetScissorRects(1, &scissor_rect);
					// }
				
					// gfx_ctx.submit();
					// {
					// 	dx12_test_frame_command_buffer->get_api_handle()->DrawIndexedInstanced( 6, 1, 0, 0, 0 );
					// }
#endif // 0
				}

				// Transition RT to Present state (needed for Present call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, backbuffer_resource.get_sync_state_tracking().get_resource_state(), veer::display::render::render_device_resource_sync_state::Present);

				m_render_service->close_command_buffer(*test_frame_command_buffer);
			}

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
		
			veer::display::render::dx12_render_device& dx12_device = static_cast<veer::display::render::dx12_render_device&>(device);
			dx12_device.check_errors();
		}

		// Wait for all previous work
		fence_values++;
		graphics_command_queue.signal(fence_values);
		graphics_command_queue.wait_for_value(fence_values);

		swap_chain = nullptr;

		return 0;
	}
}
