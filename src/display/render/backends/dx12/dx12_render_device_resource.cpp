#include <display/render/render_device_resource.h>

#include "dx12_pch.h"

#include <display/render/command_buffer.h>
#include <display/render/render_device.h>
#include <display/render/render_device_resource_sync_state.h>

namespace veer::display::render
{
    render_device_resource::render_device_resource(render_device& _device, const char* _debug_name)
        : m_device(_device)
        , m_upload_flags(render_device_resource::upload_flags::dirty_alloc)
        , m_debug_name(_debug_name)
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
        hr = m_device.get_allocator()->CreateResource(
            &default_alloc_desc, &resource_desc, _state, clear_value_arg, &m_resource_alloc, IID_NULL, NULL
        );
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create D3D12 resource. Error (" << hr << ")");

        set_api_handle(m_resource_alloc->GetResource());
    }

    void render_device_resource::upload_data_to_default_heap(
        copy_command_buffer& _upload_buffer, veer::containers::span<const byte_t> _data
    )
    {
        D3D12_RESOURCE_DESC resource_desc = get_resource_desc();

        const size_t first_sub_resource = 0u;
        const size_t sub_resource_count = resource_desc.MipLevels * resource_desc.DepthOrArraySize;

        containers::resizable_array<UINT> num_rows;
        containers::resizable_array<UINT64> row_sizes_in_bytes;
        containers::resizable_array<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> layouts;
        num_rows.resize(sub_resource_count);
        row_sizes_in_bytes.resize(sub_resource_count);
        layouts.resize(sub_resource_count);

        size_t size = 0u;
        m_device.get_api_handle()->GetCopyableFootprints(
            &resource_desc, first_sub_resource, sub_resource_count, 0, layouts.data(), num_rows.data(),
            row_sizes_in_bytes.data(), &size
        );

        D3D12_RESOURCE_DESC upload_resource_desc = {};
        {
            upload_resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            upload_resource_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            upload_resource_desc.Height = 1u;
            upload_resource_desc.DepthOrArraySize = 1u;
            upload_resource_desc.MipLevels = 1u;
            upload_resource_desc.SampleDesc.Count = 1u;
            upload_resource_desc.SampleDesc.Quality = 0u;
            upload_resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            upload_resource_desc.Format = DXGI_FORMAT_UNKNOWN;
            upload_resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

            upload_resource_desc.Width = size;
        }

        D3D12MA::ALLOCATION_DESC upload_alloc_desc = {};
        upload_alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

        D3D12MA::Allocation* upload_alloc;
        VEER_LOG("CreateResource");
        veer::hr hr = m_device.get_allocator()->CreateResource(
            &upload_alloc_desc, &upload_resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, &upload_alloc, IID_NULL,
            NULL
        );
        VEER_ASSERT(hr.succeeded(), "Failed to create upload resource. Error (" << hr << ")");

        // basically UpdateSubresources with less features
        {
            uint8_t* gpu_data;
            upload_alloc->GetResource()->Map(0u, nullptr, reinterpret_cast<void**>(&gpu_data));

            // memcpy(gpu_data, _data.data(), _data.size());
            for (size_t sub_resource_index = 0; sub_resource_index < sub_resource_count; ++sub_resource_index)
            {
                byte_t* dest_ptr = gpu_data + layouts[sub_resource_index].Offset;
                const size_t row_pitch = layouts[sub_resource_index].Footprint.RowPitch;
                const size_t slice_pitch = static_cast<size_t>(layouts[sub_resource_index].Footprint.RowPitch) *
                                     static_cast<size_t>(num_rows[sub_resource_index]);

                for (size_t slice_index = 0; slice_index < layouts[sub_resource_index].Footprint.Depth; ++slice_index)
                {
                    byte_t* dest_slice_ptr = dest_ptr + slice_pitch * slice_index;
                    // TODO : if adding support for multiple subresource, think about how I should manage input data for
                    // multiple sub resources
                    byte_t* src_slice_ptr = m_data.data();

                    const size_t row_size_in_bytes = row_sizes_in_bytes[sub_resource_index];
                    for (size_t row_index = 0; row_index < num_rows[sub_resource_index]; ++row_index)
                    {
                        byte_t* dest = dest_slice_ptr + row_pitch * row_index;
                        // TODO : here I make the assumption that the CPU data is laid out flat
                        byte_t* src = src_slice_ptr + row_size_in_bytes * row_index;
                        memcpy(dest, src, row_size_in_bytes);
                    }
                }
            }

            upload_alloc->GetResource()->Unmap(0u, nullptr);
        }

        // this alloc needs to be released after the upload command buffer is executed and waited for
        _upload_buffer.do_after_execution([upload_alloc]() { upload_alloc->Release(); });

        _upload_buffer.transition_barrier(*this, render_device_resource_sync_state::CopyDest);

        for (size_t sub_resource_index = 0; sub_resource_index < sub_resource_count; ++sub_resource_index)
        {
            const D3D12_TEXTURE_COPY_LOCATION dst{
                .pResource = get_api_handle(),
                .Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
                .SubresourceIndex = static_cast<UINT>(sub_resource_index)
            };

            const D3D12_TEXTURE_COPY_LOCATION src{
                .pResource = upload_alloc->GetResource(),
                .Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
                .PlacedFootprint = layouts[sub_resource_index]
            };

            _upload_buffer.get_api_handle()->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        }

        m_device.check_errors();
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
            {buffer_desc::usage_flags::index,         D3D12_RESOURCE_FLAG_NONE                  },
            {buffer_desc::usage_flags::vertex,        D3D12_RESOURCE_FLAG_NONE                  },
            {buffer_desc::usage_flags::constant,      D3D12_RESOURCE_FLAG_NONE                  },
            {buffer_desc::usage_flags::storage,       D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS},
            {buffer_desc::usage_flags::indirect_args, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS}
        };

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

    D3D12_RESOURCE_FLAGS render_device_resource::s_convert(texture_2d_desc::usage_flags _states)
    {
        std::pair<texture_2d_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = {
            {texture_2d_desc::usage_flags::storage,       D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS},
            {texture_2d_desc::usage_flags::render_target, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET   },
            {texture_2d_desc::usage_flags::depth_stencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL   }
        };

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

    D3D12_RESOURCE_FLAGS render_device_resource::s_convert(texture_3d_desc::usage_flags _states)
    {
        std::pair<texture_3d_desc::usage_flags, D3D12_RESOURCE_FLAGS> s_conversionTable[] = {
            {texture_3d_desc::usage_flags::storage,       D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS},
            {texture_3d_desc::usage_flags::render_target, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET   },
            {texture_3d_desc::usage_flags::depth_stencil, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL   }
        };

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
        {
#if defined(_DEBUG)
            containers::wstring w_debug_name(m_debug_name);
            get_api_handle()->SetName(w_debug_name.c_str());
#endif // defined(_DEBUG)
            m_api_handle->AddRef();
        }
    }
} // namespace veer::display::render