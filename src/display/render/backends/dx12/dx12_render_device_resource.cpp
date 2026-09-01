#include <display/render/render_device_resource.h>

#include "dx12_pch.h"

#include <display/render/command_buffer.h>
#include <display/render/render_device.h>
#include <display/render/render_device_resource_sync_state.h>


namespace veer::display::render
{
    render_device_resource::render_device_resource(render_device& _device)
        : m_device(_device), m_upload_flags(render_device_resource::upload_flags::dirty_alloc)
    {
    }

    render_device_resource::~render_device_resource()
    {
        set_api_handle(nullptr);
        if (m_resource_alloc != nullptr)
        {
            m_resource_alloc->Release();
            m_resource_alloc = nullptr;
        }
    }

    void render_device_resource::alloc(D3D12_HEAP_TYPE _heap, D3D12_RESOURCE_STATES _state)
    {
        set_api_handle(nullptr);
        if (m_resource_alloc != nullptr)
        {
            m_resource_alloc->Release();
            m_resource_alloc = nullptr;
        }

        D3D12_RESOURCE_DESC resource_desc = get_resource_desc();

        HRESULT hr;

        D3D12MA::ALLOCATION_DESC default_alloc_desc = {};
        default_alloc_desc.HeapType = _heap;

        D3D12_CLEAR_VALUE clear_value = {};
        D3D12_CLEAR_VALUE* clear_value_arg = nullptr;
        if ((resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0)
        {
            clear_value.Format = resource_desc.Format;
            clear_value.DepthStencil.Depth = 0.f;
            clear_value.DepthStencil.Stencil = 0;
            clear_value_arg = &clear_value;
        }
        else if ((resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0)
        {
            clear_value.Format = resource_desc.Format;
            clear_value.Color[0] = 0.f;
            clear_value.Color[1] = 0.f;
            clear_value.Color[2] = 0.f;
            clear_value.Color[3] = 0.f;
            clear_value_arg = &clear_value;
        }

        D3D12MA::Allocation* resource_alloc;
        VEER_LOG("CreateResource");
        hr = m_device.get_allocator()->CreateResource(&default_alloc_desc, &resource_desc, _state, clear_value_arg,
                                                      &m_resource_alloc, IID_NULL, NULL);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12 resource. Error (" << hr << ")");

        set_api_handle(m_resource_alloc->GetResource());
    }

    void render_device_resource::upload_data_to_default_heap(copy_command_buffer& _upload_buffer,
                                                             veer::containers::span<const byte_t> _data)
    {
        D3D12_RESOURCE_DESC resource_desc = get_resource_desc();

        HRESULT hr;

        D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
        upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

        D3D12MA::Allocation* upload_alloc;
        VEER_LOG("CreateResource");
        hr = m_device.get_allocator()->CreateResource(
            &upload_alloc_desc, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &upload_alloc, IID_NULL, NULL);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create upload resource. Error (" << hr << ")");

        // this alloc needs to be released after the upload command buffer is executed and waited for
        _upload_buffer.do_after_execution([upload_alloc]() { upload_alloc->Release(); });

        {
            uint8_t* gpu_data;
            upload_alloc->GetResource()->Map(0u, nullptr, reinterpret_cast<void**>(&gpu_data));
            memcpy(gpu_data, _data.data(), _data.size());
            upload_alloc->GetResource()->Unmap(0u, nullptr);
        }

        _upload_buffer.transition_barrier(*this, render_device_resource_sync_state::CopyDest);

        _upload_buffer.get_api_handle()->CopyResource(get_api_handle(), upload_alloc->GetResource());
    }

    void render_device_resource::upload_data_to_upload_heap(veer::containers::span<const byte_t> _data)
    {
        uint8_t* gpu_data;
        get_api_handle()->Map(0u, nullptr, reinterpret_cast<void**>(&gpu_data));
        memcpy(gpu_data, _data.data(), _data.size());
        get_api_handle()->Unmap(0u, nullptr);
    }

    D3D12_RESOURCE_FLAGS render_device_resource::s_convert(buffer_desc::usage_flags _states)
    {
        std::pair<buffer_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = {
            {buffer_desc::usage_flags::index, D3D12_RESOURCE_FLAG_NONE},
            {buffer_desc::usage_flags::vertex, D3D12_RESOURCE_FLAG_NONE},
            {buffer_desc::usage_flags::constant, D3D12_RESOURCE_FLAG_NONE},
            {buffer_desc::usage_flags::storage, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS},
            {buffer_desc::usage_flags::indirect_args, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS}};

        D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;
        for (size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE(s_conversionTable); ++i)
        {
            if (flags::any(s_conversionTable[i].first & _states))
            {
                dx12_flags |= s_conversionTable[i].second;
            }
        }

        return dx12_flags;
    }

    D3D12_RESOURCE_FLAGS render_device_resource::s_convert(texture_desc::usage_flags _states)
    {
        std::pair<texture_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = {
            {texture_desc::usage_flags::storage, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS},
            {texture_desc::usage_flags::render_target, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET},
            {texture_desc::usage_flags::depth_stencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL}};

        D3D12_RESOURCE_FLAGS dx12_flags = D3D12_RESOURCE_FLAG_NONE;
        for (size_t i = 0u; i < VEER_STATIC_ARRAY_SIZE(s_conversionTable); ++i)
        {
            if (flags::any(s_conversionTable[i].first & _states))
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
} // namespace veer::display::render