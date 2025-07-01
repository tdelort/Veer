#include "application.h"
#include "display/render/backends/dx12/dx12_render_device_resource.h"

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

#include <chrono>
#include <dxgiformat.h>
#include <vector>

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
		// controlling deletion order
		m_window_service.reset();
		m_render_service.reset();
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
		window* window = m_window_service->create_window(initial_win_size);
		VEER_ASSERT(window != nullptr, ("Wrong handle to window"));
		VEER_LOG( "we have a window yipee" );


		vec2u current_win_size = window->get_size();

		std::unique_ptr<swap_chain> swap_chain = device.create_swap_chain(*window, current_win_size);

		command_queue& graphics_command_queue = device.get_command_queue(command_buffer::type::Graphics);

		uint64_t fence_values = 0;

#if 0
		// TODO : abstractize
		ComPtr<ID3D12RootSignature> root_signature_obj;
		{
			HRESULT hr;
			dx12_render_device& dx12_device = static_cast<dx12_render_device&>(device);

			static constexpr const char* s_shader_source = R"(
				struct VsInput
				{
					float3 pos : POSITION;
					float2 uv : TEXCOORD;
				};
				struct VsOutput
				{
					float4 pos : SV_Position;
					float2 uv : TEXCOORD;
				};

				VsOutput mainVS( VsInput in )
				{
					VsOutput out;
					out.pos = float4( in.pos.xyz, 1 );
					out.uv = in.uv;
					return out;
				}

				float4 mainPS( VsOutput in ) : SV_Target0
				{
					const float4 color( in.uv, 0.f, 1.f );
					return color;
				}
			)";

			ComPtr<ID3DBlob> shader_vs, shader_ps;
#if defined(_DEBUG)
			// Enable better shader debugging with the graphics debugging tools.
			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			UINT compileFlags = 0;
