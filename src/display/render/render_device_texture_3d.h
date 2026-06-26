#pragma once

#include "resource_desc.h"
#include "render_device_texture_base.h"

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
#endif 

namespace veer::display::render
{
    class render_device_texture_3d : public render_device_texture_base
    {
    public:
        render_device_texture_3d( render_device& _device, const texture_3d_desc& _desc );
        ~render_device_texture_3d();

        const texture_3d_desc& desc() const { return m_desc; }

    public:
        void upload(copy_command_buffer& _upload_buffer) override;

    private:
        texture_3d_desc m_desc{};

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_texture_3d.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_texture_3d.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_texture_3d.inl"
#endif 
    };
}