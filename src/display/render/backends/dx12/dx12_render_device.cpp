#include <display/render/render_device.h>

#include "dx12_pch.h"

#include <core/containers/resizable_array.h>
#include <core/containers/static_array.h>
#include <core/debug.h>
#include <core/unique_ptr.h>

#include <display/render/backends/dx12/dx12_technique.h>
#include <display/render/backends/dx12/dx12_swap_chain.h>

#include <display/window/window.h>
#include <display/render/swap_chain.h>
#include <display/render/command_buffer.h>
#include <display/render/command_queue.h>

namespace veer::display::render
{
	static constexpr size_t s_rtv_descriptor_heap_size = 1024u;
	static constexpr size_t s_dsv_descriptor_heap_size = s_rtv_descriptor_heap_size;
	static constexpr size_t s_srv_uav_cbv_descriptor_heap_size = 4096u;
	static constexpr size_t s_sampler_descriptor_heap_size = 64u;

	render_device::render_device()
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

		VEER_LOG("CreateDXGIFactory2");
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
		VEER_LOG("D3D12CreateDevice");
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

		m_graphics_queue = unique_ptr<graphics_command_queue>::make(*this);
		m_compute_queue = unique_ptr<compute_command_queue>::make(*this);
		m_copy_queue = unique_ptr<copy_command_queue>::make(*this);

		create_allocator();
		create_descriptor_heaps();
	}

	void render_device::create_descriptor_heaps()
	{
		m_rtv_descriptor_heap = unique_ptr<dx12_descriptor_heap>::make(*this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, s_rtv_descriptor_heap_size);
		m_dsv_descriptor_heap = unique_ptr<dx12_descriptor_heap>::make(*this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, s_dsv_descriptor_heap_size);
		m_srv_uav_cbv_descriptor_heap = unique_ptr<dx12_descriptor_heap>::make(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, s_srv_uav_cbv_descriptor_heap_size);
		m_sampler_descriptor_heap = unique_ptr<dx12_descriptor_heap>::make(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, s_sampler_descriptor_heap_size);
	}

	void render_device::create_allocator()
	{
		// Alloc and upload buffer resources
		D3D12MA::ALLOCATOR_DESC allocator_desc = {};
		allocator_desc.pDevice = m_api_device_handle.Get();
		allocator_desc.pAdapter = m_adapter.Get();
		allocator_desc.Flags = D3D12MA::ALLOCATOR_FLAGS( D3D12MA::ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED );

		VEER_LOG("CreateAllocator");
		HRESULT hr = D3D12MA::CreateAllocator(&allocator_desc, &m_allocator);
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12MA Allocator");
	}

	render_device::~render_device()
	{
		// release all before live objects reporting :)
		// explicit to control order (most if not all are actually ComPtr/unique_ptr)
		m_graphics_queue.reset();
		m_compute_queue.reset();
		m_copy_queue.reset();
		m_dxgi_factory.Reset();

		m_rtv_descriptor_heap.reset();
		m_dsv_descriptor_heap.reset();
		m_srv_uav_cbv_descriptor_heap.reset();
		m_sampler_descriptor_heap.reset();

		m_allocator->Release();
		m_api_device_handle.Reset();

#if defined(_DEBUG)
		ComPtr<IDXGIDebug1> dxgi_debug;

		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_debug))))
		{
			dxgi_debug->ReportLiveObjects( DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_ALL) );
		}
		else 
		{
			VEER_LOG_ERROR("Failed to get debug interface");
		}