#endif


			// D3DCompile(_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
			// 		   _In_ SIZE_T SrcDataSize,
			// 		   _In_opt_ LPCSTR pSourceName,
			// 		   _In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
			// 		   _In_opt_ ID3DInclude* pInclude,
			// 		   _In_opt_ LPCSTR pEntrypoint,
			// 		   _In_ LPCSTR pTarget,
			// 		   _In_ UINT Flags1,
			// 		   _In_ UINT Flags2,
			// 		   _Out_ ID3DBlob** ppCode,
			// 		   _Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorMsgs);
			{
				ComPtr<ID3DBlob> shader_vs_error_blob;
				hr = D3DCompile(s_shader_source, VEER_STATIC_ARRAY_SIZE(s_shader_source), "DefaultTestShader", nullptr, nullptr, "mainVS", "vs_5_0", compileFlags, 0, &shader_vs, &shader_vs_error_blob);
				VEER_ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error Message : " << (const char*)shader_vs_error_blob->GetBufferPointer());
			}

			{
				ComPtr<ID3DBlob> shader_ps_error_blob;
				hr = D3DCompile(s_shader_source, VEER_STATIC_ARRAY_SIZE(s_shader_source), "DefaultTestShader", nullptr, nullptr, "mainPS", "ps_5_0", compileFlags, 0, &shader_ps, &shader_ps_error_blob);
				VEER_ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error Message : " << (const char*)shader_ps_error_blob->GetBufferPointer());
			}

			// Define the vertex input layout.

			// LPCSTR SemanticName;
			// UINT SemanticIndex;
			// DXGI_FORMAT Format;
			// UINT InputSlot;
			// UINT AlignedByteOffset;
			// D3D12_INPUT_CLASSIFICATION InputSlotClass;
			// UINT InstanceDataStepRate;
			D3D12_INPUT_ELEMENT_DESC input_elems_desc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 3 * 4, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			// Don't care, bindless and no resources yet
			// D3D12_ROOT_PARAMETER root_param;
			// D3D12_ROOT_PARAMETER_TYPE ParameterType;
			// union 
			// {
			// 	D3D12_ROOT_DESCRIPTOR_TABLE DescriptorTable;
			// 	D3D12_ROOT_CONSTANTS Constants;
			// 	D3D12_ROOT_DESCRIPTOR Descriptor;
			// };
			// D3D12_SHADER_VISIBILITY ShaderVisibility;

			D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
			// UINT NumParameters;
			// _Field_size_full_(NumParameters)  const D3D12_ROOT_PARAMETER *pParameters;
			// UINT NumStaticSamplers;
			// _Field_size_full_(NumStaticSamplers)  const D3D12_STATIC_SAMPLER_DESC *pStaticSamplers;
			// D3D12_ROOT_SIGNATURE_FLAGS Flags;
			root_signature_desc.NumParameters = 0u;
			root_signature_desc.pParameters = nullptr;
			root_signature_desc.NumStaticSamplers = 0u;
			root_signature_desc.pStaticSamplers = nullptr;

			ComPtr<ID3DBlob> root_signature_blob, error_blob;
			hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_signature_blob, &error_blob);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to serialize root signature. Error (" << hr << ")");

			const void* root_signature_data = root_signature_blob->GetBufferPointer();
			size_t root_signature_data_size = root_signature_blob->GetBufferSize();

			hr = dx12_device.get_api_handle()->CreateRootSignature(0, root_signature_data, root_signature_data_size, IID_PPV_ARGS(&root_signature_obj));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create root signature. Error (" << hr << ")");

			D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
			pso_desc.pRootSignature = root_signature_obj.Get();
			pso_desc.VS.pShaderBytecode = shader_vs->GetBufferPointer();
			pso_desc.VS.BytecodeLength = shader_vs->GetBufferSize();
			pso_desc.PS.pShaderBytecode = shader_ps->GetBufferPointer();
			pso_desc.PS.BytecodeLength = shader_ps->GetBufferSize();
			pso_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			pso_desc.InputLayout.NumElements = VEER_STATIC_ARRAY_SIZE(input_elems_desc);
			pso_desc.InputLayout.pInputElementDescs = input_elems_desc;
			pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			pso_desc.NumRenderTargets = 1;
			pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			pso_desc.SampleDesc.Count = 1; // default no multisampling

			ComPtr<ID3D12PipelineState> pso;
			hr = dx12_device.get_api_handle()->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&pso));
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
				0.f, 0.f, 0.f,   0.f, 0.f,
				1.f, 0.f, 0.f,   1.f, 0.f,
				0.f, 1.f, 0.f,   0.f, 1.f,
				1.f, 1.f, 0.f,   1.f, 1.f
			};


			static const size_t index_buffer_size = 6;
			static const size_t index_buffer_size_in_bytes = index_buffer_size * sizeof(unsigned int);

			static const unsigned int index_buffer[index_buffer_size] =
			{
				0, 1, 2,
				2, 1, 0
			};

			dx12_render_device& dx12_device = static_cast<dx12_render_device&>(device);

			D3D12_RESOURCE_DESC vb_desc = {};
			vb_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			vb_desc.Alignment = 0;
			vb_desc.Width = vertex_buffer_size_in_bytes;
			vb_desc.Height = 1;
			vb_desc.DepthOrArraySize = 1;
			vb_desc.MipLevels = 1;
			vb_desc.Format = DXGI_FORMAT_UNKNOWN;
			vb_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			vb_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_RESOURCE_DESC ib_desc = {};
			vb_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			vb_desc.Alignment = 0;
			vb_desc.Width = index_buffer_size_in_bytes;
			vb_desc.Height = 1;
			vb_desc.DepthOrArraySize = 1;
			vb_desc.MipLevels = 1;
			vb_desc.Format = DXGI_FORMAT_UNKNOWN;
			vb_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			vb_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			 

			D3D12MA::ALLOCATION_DESC default_alloc_desc = {};
			default_alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

			D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
			upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
			 

			D3D12MA::Allocation* vb_resource_alloc;
			HRESULT hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &vb_desc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, &vb_resource_alloc, IID_NULL, NULL);

			D3D12MA::Allocation* vb_upload_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &upload_alloc_desc, &vb_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &vb_upload_alloc, IID_NULL, NULL);


			D3D12MA::Allocation* ib_resource_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &ib_desc, D3D12_RESOURCE_STATE_COPY_DEST, NULL, &ib_resource_alloc, IID_NULL, NULL);

			D3D12MA::Allocation* ib_upload_alloc;
			hr = dx12_device.get_allocator()->CreateResource( &upload_alloc_desc, &ib_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &ib_upload_alloc, IID_NULL, NULL);

			void* data;
			vb_upload_alloc->GetResource()->Map(0u, nullptr, &data);
			memcpy(data, vertex_buffer, vertex_buffer_size_in_bytes);

			// now, copy from upload heap to gpu resource
			std::unique_ptr<command_buffer> upload_command_buffer = m_render_service->start_recording_command_buffer(command_buffer::type::Graphics);
			
			dx12_command_buffer* dx12_upload_command_buffer = static_cast<dx12_command_buffer*>(upload_command_buffer.get());
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

			command_buffer* upload_command_buffer_raw_ptr = upload_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::span<command_buffer*>( &upload_command_buffer_raw_ptr ));

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
		}
