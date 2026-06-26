#include <display/render/render_device_buffer.h>

#include "dx12_pch.h"
#include "dx12_render_device.h"

namespace veer::display::render 
{
	render_device_buffer::render_device_buffer(render_device& _device, const buffer_desc& _desc)
        : render_device_resource(_device)
        , m_desc{_desc}
	{
	}
	
	render_device_buffer::~render_device_buffer()
	{
		// TODO
	}
	
	void render_device_buffer::upload(copy_command_buffer& _upload_buffer)
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
			copy_to_gpu(_upload_buffer, get_data<byte_t>());

			flags::unset(m_upload_flags, upload_flags::dirty_data);
		}

		if (flags::none(m_upload_flags))
		{
			update_views();
		}
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

		if (flags::get(buffer_desc.m_flags, buffer_desc::usage_flags::constant))
		{
			dx12_render_device& dx12_device = static_cast<dx12_render_device&>( m_device );
			dx12_descriptor_heap& srv_heap = dx12_device.get_srv_descriptor_heap();

			m_constant_buffer_cpu_descriptor = srv_heap.acquire_descriptor();

			D3D12_CONSTANT_BUFFER_VIEW_DESC constant_buffer_view_desc = {};
			constant_buffer_view_desc.BufferLocation = location;
			constant_buffer_view_desc.SizeInBytes = size_in_bytes;

			dx12_device.get_api_handle()->CreateConstantBufferView(&constant_buffer_view_desc, m_constant_buffer_cpu_descriptor.m_handle);
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

	const dx12_descriptor& render_device_buffer::get_constant_buffer_view() const
	{
		VEER_ASSERT(flags::get(desc().m_flags, buffer_desc::usage_flags::constant), "Buffer should be alloc with constant usage flag to be used as constant buffer");
		VEER_ASSERT(flags::none(get_upload_flags()), "Buffer should be uploaded before accessing one of its view (not init at this point)");
		return m_constant_buffer_cpu_descriptor;
	}
}