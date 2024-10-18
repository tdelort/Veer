#ifndef DISPLAY_DX12_DX12_SWAP_CHAIN_H_INCLUDED
#define DISPLAY_DX12_DX12_SWAP_CHAIN_H_INCLUDED

#include "dx12_pch.h"

#include <Display/swap_chain.h>
#include <Display/DX12/dx12_render_device.h>
#include <Core/Math/vec.h>

namespace veer
{
	class window;
	class dx12_rendering_service;

	class dx12_swap_chain : public swap_chain 
	{
	public:
		dx12_swap_chain( dx12_rendering_service& _render_service, window& _window, vec2u _window_size );
		virtual ~dx12_swap_chain();

		void present(size_t _sync_intervals) override;

		ComPtr<ID3D12Resource> get_backbuffer_resource( uint64_t _frame_index );
		D3D12_CPU_DESCRIPTOR_HANDLE get_backbuffer_cpu_handle( uint64_t _frame_index );

	private:
		ComPtr<IDXGISwapChain4> m_api_swap_chain_handle;
		ComPtr<ID3D12DescriptorHeap> m_back_buffers_descritor_heap;

		ComPtr<ID3D12Resource> m_back_buffers_resources[s_swap_chain_buffer_count];
		D3D12_CPU_DESCRIPTOR_HANDLE m_back_buffers_cpu_handles[s_swap_chain_buffer_count];
	};
}

#endif // DISPLAY_DX12_DX12_SWAP_CHAIN_H_INCLUDED
