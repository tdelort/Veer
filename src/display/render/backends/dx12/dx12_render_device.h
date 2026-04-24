#pragma once

#include "dx12_pch.h"
#include "dx12_command_queue.h"

#include <display/render/render_device.h>
#include <display/render/command_buffer.h>
#include <display/window/window.h>

namespace veer::display::render
{
	class swap_chain;
	class dx12_descriptor_heap;

	class dx12_render_device : public render_device
	{
	public:
		dx12_render_device();
		~dx12_render_device() override;

		ComPtr<ID3D12Device2> get_api_handle() const;
		D3D12MA::Allocator* get_allocator() const;
		ComPtr<IDXGIFactory4> get_dxgi_factory() const;

		dx12_descriptor_heap& get_rtv_descriptor_heap() const;
		dx12_descriptor_heap& get_srv_descriptor_heap() const;

		command_queue& get_command_queue(command_buffer::type _corresponding_command_buffer_type) override;

		// factory part
		std::unique_ptr<swap_chain> create_swap_chain(veer::display::window::window& _window, veer::math::vec2u _size) override;

  		std::unique_ptr<graphics_technique> create_graphics_technique(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state) override;
    	std::unique_ptr<compute_technique> create_compute_technique(const shader_stage_source_container_t& _source_code) override;

	private:
		void create_descriptor_heaps();
		void create_allocator();

		std::unique_ptr<dx12_command_queue> m_graphics_queue;

		ComPtr<ID3D12Device2> m_api_device_handle;
		ComPtr<IDXGIAdapter1> m_adapter;
		ComPtr<IDXGIFactory4> m_dxgi_factory;

		std::unique_ptr<dx12_descriptor_heap> m_rtv_descriptor_heap;
		std::unique_ptr<dx12_descriptor_heap> m_srv_descriptor_heap;
		// std::unique_ptr<dx12_descriptor_heap> m_sampler_descriptor_heap;

		D3D12MA::Allocator* m_allocator;
#if defined(_DEBUG)
		ComPtr<IDXGIInfoQueue> m_info_queue;
#endif // defined(_DEBUG)

	};
}