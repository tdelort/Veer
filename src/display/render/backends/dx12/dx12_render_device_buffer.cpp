#include <display/render/render_device_buffer.h>

#include "dx12_pch.h"

#include <display/render/render_device.h>

namespace veer::display::render 
{
	render_device_buffer::render_device_buffer(render_device& _device, const buffer_desc& _desc)
        : render_device_resource(_device)
        , m_desc{_desc}
	{
	}
	
	render_device_buffer::~render_device_buffer()
	{
		m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor);
		m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);
		m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_cbv_cpu_descriptor);
	}
	
	D3D12_RESOURCE_DESC render_device_buffer::get_resource_desc() const
	{
		D3D12_RESOURCE_DESC dx12_desc = {};

		dx12_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		dx12_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT; 
		dx12_desc.Height = 1u;
		dx12_desc.DepthOrArraySize = 1u;
		dx12_desc.MipLevels = 1u;
		dx12_desc.SampleDesc.Count = 1u;
		dx12_desc.SampleDesc.Quality = 0u;
		dx12_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		dx12_desc.Format = DXGI_FORMAT_UNKNOWN;


		const buffer_desc& veer_desc = desc();

		dx12_desc.Width = veer_desc.m_size * veer_desc.m_stride;
		dx12_desc.Flags = s_convert(veer_desc.m_flags);

		return dx12_desc;
	}

	byte_t* render_device_buffer::map()
	{
		byte_t* gpu_data;
		get_api_handle()->Map(0u, nullptr, reinterpret_cast<void**>(&gpu_data));
		return gpu_data;
	}

	void render_device_buffer::unmap()
	{
		get_api_handle()->Unmap(0u, nullptr);
	}

	void render_device_buffer::upload(copy_command_buffer& _upload_command_buffer, upload_flags _upload_flags)
	{
		// alloc
		if (flags::get(m_upload_flags, upload_flags::dirty_alloc) && flags::get(_upload_flags, upload_flags::dirty_alloc))
		{
			alloc(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
			get_sync_state_tracking().set_resource_state(render_device_resource_sync_state::Common);

			flags::unset(m_upload_flags, upload_flags::dirty_alloc);
		}

		// upload data
		if (flags::get(m_upload_flags, upload_flags::dirty_data) && flags::get(_upload_flags, upload_flags::dirty_data))
		{
			upload_data_to_upload_heap(get_data<byte_t>());

			flags::unset(m_upload_flags, upload_flags::dirty_data);
		}

		// TODO : we could do this only if we alloc (NOTE that we might want to create smaller/multiple views on the same alloc, and updating the data might lead to a need to update the views) 
		// NOTE we can have the "one alloc multiple view" behaviour by using a custom allocator in the alloc and have one to one render_device_resource <-> view mapping (using D3D12MA, that's already a little bit the case)
		if (flags::none(m_upload_flags))
		{
			update_views();
		}
	}

	void render_device_buffer::update_views()
	{
		const buffer_desc& buffer_desc = desc();	

		size_t size_in_bytes = buffer_desc.m_size * buffer_desc.m_stride;
		D3D12_GPU_VIRTUAL_ADDRESS location = get_api_handle()->GetGPUVirtualAddress();

		if (flags::get(buffer_desc.m_flags, buffer_desc::usage_flags::index))
		{
			m_index_buffer_view.BufferLocation = location;
			m_index_buffer_view.SizeInBytes = size_in_bytes;

			// TODO : expose this if needed
			m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		}

		if (flags::get(buffer_desc.m_flags, buffer_desc::usage_flags::vertex))
		{
			m_vertex_buffer_view.BufferLocation = location;
			m_vertex_buffer_view.SizeInBytes = size_in_bytes;

			m_vertex_buffer_view.StrideInBytes = buffer_desc.m_stride; 
		}

		{
			dx12_descriptor_heap& srv_heap = m_device.get_srv_uav_cbv_descriptor_heap();

			{
				m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_srv_cpu_descriptor);

				D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
				srv_desc.Format = DXGI_FORMAT_UNKNOWN;
				srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
				srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srv_desc.Buffer.FirstElement = 0u;
				srv_desc.Buffer.NumElements = buffer_desc.m_size;
				srv_desc.Buffer.StructureByteStride = buffer_desc.m_stride;
				srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

				m_srv_cpu_descriptor = srv_heap.acquire_descriptor();
				m_device.get_api_handle()->CreateShaderResourceView(get_api_handle(), &srv_desc, m_srv_cpu_descriptor.m_handle);
			}

			if (flags::get(buffer_desc.m_flags, buffer_desc::usage_flags::constant))
			{
				m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_cbv_cpu_descriptor);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
				cbv_desc.BufferLocation = location;
				cbv_desc.SizeInBytes = size_in_bytes;

				m_cbv_cpu_descriptor = srv_heap.acquire_descriptor();
				m_device.get_api_handle()->CreateConstantBufferView(&cbv_desc, m_cbv_cpu_descriptor.m_handle);
			}

			if (flags::get(buffer_desc.m_flags, buffer_desc::usage_flags::storage))
			{
				m_device.get_srv_uav_cbv_descriptor_heap().release_descriptor(m_uav_cpu_descriptor);

				D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
				uav_desc.Format = DXGI_FORMAT_UNKNOWN;
				uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				uav_desc.Buffer.FirstElement = 0u;
				uav_desc.Buffer.NumElements = buffer_desc.m_size;
				uav_desc.Buffer.StructureByteStride = buffer_desc.m_stride;
				uav_desc.Buffer.CounterOffsetInBytes = 0u;
				uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

				m_uav_cpu_descriptor = srv_heap.acquire_descriptor();
				m_device.get_api_handle()->CreateUnorderedAccessView(get_api_handle(), nullptr, &uav_desc, m_uav_cpu_descriptor.m_handle);
			}
		}
	}

	const D3D12_INDEX_BUFFER_VIEW& render_device_buffer::get_index_buffer_view() const
	{
		VEER_ASSERT(flags::get(desc().m_flags, buffer_desc::usage_flags::index), "Buffer should be alloc with index usage flag to be used as index buffer");
		VEER_ASSERT(flags::none(get_upload_flags()), "Buffer should be uploaded before accessing one of its view (not init at this point)");
		return m_index_buffer_view;
	}

	const D3D12_VERTEX_BUFFER_VIEW& render_device_buffer::get_vertex_buffer_view() const
	{
		VEER_ASSERT(flags::get(desc().m_flags, buffer_desc::usage_flags::vertex), "Buffer should be alloc with index usage flag to be used as vertex buffer");
		VEER_ASSERT(flags::none(get_upload_flags()), "Buffer should be uploaded before accessing one of its view (not init at this point)");
		return m_vertex_buffer_view;
	}

	render_device_resource::bindless_id render_device_buffer::get_bindless_id(render_device_resource_heap_type _heap_type) const
	{
		switch (_heap_type) 
		{
			case render_device_resource_heap_type::srv:
				return static_cast<bindless_id>(m_srv_cpu_descriptor.m_index);
			case render_device_resource_heap_type::uav:
				return static_cast<bindless_id>(m_uav_cpu_descriptor.m_index);
			case render_device_resource_heap_type::cbv:
				return static_cast<bindless_id>(m_cbv_cpu_descriptor.m_index);
        }

		return s_invalid_bindless_id;
	}

#if 0
	const dx12_descriptor& render_device_buffer::get_constant_buffer_view() const
	{
		VEER_ASSERT(flags::get(desc().m_flags, buffer_desc::usage_flags::constant), "Buffer should be alloc with constant usage flag to be used as constant buffer");
		VEER_ASSERT(flags::none(get_upload_flags()), "Buffer should be uploaded before accessing one of its view (not init at this point)");
		return m_constant_buffer_cpu_descriptor;
	}
#endif // 0
}