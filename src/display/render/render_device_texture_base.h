#pragma once

#include "render_device_resource.h"

namespace veer::display::render
{
    class render_device_texture_base : public render_device_resource
    {
    public:
        render_device_texture_base(render_device& _device);
        virtual ~render_device_texture_base();
		
	public:
		virtual void upload(copy_command_buffer& _upload_command_buffer, upload_flags _upload_flags) override;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_texture_base.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_texture_base.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_texture_base.inl"
#endif 
    };
}