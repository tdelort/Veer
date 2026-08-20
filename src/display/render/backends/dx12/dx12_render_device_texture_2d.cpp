#include <display/render/render_device_texture_2d.h>

#include "dx12_pch.h"

#include <core/debug.h>
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
#include <display/render/backends/dx12/dx12_render_device_data_format.h>
#include <display/render/render_device_resource.h>
#include <display/render/render_device_texture_base.h>
#include <display/render/render_device.h>

namespace veer::display::render 
{
	render_device_texture_2d::render_device_texture_2d(render_device& _device, const texture_2d_desc& _desc)
        : render_device_texture_base(_device)
        , m_desc{ _desc }
	{
	}
	
	render_device_texture_2d::~render_device_texture_2d()
	{
		m_device.get_rtv_descriptor_heap().release_descriptor(m_rtv_cpu_descriptor);
		m_device.get_dsv_descriptor_heap().release_descriptor(m_dsv_cpu_descriptor); 
		m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor); 
		m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);
	}

	D3D12_RESOURCE_DESC render_device_texture_2d::get_resource_desc() const
	{
		D3D12_RESOURCE_DESC dx12_desc{};

		dx12_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		dx12_desc.Alignment = 0u; // use default. TODO : add support for small textures
		dx12_desc.SampleDesc.Count = 1u;
		dx12_desc.SampleDesc.Quality = 0u;
		dx12_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		const texture_2d_desc& veer_desc = desc();

		dx12_desc.Width = veer_desc.m_size[0u];
		dx12_desc.Height = veer_desc.m_size[1u];
		dx12_desc.DepthOrArraySize = 1u; // TODO : add support for depth textures
		dx12_desc.MipLevels = 1u; // TODO : add support for mips

		dx12_desc.Format = display::render::s_convert(veer_desc.m_format);
		dx12_desc.Flags = s_convert(veer_desc.m_flags);

		return dx12_desc;
	}

	// TODO : at one point, I will probably need to make different views on the same texture
	// I could keep them here (for example for views on each mips, this is easy)
	// BUT I will also probably need to create views of different types.
	// When this happens, I can simply add a way to request a new view on an existing texture, and keep the default views below
	// something like "unique_ptr<render_device_texture_2d_view> create_view(render_device_texture_2d_view_desc _desc)"
	void render_device_texture_2d::update_views()
	{
		const texture_2d_desc& texture_desc = desc();

		{
			dx12_descriptor_heap& srv_uav_cbv_heap = m_device.get_srv_uav_cbv_descriptor_heap();

			{
				m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor);

				D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
				srv_desc.Format = display::render::s_convert(texture_desc.m_format);
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srv_desc.Texture2D.MostDetailedMip = 0u; // see comment above function when views on mips are needed
				srv_desc.Texture2D.MipLevels = 1u; // TODO : add support for mips
				srv_desc.Texture2D.PlaneSlice = 0u;
				srv_desc.Texture2D.ResourceMinLODClamp = 0.f;

				m_srv_cpu_descriptor = srv_uav_cbv_heap.acquire_descriptor();
				m_device.get_api_handle()->CreateShaderResourceView(get_api_handle(), &srv_desc, m_srv_cpu_descriptor.m_handle);
			}

			if (flags::get(texture_desc.m_flags, texture_desc::usage_flags::storage))
			{
				m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);

				D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
				uav_desc.Format = display::render::s_convert(texture_desc.m_format);
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				uav_desc.Texture2D.MipSlice = 0u; // see comment above function when views on other mips are needed
				uav_desc.Texture2D.PlaneSlice = 0u;

				m_uav_cpu_descriptor = srv_uav_cbv_heap.acquire_descriptor();
				m_device.get_api_handle()->CreateUnorderedAccessView(get_api_handle(), nullptr, &uav_desc, m_uav_cpu_descriptor.m_handle);
			}
		}

		if (flags::get(texture_desc.m_flags, texture_desc::usage_flags::render_target))
		{
			m_device.get_rtv_descriptor_heap().release_descriptor(m_rtv_cpu_descriptor);

			D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
			rtv_desc.Format = display::render::s_convert(texture_desc.m_format);
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtv_desc.Texture2D.MipSlice = 0u; // see comment above function when views on other mips are needed
			rtv_desc.Texture2D.PlaneSlice = 0u;

			dx12_descriptor_heap& rtv_heap = m_device.get_rtv_descriptor_heap();
			m_rtv_cpu_descriptor = rtv_heap.acquire_descriptor();
			m_device.get_api_handle()->CreateRenderTargetView(get_api_handle(), &rtv_desc, m_rtv_cpu_descriptor.m_handle);
		}

		if (flags::get(texture_desc.m_flags, texture_desc::usage_flags::depth_stencil))
		{
			m_device.get_dsv_descriptor_heap().release_descriptor(m_dsv_cpu_descriptor);

			D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
			dsv_desc.Format = display::render::s_convert(texture_desc.m_format);
			dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Flags = D3D12_DSV_FLAG_NONE; // D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL
			dsv_desc.Texture2D.MipSlice = 0u; // see comment above function when views on other mips are needed

			dx12_descriptor_heap& dsv_heap = m_device.get_dsv_descriptor_heap();
			m_dsv_cpu_descriptor = dsv_heap.acquire_descriptor();
			m_device.get_api_handle()->CreateDepthStencilView(get_api_handle(), &dsv_desc, m_dsv_cpu_descriptor.m_handle );
		}
	}

	const dx12_descriptor& render_device_texture_2d::get_render_target_view() const
	{
		return m_rtv_cpu_descriptor;
	}

	const dx12_descriptor& render_device_texture_2d::get_depth_stencil_view() const
	{
		return m_dsv_cpu_descriptor;
	}

	render_device_resource::bindless_id render_device_texture_2d::get_bindless_id(render_device_resource_heap_type _heap_type) const
	{
		switch (_heap_type) 
		{
			case render_device_resource_heap_type::srv:
				return static_cast<bindless_id>(m_srv_cpu_descriptor.m_index);
			case render_device_resource_heap_type::uav:
				return static_cast<bindless_id>(m_uav_cpu_descriptor.m_index);
			default:
				VEER_ASSERT(false, "Texture 2D only supports SRV and UAV heap types for get_bindless_id");
        }

		return s_invalid_bindless_id;
	}
}