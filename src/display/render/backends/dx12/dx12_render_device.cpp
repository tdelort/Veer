#include "dx12_pch.h"
#include "dx12_render_device.h"

#include "dx12_technique.h"
#include "dx12_command_buffer.h"
#include "dx12_swap_chain.h"
#include "dx12_command_queue.h"
#include "dx12_rendering_service.h"

#include <display/window/window.h>
#include <display/render/swap_chain.h>
#include <display/render/command_buffer.h>
#include <display/render/rendering_service.h>

namespace veer::display::render
{
	static constexpr size_t s_rtv_descriptor_heap_size = 4096u;
	static constexpr size_t s_srv_descriptor_heap_size = 4096u;

	dx12_render_device::dx12_render_device()
	{
		UINT factory_flags = 0;

#if defined(_DEBUG)
		ComPtr<ID3D12Debug> debug_interface;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface))))
		{
			debug_interface->EnableDebugLayer();
		}
		else
		{
			VEER_LOG_ERROR("Failed to get ID3D12 Debug Interface ");
		}

		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_info_queue))))
		{
			factory_flags = DXGI_CREATE_FACTORY_DEBUG;

			m_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			m_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
			m_info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, TRUE);
		}
#endif // defined(_DEBUG)

		HRESULT hr = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_dxgi_factory));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create DXGIFactory4, which is required for DX12! (" << hr << ")");



		ComPtr<IDXGIAdapter1> adapter;
		for (UINT adapter_index = 0; DXGI_ERROR_NOT_FOUND != m_dxgi_factory->EnumAdapters1(adapter_index, &adapter); ++adapter_index)
		{

			DXGI_ADAPTER_DESC1 desc;
			HRESULT hr = adapter->GetDesc1(&desc);
			if (FAILED(hr))
				continue;

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12,
			// but don't create the actual device yet.
			if (SUCCEEDED(
				D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0,
					_uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		m_adapter = adapter;
		ComPtr<ID3D12Device2> d3d12_device_2;
		hr = D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12_device_2));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12 Device (" << hr << ")");

#if 0
		if (SUCCEEDED(d3d12_device_2.As(&m_info_queue)))
		{
			m_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			m_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			m_info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

			D3D12_INFO_QUEUE_FILTER info_queue_filter = {};
#if 0
			// Suppress whole categories of messages
			D3D12_MESSAGE_CATEGORY categories[] = {};
			info_queue_filter.DenyList.NumCategories = _countof(categories);
			info_queue_filter.DenyList.pCategoryList = categories;
#endif // 0

#if 0
			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};
			info_queue_filter.DenyList.NumSeverities = _countof(severities);
			info_queue_filter.DenyList.pSeverityList = severities;
#endif // 0

#if 0
			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID deny_ids[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};
			info_queue_filter.DenyList.NumIDs = _countof(deny_ids);
			info_queue_filter.DenyList.pIDList = deny_ids;
#endif // 0

			HRESULT hr = m_info_queue->PushStorageFilter(&info_queue_filter);
			VEER_ASSERT(SUCCEEDED(hr), "An error occurend when pushing storage filter in info queue (" << hr << ")");
		}
#endif // defined(_DEBUG)

		m_api_device_handle = d3d12_device_2;

		// Now create needed command queues

		m_graphics_queue = std::make_unique<dx12_command_queue>(*this, command_buffer::type::Graphics);
		//m_compute_queue = std::unique_ptr<dx12_command_queue>(new dx12_command_queue( this, command_buffer::type::Compute ));
		//m_copy_queue = std::unique_ptr<dx12_command_queue>(new dx12_command_queue( this, command_buffer::type::Copy ));

		create_allocator();
		create_descriptor_heaps();
	}

	void dx12_render_device::create_descriptor_heaps()
	{
		m_rtv_descriptor_heap = std::make_unique<dx12_descriptor_heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, s_rtv_descriptor_heap_size);
		m_srv_descriptor_heap = std::make_unique<dx12_descriptor_heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, s_rtv_descriptor_heap_size);
		// m_sampler_descriptor_heap = std::make_unique<dx12_descriptor_heap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, s_rtv_descriptor_heap_size);
	}

	void dx12_render_device::create_allocator()
	{
		// Alloc and upload buffer resources
		D3D12MA::ALLOCATOR_DESC allocator_desc = {};
		allocator_desc.pDevice = m_api_device_handle.Get();
		allocator_desc.pAdapter = m_adapter.Get();
		allocator_desc.Flags = D3D12MA::ALLOCATOR_FLAGS( D3D12MA::ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED );

		HRESULT hr = D3D12MA::CreateAllocator(&allocator_desc, &m_allocator);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12MA Allocator");
	}

	dx12_render_device::~dx12_render_device()
	{
		// release all before live objects reporting :)
		m_graphics_queue.reset();
		m_dxgi_factory.Reset();
		m_rtv_descriptor_heap.reset();
		m_srv_descriptor_heap.reset();
		m_allocator->Release();
		m_api_device_handle.Reset();

#if defined(_DEBUG)
		ComPtr<IDXGIDebug1> dxgi_debug;

		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_debug))))
		{
			dxgi_debug->ReportLiveObjects( DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_ALL) );
		}
#endif // defined(_DEBUG)
	}

	command_queue& dx12_render_device::get_command_queue(command_buffer::type _corresponding_command_buffer_type)
	{
		// TODO use arg when supporting more queues
		(void)_corresponding_command_buffer_type;

		return *m_graphics_queue.get();
	}



	std::unique_ptr<swap_chain> dx12_render_device::create_swap_chain( veer::display::window::window& _window, veer::math::vec2u _size)
	{
		return std::make_unique<dx12_swap_chain>( *this, _window, _size);
	}

	std::unique_ptr<graphics_technique> dx12_render_device::create_graphics_technique(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state)
	{
		return std::make_unique<dx12_graphics_technique>(*this, _source_code, _signature, _render_state);
	}

	std::unique_ptr<compute_technique> dx12_render_device::create_compute_technique(const shader_stage_source_container_t& _source_code)
	{
		return std::make_unique<dx12_compute_technique>(*this, _source_code);
	}



	ComPtr<ID3D12Device2> dx12_render_device::get_api_handle() const
	{
		return m_api_device_handle;
	}

	D3D12MA::Allocator* dx12_render_device::get_allocator() const
	{
		return m_allocator;
	}

	ComPtr<IDXGIFactory4> dx12_render_device::get_dxgi_factory() const
	{
		return m_dxgi_factory;
	}

	dx12_descriptor_heap& dx12_render_device::get_rtv_descriptor_heap() const
	{
		return *m_rtv_descriptor_heap;
	}

	dx12_descriptor_heap& dx12_render_device::get_srv_descriptor_heap() const
	{
		return *m_srv_descriptor_heap;
	}
}
