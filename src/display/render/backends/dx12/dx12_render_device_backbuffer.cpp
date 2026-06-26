#include "dx12_render_device_backbuffer.h"

namespace veer::display::render
{
	dx12_render_device_backbuffer::dx12_render_device_backbuffer(render_device& _device, const texture_2d_desc& _desc, ID3D12Resource* _backbuffer_resource)
		: render_device_texture_2d( _device, _desc )
	{
		set_api_handle(_backbuffer_resource);

		update_views();

		flags::unset(m_upload_flags, render_device_resource::upload_flags::dirty_all);
	}

	dx12_render_device_backbuffer::~dx12_render_device_backbuffer()
	{
	}

	void dx12_render_device_backbuffer::upload(copy_command_buffer& _upload_buffer)
	{
		// override to explicitly do nothing here since we got the resource from the OS
	}
}
