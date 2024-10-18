#include "dx12_swap_chain.h"

#include <application.h>

#include <Display/rendering_service.h>
#include <Display/window.h>
#include <Display/DX12/dx12_rendering_service.h>

namespace veer
{
	dx12_swap_chain::dx12_swap_chain(dx12_rendering_service& _render_service, window& _window, vec2u _window_size )
		: swap_chain()
    {
		bool allow_tearing = false;
		
		ComPtr<IDXGIFactory4> factory4 = _render_service.get_dxgi_factory();

		// DXGI 4 supposed since DX12
		ComPtr<IDXGIFactory5> factory5;
        if (SUCCEEDED(factory4.As(&factory5)))
        {
			if (FAILED(factory5->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing))))
			{
				allow_tearing = false;
			}
		}
		m_tearing_allowed = allow_tearing;
		
		// get hwnd from window
		HWND windows_window_handle = _window.get_os_window_handle();
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>(_render_service.get_render_device());

		// Create swap chain
		{
			DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
			swap_chain_desc.Width = _window_size[0];
			swap_chain_desc.Height = _window_size[1];
			swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swap_chain_desc.Stereo = FALSE;
			swap_chain_desc.SampleDesc = { 1, 0 };
			swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swap_chain_desc.BufferCount = s_swap_chain_buffer_count;
			swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			// It is recommended to always allow tearing if tearing support is available.
			swap_chain_desc.Flags = m_tearing_allowed ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

			dx12_command_queue& command_queue = static_cast<dx12_command_queue&>( dx12_device.get_command_queue(command_buffer::type::Graphics) );

			ComPtr<IDXGISwapChain1> swap_chain1;
			HRESULT hr = factory4->CreateSwapChainForHwnd( command_queue.get_api_handle().Get(), windows_window_handle, &swap_chain_desc, nullptr, nullptr, &swap_chain1);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create swap chain (" << hr << ")");
	 
			hr = factory4->MakeWindowAssociation(windows_window_handle, DXGI_MWA_NO_ALT_ENTER);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create make window association (" << hr << ")");
	 
			hr = swap_chain1.As(&m_api_swap_chain_handle);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to cast swap chain to DXGI 4 version (" << hr << ")");
		}
		
		ID3D12Device2* id3d12_device = dx12_device.get_api_handle();
		// Create backbuffer descriptors
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.NumDescriptors = s_swap_chain_buffer_count;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		 
			HRESULT hr = id3d12_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_back_buffers_descritor_heap));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create swap chain descriptor heap (" << hr << ")");
		}
	 
		// Retrieve backbuffer resources
		{

			unsigned int rtv_descriptor_size = id3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	 
			D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_back_buffers_descritor_heap->GetCPUDescriptorHandleForHeapStart();
	 
			for (size_t i = 0; i < s_swap_chain_buffer_count; ++i)
			{
				ComPtr<ID3D12Resource> back_buffer_resource;
				HRESULT hr = m_api_swap_chain_handle->GetBuffer((UINT)i, IID_PPV_ARGS(&back_buffer_resource));
				VEER_ASSERT(SUCCEEDED(hr), "Failed to retrieve swap chain back buffer resource");
		 
				id3d12_device->CreateRenderTargetView(back_buffer_resource.Get(), nullptr, rtv_handle);
		 
				m_back_buffers_resources[i] = back_buffer_resource;
				m_back_buffers_cpu_handles[i] = rtv_handle;
		 
				rtv_handle.ptr += rtv_descriptor_size;
			}
		}
    }

	dx12_swap_chain::~dx12_swap_chain()
	{
		for (size_t i = 0; i < s_swap_chain_buffer_count; ++i)
			m_back_buffers_resources[i].Reset();
	}

	void dx12_swap_chain::present(size_t _sync_intervals)
	{
		UINT flags = m_tearing_allowed && (_sync_intervals == 0) ? DXGI_PRESENT_ALLOW_TEARING : 0;
		HRESULT hr = m_api_swap_chain_handle->Present((UINT)_sync_intervals, flags);

		VEER_ASSERT(SUCCEEDED(hr), "Failed to Present swap chain");
	}

	ComPtr<ID3D12Resource> dx12_swap_chain::get_backbuffer_resource( uint64_t _frame_index )
	{
		return m_back_buffers_resources[_frame_index % s_swap_chain_buffer_count];
	}

	D3D12_CPU_DESCRIPTOR_HANDLE dx12_swap_chain::get_backbuffer_cpu_handle( uint64_t _frame_index )
	{
		return m_back_buffers_cpu_handles[_frame_index % s_swap_chain_buffer_count];
	}

}
