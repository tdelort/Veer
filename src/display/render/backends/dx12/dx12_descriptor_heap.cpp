#include "dx12_descriptor_heap.h"
#include "dx12_render_device.h"

#include <core/core.h>
#include <core/debug.h>

namespace veer
{
    dx12_descriptor_heap::dx12_descriptor_heap(dx12_render_device& _device, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags, size_t _size)
    {
		// Create descriptor heaps.
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
        rtv_heap_desc.NumDescriptors = (UINT)_size;
        rtv_heap_desc.Type = _type;
        rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		HRESULT hr = _device.get_api_handle()->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_api_handle));
		VEER_ASSERT(SUCCEEDED(hr), "Failed to create RTV descriptor heap (" << hr << ")");

        m_first_available_descriptor_index = 0u;
        m_increment_size = _device.get_api_handle()->GetDescriptorHandleIncrementSize( _type );
        m_size = _size;
    }

    dx12_descriptor_heap::~dx12_descriptor_heap()
    {
        m_api_handle.Reset();
    }

    dx12_descriptor dx12_descriptor_heap::acquire_descriptor()
    {
        size_t index = m_first_available_descriptor_index;

        if (!m_free_indices.empty())
        {
            index = m_free_indices.back();
            m_free_indices.pop_back();
        }

        m_first_available_descriptor_index = std::max( index + 1, m_first_available_descriptor_index );

		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_api_handle->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_increment_size * index;

        return dx12_descriptor(handle, index);
    }

    void dx12_descriptor_heap::release_descriptor(dx12_descriptor _descriptor)
    {
        size_t index = _descriptor.m_index;

        m_free_indices.push_back(index);
    }
}
