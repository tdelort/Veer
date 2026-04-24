#include "display/render/backends/dx12/dx12_render_device.h"
#include "display/render/render_device_resource.h"
#include "display/render/render_device_resource_sync_state.h"
#include "display/render/resource_desc.h"
#include "dx12_pch.h"
#include "dx12_render_device.h"
#include "dx12_render_device_resource.h"
#include "dx12_render_device_data_format.h"

#include "dx12_command_buffer.h"
#include "dx12_render_device_resource_sync_state.h"
#include <d3d12.h>

namespace veer::display::render
{
	// TODO : this function should belong to render_device_resource (or one of it's child)
	void s_resize( render_device& _device, render_device_resource& _engine_resource )
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>( _device );
		
		dx12_render_device_resource& dx12_resource = static_resource_cast<dx12_render_device_resource>( _engine_resource );
		D3D12_RESOURCE_DESC resource_desc = dx12_resource.get_resource_desc();
		
		HRESULT hr;

		D3D12MA::ALLOCATION_DESC default_alloc_desc = {};
		default_alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		// TODO : resource default state
		render_device_resource_sync_state default_state = render_device_resource_sync_state::Common;

		D3D12MA::Allocation* resource_alloc;
		hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &resource_desc, s_convert(default_state), NULL, &resource_alloc, IID_NULL, NULL);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12 resource. Error (" << hr << ")");
		
		dx12_resource.set_api_handle( resource_alloc->GetResource() );
		_engine_resource.get_sync_state_tracking().set_resource_state(default_state);
	}

	void s_copy_to_gpu( render_device& _device, command_buffer& _upload_buffer, render_device_resource& _engine_resource )
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>( _device );

		dx12_render_device_resource& dx12_resource = static_resource_cast<dx12_render_device_resource>( _engine_resource );
		D3D12_RESOURCE_DESC resource_desc = dx12_resource.get_resource_desc();
		
		HRESULT hr;


		D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
		upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;


		D3D12MA::Allocation* upload_alloc;
		hr = dx12_device.get_allocator()->CreateResource( &upload_alloc_desc, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &upload_alloc, IID_NULL, NULL);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer upload resource. Error (" << hr << ")");

		_upload_buffer.transition_barrier( _engine_resource, _engine_resource.get_sync_state_tracking().get_resource_state(), render_device_resource_sync_state::CopyDest );

		dx12_command_buffer& dx12_upload_command_buffer = static_cast<dx12_command_buffer&>(_upload_buffer);
		dx12_upload_command_buffer.get_api_handle()->CopyResource(dx12_resource.get_api_handle(), upload_alloc->GetResource());
	}

// --- dx12_render_device_resource ---

	ID3D12Resource* dx12_render_device_resource::get_api_handle() const
	{
		return m_api_handle.Get();
	}

	void dx12_render_device_resource::set_api_handle( ID3D12Resource* _val )
	{
		m_api_handle = _val;
	}

	const dx12_descriptor& dx12_render_device_resource::get_cpu_handle() const
	{
		return m_cpu_descriptor;
	}


// --- dx12_render_device_buffer ---
	D3D12_RESOURCE_FLAGS s_convert( buffer_desc::usage_flags _states )
	{
		std::pair<buffer_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = 
		{
			{ buffer_desc::usage_flags::index, 			D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::vertex, 		D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::constant, 		D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::storage, 		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS },
			{ buffer_desc::usage_flags::indirect_args, 	D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS }
		};

		D3D12_RESOURCE_FLAGS dx12_flags;
		for ( size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE( s_conversionTable ); ++i )
		{
			if ( flags::any( s_conversionTable[i].first & _states ) )
			{
				dx12_flags |= s_conversionTable[i].second;
			}
		}

		return dx12_flags;
	}

	dx12_render_device_buffer::dx12_render_device_buffer( render_device& _device, const buffer_desc& _desc )
		: render_device_buffer( _device, _desc )
	{

	}

	dx12_render_device_buffer::~dx12_render_device_buffer()
	{
		// TODO
	}
	
	void dx12_render_device_buffer::upload( command_buffer& _upload_buffer )
	{
		if ( flags::get( m_upload_flags, render_device_resource::upload_flags::dirty_alloc ) )
		{
			// alloc
		}
	}

	D3D12_RESOURCE_DESC dx12_render_device_buffer::get_resource_desc() const
	{
		D3D12_RESOURCE_DESC dx12_desc{};

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

		dx12_desc.Width = veer_desc.m_size;
		dx12_desc.Flags = s_convert( veer_desc.m_flags );

		return dx12_desc;
	}

	void dx12_render_device_buffer::update_views()
	{
		// TODO
	}


