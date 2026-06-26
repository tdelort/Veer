#include "dx12_swap_chain.h"

#include "display/render/render_device_data_format.h"
#include "dx12_descriptor_heap.h"
#include "dx12_render_device_backbuffer.h"

#include <display/render/rendering_service.h>
#include <display/window/window.h>

namespace veer::display::render
{
	dx12_swap_chain::dx12_swap_chain(dx12_render_device& _device, veer::display::window::window& _window, veer::math::vec2u _window_size )
		: veer::display::render::swap_chain()
    {
		
		ComPtr<IDXGIFactory4> factory4 = _device.get_dxgi_factory();

		BOOL allow_tearing = false;
		ComPtr<IDXGIFactory5> factory5;
        if (SUCCEEDED(factory4.As(&factory5)))
        {
			if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing))))
			{
				allow_tearing = false;
			}
		}
		m_tearing_allowed = allow_tearing;
		
		// get hwnd from window
		HWND windows_window_handle = _window.get_os_window_handle();

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

			dx12_command_queue& command_queue = static_cast<dx12_command_queue&>( _device.get_command_queue(command_buffer::type::graphics) );

			ComPtr<IDXGISwapChain1> swap_chain1;
			HRESULT hr = factory4->CreateSwapChainForHwnd(command_queue.get_api_handle().Get(), windows_window_handle, &swap_chain_desc, nullptr, nullptr, &swap_chain1);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create swap chain (" << hr << ")");

			hr = swap_chain1.As(&m_api_swap_chain_handle);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to cast swap chain to DXGI 4 version (" << hr << ")");
	 
			hr = factory4->MakeWindowAssociation(windows_window_handle, DXGI_MWA_NO_ALT_ENTER);
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create make window association (" << hr << ")");
	 
		}
		
		ComPtr<ID3D12Device2> id3d12_device = _device.get_api_handle();
		// Retrieve backbuffer resources
		{
			dx12_descriptor_heap& rtv_heap = _device.get_rtv_descriptor_heap();
	 
			for (size_t i = 0; i < s_swap_chain_buffer_count; ++i)
			{
				ComPtr<ID3D12Resource> back_buffer_resource;
				HRESULT hr = m_api_swap_chain_handle->GetBuffer((UINT)i, IID_PPV_ARGS(&back_buffer_resource));
				VEER_ASSERT(SUCCEEDED(hr), "Failed to retrieve swap chain back buffer resource");
				VEER_ASSERT(back_buffer_resource.Get() != nullptr, "Retrieved swap chain back buffer resource is nullptr");

				texture_2d_desc backbuffer_desc;
				backbuffer_desc.m_size = _window_size;
				backbuffer_desc.m_flags = texture_desc::usage_flags::render_target;
				backbuffer_desc.m_format = render_device_data_format::r8g8b8a8_unorm;
		 
				m_back_buffers_resources[i] = std::make_unique<dx12_render_device_backbuffer>(_device, backbuffer_desc, back_buffer_resource.Get());
			}
		}
    }

	dx12_swap_chain::~dx12_swap_chain() 
	{
		for (size_t i = 0; i < s_swap_chain_buffer_count; ++i)
		{
			m_back_buffers_resources[i] = nullptr;
		}
		m_api_swap_chain_handle.Reset();
	}

	void dx12_swap_chain::present(size_t _sync_intervals)
	{
		UINT flags = m_tearing_allowed && (_sync_intervals == 0) ? DXGI_PRESENT_ALLOW_TEARING : 0;
		HRESULT hr = m_api_swap_chain_handle->Present((UINT)_sync_intervals, flags);

		VEER_ASSERT(SUCCEEDED(hr), "Failed to Present swap chain");
	}

	render_device_texture_2d& dx12_swap_chain::get_current_backbuffer()
	{
		render_device_texture_2d* resource_ptr = static_cast<render_device_texture_2d*>( m_back_buffers_resources[get_backbuffer_index()].get() );
		return *resource_ptr;
	}

	size_t dx12_swap_chain::get_backbuffer_index()
	{
		UINT index = m_api_swap_chain_handle->GetCurrentBackBufferIndex();
		return (size_t)index;
	}
}
