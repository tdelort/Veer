#ifndef DISPLAY_DX12_DX12_COMMAND_QUEUE_H_INCLUDED
#define DISPLAY_DX12_DX12_COMMAND_QUEUE_H_INCLUDED

#include "dx12_pch.h"

#include <Display/command_buffer.h>
#include <Display/command_queue.h>

namespace veer
{
	class dx12_render_device;

	class dx12_command_queue : public command_queue
	{
	public:
		dx12_command_queue( dx12_render_device& _device, command_buffer::type _type );
		~dx12_command_queue() override;

		void execute_command_buffers(span<command_buffer*> _command_buffers) override;

		void signal(uint64_t _value) override;
		void wait_for_value(uint64_t _value) override;

		ComPtr<ID3D12CommandQueue>& get_api_handle();
	private:
		ComPtr<ID3D12CommandQueue> m_command_queue_api_handle;

		ComPtr<ID3D12Fence> m_fence;
	};
}

#endif // DISPLAY_DX12_DX12_COMMAND_QUEUE_H_INCLUDED
