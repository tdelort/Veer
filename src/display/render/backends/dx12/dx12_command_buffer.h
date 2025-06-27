#ifndef DISPLAY_DX12_DX12_COMMAND_BUFFER_H_INCLUDED
#define DISPLAY_DX12_DX12_COMMAND_BUFFER_H_INCLUDED

#include "dx12_pch.h"
#include "dx12_render_device.h"

#include <display/render/command_buffer.h>

namespace veer
{
	class dx12_command_buffer : public command_buffer
	{
	public:
		dx12_command_buffer(command_buffer::type _type);

		void open( dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator );
		void close();

		void transition_barrier(render_device_resource* _resource, resource_sync_state _from_state, resource_sync_state _to_state) override;

		void clear_render_target(render_device_resource* _render_target_resource, vec4f _color) override;

		ComPtr<ID3D12GraphicsCommandList> get_api_handle();

		static D3D12_COMMAND_LIST_TYPE s_to_dx12_type(const command_buffer::type _type);
	private:
		// We might need another handle type on other platforms (XBox DMA CommandLists)
		ComPtr<ID3D12GraphicsCommandList> m_command_list_handle;
	};
}

#endif // DISPLAY_DX12_DX12_COMMAND_BUFFER_H_INCLUDED
