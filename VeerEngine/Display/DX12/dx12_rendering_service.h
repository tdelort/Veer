#ifndef DISPLAY_DX12_DX12_RENDERING_SERVICE_H_INCLUDED
#define DISPLAY_DX12_DX12_RENDERING_SERVICE_H_INCLUDED

#include "dx12_pch.h"

#include <Core/service.h>
#include <Display/render_device.h>
#include <Display/rendering_service.h>

namespace veer
{
	class dx12_rendering_service : public service_generic<rendering_service>
	{
	public:
		dx12_rendering_service();
		~dx12_rendering_service() override;

		// rendering_thread
		void start_frame( uint64_t _frame_index ) override;
		void end_frame() override;

		std::unique_ptr<command_buffer> start_recording_command_buffer(command_buffer::type _type) override;
		void stop_recording_command_buffer(command_buffer& _command_buffer) override;
		// end rendering_thread
	private:
		ComPtr<ID3D12CommandAllocator> m_command_allocators[swap_chain::s_swap_chain_buffer_count];
	};
}

#endif // DISPLAY_DX12_DX12_RENDERING_SERVICE_H_INCLUDED
