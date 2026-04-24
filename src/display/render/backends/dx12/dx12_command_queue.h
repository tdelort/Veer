#pragma once

#include "dx12_pch.h"

#include <display/render/command_buffer.h>
#include <display/render/command_queue.h>

namespace veer::display::render
{
	class dx12_render_device;

	class dx12_command_queue : public command_queue
	{
	public:
		dx12_command_queue( dx12_render_device& _device, command_buffer::type _type );
		~dx12_command_queue() override;

		void execute_command_buffers(veer::containers::span<command_buffer*> _command_buffers) override;

		void signal(uint64_t _value) override;
		void wait_for_value(uint64_t _value) override;

		ComPtr<ID3D12CommandQueue> get_api_handle();
	private:
		ComPtr<ID3D12CommandQueue> m_command_queue_api_handle;

		ComPtr<ID3D12Fence> m_fence;
	};
}