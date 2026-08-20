#pragma once

#include "dx12_pch.h"

#include <core/unique_ptr.h>
#include <core/math/vec.h>

#include <display/render/swap_chain.h>
#include <display/render/backends/dx12/dx12_render_device_backbuffer.h>


namespace veer::display::window
{
	class window;
}

namespace veer::display::render
{
	class render_device;

	class dx12_swap_chain : public veer::display::render::swap_chain 
	{
	public:
		dx12_swap_chain(render_device& _device, veer::display::window::window& _window);
		virtual ~dx12_swap_chain();

		void present(size_t _sync_intervals) override;
		render_device_texture_2d& get_current_backbuffer() override;
		size_t get_backbuffer_index() override;
	private:
		ComPtr<IDXGISwapChain4> m_api_swap_chain_handle;
		ComPtr<ID3D12DescriptorHeap> m_back_buffers_descritor_heap;

		containers::static_array<unique_ptr<dx12_render_device_backbuffer>, s_swap_chain_buffer_count> m_back_buffers_resources = {};
	};
}