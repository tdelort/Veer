#pragma once

#include "dx12_pch.h"
#include "display/render/render_device_buffer.h"
#include "display/render/command_buffer.h"

namespace veer::display::render
{
	class dx12_render_device_resource : public render_device_buffer
	{
	public:
        virtual void upload( command_buffer& _upload_command_buffer );

	public:
		ID3D12Resource* get_resource();

		virtual ~dx12_render_device_resource();
	private:
        D3D12MA::Allocation* m_device_alloc;
	};
}