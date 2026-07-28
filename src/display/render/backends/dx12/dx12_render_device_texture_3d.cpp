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
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>(m_device);

		dx12_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor); 
		dx12_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);
	}

	D3D12_RESOURCE_DESC render_device_texture_3d::get_resource_desc() const
	{
		D3D12_RESOURCE_DESC dx12_desc{};

		dx12_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;

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

		dx12_descriptor_heap& srv_uav_cbv_heap = dx12_device.get_srv_uav_cbv_descriptor_heap();

		{
			dx12_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor); 

			D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
			srv_desc.Format = display::render::s_convert(texture_desc.m_format);
			srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
			srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srv_desc.Texture3D.MostDetailedMip = 0u; // see comment above function when views on mips are needed
			srv_desc.Texture3D.MipLevels = 1u; // TODO : add support for mips
			srv_desc.Texture3D.ResourceMinLODClamp = 0.f;

			m_srv_cpu_descriptor = srv_uav_cbv_heap.acquire_descriptor();
			dx12_device.get_api_handle()->CreateShaderResourceView(get_api_handle(), &srv_desc, m_srv_cpu_descriptor.m_handle);
		}

		if (flags::get(texture_desc.m_flags, texture_desc::usage_flags::storage))
		{
			dx12_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);

			D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
			uav_desc.Format = display::render::s_convert(texture_desc.m_format);
			uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			uav_desc.Texture3D.MipSlice = 0u; // see comment above function when views on other mips are needed
			uav_desc.Texture3D.FirstWSlice = 0u;
			uav_desc.Texture3D.WSize = -1; // -1 means all

			m_uav_cpu_descriptor = srv_uav_cbv_heap.acquire_descriptor();
			dx12_device.get_api_handle()->CreateUnorderedAccessView(get_api_handle(), nullptr, &uav_desc, m_uav_cpu_descriptor.m_handle);
		}
	}

	render_device_resource::bindless_id render_device_texture_3d::get_bindless_id(render_device_resource_heap_type _heap_type) const
	{
		switch (_heap_type) 
		{
			case render_device_resource_heap_type::srv:
				return static_cast<bindless_id>(m_srv_cpu_descriptor.m_index);
			case render_device_resource_heap_type::uav:
				return static_cast<bindless_id>(m_uav_cpu_descriptor.m_index);
			default:
				VEER_ASSERT(false, "Texture 3D only supports SRV and UAV heap types for get_bindless_id");
        }

		return s_invalid_bindless_id;
	}
}