#pragma once

#include "render_device_texture_base.h"
#include "resource_desc.h"

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
#include <display/render/backends/dx12/dx12_pch.h>

#endif

namespace veer::display::render
{
    class render_device_texture_3d : public render_device_texture_base
    {
    public:
        render_device_texture_3d(render_device& _device, const texture_3d_desc& _desc, const char* _debug_name = "render_device_texture_3d");
        ~render_device_texture_3d() override;

        const texture_3d_desc& get_desc() const
        {
            return m_desc;
        }
        void set_desc(const texture_3d_desc& _desc)
        {
            if (_desc == m_desc)
                return;

            m_desc = _desc;
            flags::set(m_upload_flags, upload_flags::dirty_alloc);
        }

        bindless_id get_bindless_id(render_device_resource_heap_type _heap_type) const override;

    private:
        texture_3d_desc m_desc{};

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_texture_3d.inl"
#elif defined(VULKAN_RENDER_BACKEND)
#error not implemented
#elif defined(METAL_RENDER_BACKEND)
#error not implemented
#endif
    };
} // namespace veer::display::render