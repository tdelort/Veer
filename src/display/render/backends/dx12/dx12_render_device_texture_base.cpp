#include <display/render/render_device_texture_base.h>

#include "dx12_pch.h"
#include "dx12_render_device.h"

namespace veer::display::render 
{
	render_device_texture_base::render_device_texture_base(render_device& _device)
        : render_device_resource(_device)
	{
	}
	
	render_device_texture_base::~render_device_texture_base()
	{
		// TODO
	}

	void render_device_texture_base::upload(copy_command_buffer& _upload_buffer, upload_flags _upload_flags)
	{
		// alloc
		if (flags::get(m_upload_flags, upload_flags::dirty_alloc) && flags::get(_upload_flags, upload_flags::dirty_alloc))
		{
			alloc(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON);
			get_sync_state_tracking().set_resource_state(render_device_resource_sync_state::Common);

			flags::unset(m_upload_flags, upload_flags::dirty_alloc);
		}

		// upload data
		if (flags::get(m_upload_flags, upload_flags::dirty_data) && flags::get(_upload_flags, upload_flags::dirty_data))
		{
			upload_data_to_default_heap(_upload_buffer, get_data<byte_t>());

			flags::unset(m_upload_flags, upload_flags::dirty_data);
		}

		if (flags::none(m_upload_flags))
		{
			update_views();
		}
	}
}