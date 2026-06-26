#include <display/render/render_device_texture_3d.h>

#include "dx12_pch.h"
#include "dx12_render_device.h"
#include "dx12_render_device_data_format.h"

namespace veer::display::render 
{
	render_device_texture_3d::render_device_texture_3d(render_device& _device, const texture_3d_desc& _desc)
        : render_device_texture_base(_device)
        , m_desc{ _desc }
	{
	}
	
	render_device_texture_3d::~render_device_texture_3d()
	{
		// TODO
		// release views, allocs
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>(m_device);
		if ( m_render_target_cpu_descriptor.is_valid() )
			dx12_device.get_rtv_descriptor_heap().release_descriptor(m_render_target_cpu_descriptor);
	}
	
	void render_device_texture_3d::upload(copy_command_buffer& _upload_buffer)
	{
		// alloc
		if (flags::get(m_upload_flags, upload_flags::dirty_alloc))
		{
			alloc();

			flags::unset(m_upload_flags, upload_flags::dirty_alloc);
		}

		// upload data
		if (flags::get(m_upload_flags, upload_flags::dirty_data))
		{
			// TODO : handle textures
			// copy_to_gpu(_upload_buffer, get_data<byte_t>());
			VEER_ASSERT(false, "Not implemented")

			flags::unset(m_upload_flags, upload_flags::dirty_data);
		}

		if (flags::none(m_upload_flags))
		{
			update_views();
		}
	}

	D3D12_RESOURCE_DESC render_device_texture_3d::get_resource_desc() const
	{
		D3D12_RESOURCE_DESC dx12_desc{};

		dx12_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		dx12_desc.Alignment = 0u; // use default. TODO : add support for small textures
		dx12_desc.SampleDesc.Count = 1u;
		dx12_desc.SampleDesc.Quality = 0u;
		dx12_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		const texture_3d_desc& veer_desc = desc();

		dx12_desc.Width = veer_desc.m_size[0u];
		dx12_desc.Height = veer_desc.m_size[1u];
		dx12_desc.DepthOrArraySize = veer_desc.m_size[2u];
		dx12_desc.MipLevels = 1u; // TODO : add support for mips

		dx12_desc.Format = display::render::s_convert(veer_desc.m_format);
		dx12_desc.Flags = s_convert(veer_desc.m_flags);

		return dx12_desc;
	}

	void render_device_texture_3d::update_views()
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>( m_device );

		const texture_3d_desc& texture_desc = desc();
		if ( flags::get(texture_desc.m_flags, texture_desc::usage_flags::render_target) )
		{
			dx12_descriptor_heap& rtv_heap = dx12_device.get_rtv_descriptor_heap();

			m_render_target_cpu_descriptor = rtv_heap.acquire_descriptor();

			// TODO : nullptr desc arg
			dx12_device.get_api_handle()->CreateRenderTargetView(get_api_handle(), nullptr, m_render_target_cpu_descriptor.m_handle );
		}

		// TODO other views
	}

	const dx12_descriptor& render_device_texture_3d::get_render_target_view() const
	{
		return m_render_target_cpu_descriptor;
	}

	
}