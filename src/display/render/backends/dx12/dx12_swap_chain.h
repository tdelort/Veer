#pragma once

#include "dx12_pch.h"

#include "dx12_render_device.h"
#include "dx12_render_device_backbuffer.h"

#include <core/math/vec.h>

#include <display/render/swap_chain.h>

namespace veer::display::window
{
	class window;
}

namespace veer::display::render
{
	class dx12_rendering_service;

	class dx12_swap_chain : public veer::display::render::swap_chain 
	{
	public:
		dx12_swap_chain( dx12_render_device& _device, veer::display::window::window& _window, veer::math::vec2u _window_size );
		virtual ~dx12_swap_chain();

		void present(size_t _sync_intervals) override;
		render_device_texture_2d& get_current_backbuffer() override;
		size_t get_backbuffer_index() override;
	private:
		ComPtr<IDXGISwapChain4> m_api_swap_chain_handle;
		ComPtr<ID3D12DescriptorHeap> m_back_buffers_descritor_heap;

		std::unique_ptr<dx12_render_device_backbuffer> m_back_buffers_resources[s_swap_chain_buffer_count];
	};
}