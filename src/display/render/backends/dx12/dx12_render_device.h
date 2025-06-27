#ifndef DISPLAY_DX12_DX12_RENDER_DEVICE_H_INCLUDED
#define DISPLAY_DX12_DX12_RENDER_DEVICE_H_INCLUDED

#include "dx12_pch.h"
#include "dx12_command_queue.h"

#include <display/render/render_device.h>
#include <display/render/command_buffer.h>
#include <display/window/window.h>


namespace veer
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
		std::unique_ptr<swap_chain> create_swap_chain(window& _window, vec2u _size) override;
		void delete_swap_chain(std::unique_ptr<swap_chain> _swap_chain) override;

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

#endif // DISPLAY_DX12_DX12_RENDER_DEVICE_H_INCLUDED