#endif // defined(_DEBUG)
	}

	copy_command_queue& render_device::get_copy_command_queue()
	{
		return *m_copy_queue.get();
	}

	compute_command_queue& render_device::get_compute_command_queue()
	{
		return *m_compute_queue.get();
	}

	graphics_command_queue& render_device::get_graphics_command_queue()
	{
		return *m_graphics_queue.get();
	}



	unique_ptr<swap_chain> render_device::alloc_internal(veer::display::window::window& _window)
	{
		return unique_ptr<dx12_swap_chain>::make(*this, _window);
	}

	unique_ptr<graphics_technique> render_device::alloc_internal(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state)
	{
		return unique_ptr<dx12_graphics_technique>::make(*this, _source_code, _signature, _render_state);
	}

	unique_ptr<compute_technique> render_device::alloc_internal(const shader_stage_source_container_t& _source_code)
	{
		return unique_ptr<dx12_compute_technique>::make(*this, _source_code);
	}


	void render_device::check_errors() const
	{
#if defined(_DEBUG)

		HRESULT deviceRemoved = get_api_handle()->GetDeviceRemovedReason();
		VEER_ASSERT(deviceRemoved == S_OK, "Device has been removed");

		if( m_info_queue == nullptr )
			return;

		size_t count = m_info_queue->GetNumStoredMessages(DXGI_DEBUG_ALL);

		containers::resizable_array<char> buffer;

		size_t message_length = 0u;
		for( UINT64 i = 0; i < count; ++i )
		{
			m_info_queue->GetMessage(DXGI_DEBUG_ALL, i, NULL, &message_length);

			buffer.resize(message_length);
			DXGI_INFO_QUEUE_MESSAGE* message = (DXGI_INFO_QUEUE_MESSAGE*)(buffer.data());
			m_info_queue->GetMessage(DXGI_DEBUG_ALL, i, message, &message_length);

			if( message == nullptr )
			{
				VEER_ASSERT(false, "Validation message could not be retrieved");
				continue;
			}

			switch( message->Severity )
			{
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
			{
				VEER_LOG_ERROR("[D3D12 ERROR] (" << (message->ID) << "): " << message->pDescription);
			}
			break;

			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
			{
				VEER_LOG_WARNING("[D3D12 WARNING] (" << (message->ID) << "): " << message->pDescription);
			}
			break;

			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
			case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
			{
				VEER_LOG_DEBUG("[D3D12 INFO] (" << (message->ID) << "): " << message->pDescription);
			}
			break;
			}
		}
		size_t missed_messages = m_info_queue->GetNumMessagesDiscardedByMessageCountLimit(DXGI_DEBUG_ALL);
		if( missed_messages > 0u )
		{
			VEER_LOG_ERROR( "[D3D12] " << missed_messages << " additional validation messages were received but could not be stored in the queue.");
		}
		m_info_queue->ClearStoredMessages(DXGI_DEBUG_ALL);

	#if 0
		{
			D3D12MA::Budget localBudget;
			m_allocator->GetBudget(&localBudget, NULL);
			
			VEER_LOG(
				"My GPU memory currently has " << localBudget.Stats.AllocationCount << " allocations taking " << localBudget.Stats.AllocationBytes << " B,"
				"allocated out of " << localBudget.Stats.BlockCount << " D3D12 memory heaps taking " << localBudget.Stats.BlockBytes << " B,"
				"D3D12 reports total usage " << localBudget.UsageBytes << " B with budget " << localBudget.BudgetBytes << " B.\n"
			);
		}
	#endif // 0
#endif
	}


	ComPtr<ID3D12Device2> render_device::get_api_handle() const
	{
		return m_api_device_handle;
	}

	D3D12MA::Allocator* render_device::get_allocator() const
	{
		VEER_LOG("get_allocator");
		return m_allocator;
	}

	ComPtr<IDXGIFactory4> render_device::get_dxgi_factory() const
	{
		return m_dxgi_factory;
	}

	dx12_descriptor_heap& render_device::get_rtv_descriptor_heap() const
	{
		return *m_rtv_descriptor_heap;
	}

	dx12_descriptor_heap& render_device::get_dsv_descriptor_heap() const
	{
		return *m_dsv_descriptor_heap;
	}

	dx12_descriptor_heap& render_device::get_srv_uav_cbv_descriptor_heap() const
	{
		return *m_srv_uav_cbv_descriptor_heap;
	}

	dx12_descriptor_heap& render_device::get_sampler_descriptor_heap() const
	{
		return *m_sampler_descriptor_heap;
	}
}