#endif // 0

		

		uint64_t frame_index = 0;
		size_t backbuffer_index = swap_chain->get_backbuffer_index();

		uint64_t backbuffers_fence_values[swap_chain::s_swap_chain_buffer_count] = {};
		
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
				swap_chain = device.create_swap_chain( *window, current_win_size);

				for (size_t i = 0; i < swap_chain::s_swap_chain_buffer_count; ++i)
				{
					backbuffers_fence_values[i] = backbuffers_fence_values[backbuffer_index];
				}
			}
			
			std::unique_ptr<command_buffer> test_frame_command_buffer = m_render_service->start_recording_command_buffer(command_buffer::type::Graphics);

			render_device_resource* backbuffer_resource = swap_chain->get_current_backbuffer();
			{
				// Transition RT to RenderTarget state (needed for Clear call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, resource_sync_state::Present, resource_sync_state::RenderTarget);
 

				// Clear RT
				vec4f clear_color( 0.4f, 0.6f, 0.9f, 1.0f );
				test_frame_command_buffer->clear_render_target(backbuffer_resource, clear_color);

				// Draw !!

				#if 0
				dx12_command_buffer* dx12_test_frame_command_buffer = static_cast<dx12_command_buffer*>(test_frame_command_buffer.get());

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
				
				dx12_test_frame_command_buffer->get_api_handle()->SetGraphicsRootSignature(root_signature_obj.Get());
				dx12_test_frame_command_buffer->get_api_handle()->RSSetViewports(1, &viewport);
				dx12_test_frame_command_buffer->get_api_handle()->RSSetScissorRects(1, &scissor_rect);
				
				dx12_test_frame_command_buffer->get_api_handle()->OMSetRenderTargets(1, &static_cast<dx12_render_device_resource*>( backbuffer_resource )->get_cpu_descriptor_handle().m_handle, FALSE, nullptr);

				// Record commands.
				const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
				dx12_test_frame_command_buffer->get_api_handle()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				dx12_test_frame_command_buffer->get_api_handle()->IASetVertexBuffers(0, 1, &vertex_buffer_view);
				dx12_test_frame_command_buffer->get_api_handle()->IASetIndexBuffer( &index_buffer_view );
				dx12_test_frame_command_buffer->get_api_handle()->DrawInstanced(3, 1, 0, 0);
				#endif 0
			}

			{
				// Transition RT to Present state (needed for Present call)
				test_frame_command_buffer->transition_barrier(backbuffer_resource, resource_sync_state::RenderTarget, resource_sync_state::Present);
			}

			m_render_service->stop_recording_command_buffer( *test_frame_command_buffer );

			command_buffer* test_frame_command_buffer_raw_ptr = test_frame_command_buffer.get();
			graphics_command_queue.execute_command_buffers(veer::span<command_buffer*>(&test_frame_command_buffer_raw_ptr));

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