// --- dx12_render_device_texture_2d ---

	D3D12_RESOURCE_FLAGS s_convert( texture_desc::usage_flags _states )
	{
		std::pair<texture_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = 
		{
			{ texture_desc::usage_flags::storage, 		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS },
			{ texture_desc::usage_flags::render_target, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET },
			{ texture_desc::usage_flags::depth_stencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL }
		};

		D3D12_RESOURCE_FLAGS dx12_flags;
		for ( size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE( s_conversionTable ); ++i )
		{
			if ( flags::any( s_conversionTable[i].first & _states ) )
			{
				dx12_flags |= s_conversionTable[i].second;
			}
		}

		return dx12_flags;
	}

	dx12_render_device_texture_2d::dx12_render_device_texture_2d( render_device& _device, const texture_2d_desc& _desc )
		: render_device_texture_2d( _device, _desc )
	{

	}
	
	dx12_render_device_texture_2d::~dx12_render_device_texture_2d()
	{
		// TODO
	}
	
	void dx12_render_device_texture_2d::upload( command_buffer& _upload_buffer )
	{
		
	}

	D3D12_RESOURCE_DESC dx12_render_device_texture_2d::get_resource_desc() const
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

		dx12_desc.Format = s_convert( veer_desc.m_format );
		dx12_desc.Flags = s_convert( veer_desc.m_flags );

		return dx12_desc;
	}

	void dx12_render_device_texture_2d::update_views()
	{
		// TODO
	}

// --- dx12_render_device_texture_3d ---
	dx12_render_device_texture_3d::dx12_render_device_texture_3d( render_device& _device, const texture_3d_desc& _desc )
		: render_device_texture_3d( _device, _desc )
	{

	}	

	dx12_render_device_texture_3d::~dx12_render_device_texture_3d()
	{
		// TODO
	}

	void dx12_render_device_texture_3d::upload( command_buffer& _upload_buffer )
	{
		
	}

	D3D12_RESOURCE_DESC dx12_render_device_texture_3d::get_resource_desc() const
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

		dx12_desc.Format = s_convert( veer_desc.m_format );
		dx12_desc.Flags = s_convert( veer_desc.m_flags );

		return dx12_desc;
	}

	void dx12_render_device_texture_3d::update_views()
	{
		// TODO
	}

// --- dx12_render_device_backbuffer ---

	dx12_render_device_backbuffer::dx12_render_device_backbuffer( render_device& _device, const texture_2d_desc& _desc, ID3D12Resource* _backbuffer_resource )
		: dx12_render_device_texture_2d( _device, _desc )
	{
		set_api_handle(_backbuffer_resource);
		// TODO: handle like other resources ?? 
		update_views();
	}

	dx12_render_device_backbuffer::~dx12_render_device_backbuffer()
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>( m_device );
		if ( m_cpu_descriptor.is_valid() )
			dx12_device.get_rtv_descriptor_heap().release_descriptor( m_cpu_descriptor );
	}

	void dx12_render_device_backbuffer::upload( command_buffer& _upload_buffer )
	{
		// nothing to do here
	}

	void dx12_render_device_backbuffer::update_views()
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>( m_device );
		dx12_descriptor_heap& rtv_heap = dx12_device.get_rtv_descriptor_heap();

		m_cpu_descriptor = rtv_heap.acquire_descriptor();
		dx12_device.get_api_handle()->CreateRenderTargetView(get_api_handle(), nullptr, m_cpu_descriptor.m_handle );
	}
}
