#pragma once

#include "dx12_pch.h"

#include <core/service.h>

#include <display/render/rendering_service.h>
#include <display/render/command_buffer.h>
#include <display/render/swap_chain.h>

namespace veer::display::render
{
	class dx12_rendering_service : public service_generic<rendering_service>
	{
	public:
		dx12_rendering_service();
		~dx12_rendering_service() override;

		// rendering_thread
		void start_frame( uint64_t _frame_index ) override;
		void end_frame() override;

		void open_command_buffer(command_buffer& command_buffer) override;
		void close_command_buffer(command_buffer& command_buffer) override;
		// end rendering_thread
	private:
		ComPtr<ID3D12CommandAllocator> m_command_allocators[swap_chain::s_swap_chain_buffer_count];
	};

}