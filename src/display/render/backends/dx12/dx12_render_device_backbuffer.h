#pragma once

#include "dx12_pch.h"

#include "dx12_descriptor_heap.h"

#include <d3d12.h>
#include <display/render/render_device_resource.h>
#include <display/render/render_device_buffer.h>
#include <display/render/command_buffer.h>


namespace veer::display::render
{
	class dx12_render_device_backbuffer : public render_device_texture_2d
    {
	public:
		dx12_render_device_backbuffer(render_device& _device, const texture_2d_desc& _desc, ID3D12Resource* _backbuffer_resource);
		virtual ~dx12_render_device_backbuffer() override;

		virtual void upload(copy_command_buffer& _upload_buffer ) override;
	
	private:
		ComPtr<ID3D12Resource> m_api_handle;
    };
}