#include <display/render/render_device_resource.h>

#include "dx12_pch.h"
#include "dx12_render_device.h"
#include "dx12_render_device_resource_sync_state.h"

namespace veer::display::render
{
	render_device_resource::render_device_resource( render_device& _device )
		: m_device( _device )
		, m_upload_flags( render_device_resource::upload_flags::dirty_all )
	{
		
	}

	render_device_resource::~render_device_resource()
	{
		set_api_handle(nullptr);
		if ( m_resource_alloc != nullptr)
		{
			m_resource_alloc->Release();	
			m_resource_alloc = nullptr;
		}
	}

	void render_device_resource::alloc()
	{	
		set_api_handle(nullptr);
		if ( m_resource_alloc != nullptr)
		{
			m_resource_alloc->Release();	
			m_resource_alloc = nullptr;
		}

		dx12_render_device& dx12_device = static_cast<dx12_render_device&>(get_render_device());
		
		D3D12_RESOURCE_DESC resource_desc = get_resource_desc();
		
		HRESULT hr;

		D3D12MA::ALLOCATION_DESC default_alloc_desc = {};
		default_alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		// TODO : resource default state
		render_device_resource_sync_state default_state = render_device_resource_sync_state::Common;

		D3D12MA::Allocation* resource_alloc;
		hr = dx12_device.get_allocator()->CreateResource( &default_alloc_desc, &resource_desc, display::render::s_convert(default_state), NULL, &m_resource_alloc, IID_NULL, NULL);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12 resource. Error (" << hr << ")");
		
		set_api_handle(m_resource_alloc->GetResource());
		get_sync_state_tracking().set_resource_state(default_state);
	}

	void render_device_resource::copy_to_gpu(copy_command_buffer& _upload_buffer, veer::containers::span<const byte_t> _data)
	{
		dx12_render_device& dx12_device = static_cast<dx12_render_device&>(get_render_device());

		D3D12_RESOURCE_DESC resource_desc = get_resource_desc();
		
		HRESULT hr;

		D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
		upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;


		D3D12MA::Allocation* upload_alloc;
		hr = dx12_device.get_allocator()->CreateResource(&upload_alloc_desc, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &upload_alloc, IID_NULL, NULL);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create upload resource. Error (" << hr << ")");

		// this alloc needs to be released after the upload command buffer is executed and waited for 
		_upload_buffer.do_after_execution([upload_alloc](){ upload_alloc->Release(); });

		{
			uint8_t* data_vertex;
			upload_alloc->GetResource()->Map(0u, nullptr, reinterpret_cast<void**>(&data_vertex));
			memcpy(data_vertex, _data.data(), _data.size());
			upload_alloc->GetResource()->Unmap(0u, nullptr);
		}

		_upload_buffer.transition_barrier(*this, get_sync_state_tracking().get_resource_state(), render_device_resource_sync_state::CopyDest);

		// TODO : use common code
		// _upload_buffer.copy_texture(const render_device_texture_2d &_dst, const render_device_texture_2d &_src)->CopyResource(dx12_resource.get_api_handle(), upload_alloc->GetResource());
		_upload_buffer.get_api_handle()->CopyResource(get_api_handle(), upload_alloc->GetResource());
	}


	D3D12_RESOURCE_FLAGS render_device_resource::s_convert(buffer_desc::usage_flags _states)
	{
		std::pair<buffer_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = 
		{
			{ buffer_desc::usage_flags::index, 			D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::vertex, 		D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::constant, 		D3D12_RESOURCE_FLAG_NONE },
			{ buffer_desc::usage_flags::storage, 		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS },
			{ buffer_desc::usage_flags::indirect_args, 	D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS }
		};

		D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;
		for ( size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE( s_conversionTable ); ++i )
		{
			if ( flags::any( s_conversionTable[i].first & _states ) )
			{
				dx12_flags |= s_conversionTable[i].second;
			}
		}

		return dx12_flags;
	}

	D3D12_RESOURCE_FLAGS render_device_resource::s_convert(texture_desc::usage_flags _states)
	{
		std::pair<texture_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = 
		{
			{ texture_desc::usage_flags::storage, 		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS },
			{ texture_desc::usage_flags::render_target, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET },
			{ texture_desc::usage_flags::depth_stencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL }
		};

		D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;
		for ( size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE( s_conversionTable ); ++i )
		{
			if ( flags::any( s_conversionTable[i].first & _states ) )
			{
				dx12_flags |= s_conversionTable[i].second;
			}
		}

		return dx12_flags;
	}

	ID3D12Resource* render_device_resource::get_api_handle() const
	{
		return m_api_handle;
	}

	void render_device_resource::set_api_handle(ID3D12Resource* _resource)
	{
		if (m_api_handle != nullptr)
			m_api_handle->Release();

		m_api_handle = _resource;

		if (m_api_handle != nullptr)
			m_api_handle->AddRef();
	}
}