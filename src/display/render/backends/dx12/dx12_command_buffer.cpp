#include "dx12_pch.h"

#include "dx12_command_buffer.h"

#include "dx12_render_device.h"
#include "dx12_render_device_resource.h"
#include "dx12_render_device_resource_sync_state.h"
#include "dx12_technique.h"

#include <core/debug.h>

#include <display/render/render_device_resource.h>

namespace veer::display::render
{
	D3D12_COMMAND_LIST_TYPE dx12_command_buffer::s_to_dx12_type(const command_buffer::type _type)
	{
		D3D12_COMMAND_LIST_TYPE dx12_type;
		switch (_type)
		{
		case command_buffer::type::Graphics:
			dx12_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case command_buffer::type::Compute:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			break;
		case command_buffer::type::Copy:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		default:
			VEER_ASSERT(false, "All command_buffer::type should be covered");
			break;
		}

		return dx12_type;
	}

	dx12_command_buffer::dx12_command_buffer(command_buffer::type _type)
		: command_buffer( _type )
	{
	}

	void dx12_command_buffer::open( dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator )
	{
		const D3D12_COMMAND_LIST_TYPE dx12_type = dx12_command_buffer::s_to_dx12_type(m_type);
		VEER_ASSERT(m_type == command_buffer::type::Graphics, "We don't support other types yet");

		if (m_command_list_handle == nullptr)
		{
			const HRESULT hr = _device.get_api_handle()->CreateCommandList(0, dx12_type, _command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_command_list_handle));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list (" << hr << ")");
		}
		else
		{
			m_command_list_handle->Reset(_command_allocator.Get(), nullptr);
		}
	}

	void dx12_command_buffer::close()
	{
		if (m_command_list_handle != nullptr)
		{
			const HRESULT hr = m_command_list_handle->Close();
			VEER_ASSERT(SUCCEEDED(hr), "Failed to properly close command list (" << hr << ")");
		}
	}

	// TODO : This is the low level stuff, it should be private 
	void dx12_command_buffer::transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		dx12_render_device_resource& dx12_resource = static_resource_cast<dx12_render_device_resource>(_resource);
		barrier.Transition.pResource = dx12_resource.get_api_handle();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = s_convert( _from_state );
		barrier.Transition.StateAfter = s_convert( _to_state );
		get_api_handle()->ResourceBarrier(1, &barrier);
	}

	void dx12_command_buffer::clear_render_target(render_device_resource& _render_target_resource, veer::math::vec4f _color)
	{
		const float arr[4] = { _color[0], _color[1], _color[2], _color[3] };
		get_api_handle()->ClearRenderTargetView( static_resource_cast<dx12_render_device_resource>( _render_target_resource ).get_cpu_handle().m_handle, arr, 0, nullptr);
	}

	void dx12_command_buffer::set_technique(graphics_technique& _technique)
	{
		dx12_graphics_technique& dx12_technique = static_cast<dx12_graphics_technique&>(_technique);
		get_api_handle()->SetPipelineState(dx12_technique.get_pipeline_state_object());
		get_api_handle()->SetGraphicsRootSignature(dx12_technique.get_root_signature());
	}

	void dx12_command_buffer::set_technique(compute_technique& _technique)
	{
		dx12_compute_technique& dx12_technique = static_cast<dx12_compute_technique&>(_technique);
		get_api_handle()->SetPipelineState(dx12_technique.get_pipeline_state_object());
		get_api_handle()->SetGraphicsRootSignature(dx12_technique.get_root_signature());
	}

	ComPtr<ID3D12GraphicsCommandList> dx12_command_buffer::get_api_handle()
	{
		return m_command_list_handle;
	}
}
