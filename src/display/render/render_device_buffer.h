#pragma once

#include "core/containers/span.h"
#include "resource_desc.h"
#include "render_device_resource.h"

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
#endif 

namespace veer::display::render
{
    class render_device;

    class render_device_buffer : public render_device_resource
    {
    public:
        struct memory_mapping
        {
            memory_mapping(render_device_buffer& _buffer)
                : m_buffer{_buffer}, m_ptr{nullptr}
            {
                m_ptr = _buffer.map();
                VEER_ASSERT(m_ptr != nullptr, "Failed to Map buffer");
            }

            byte_t* ptr() { return m_ptr; }

            ~memory_mapping()
            {
                m_buffer.unmap();
            }
        private:
            render_device_buffer& m_buffer;
            byte_t* m_ptr;
        };

    public:
        render_device_buffer(render_device& _device, const buffer_desc& _desc);
        virtual ~render_device_buffer();

        const buffer_desc& desc() const { return m_desc; }
		
		bindless_id get_bindless_id(render_device_resource_heap_type _heap_type) const override;
	public:
		virtual void upload(copy_command_buffer& _upload_command_buffer, upload_flags _upload_flags) override;

    private:
        byte_t* map();
        void unmap();

    private:
        buffer_desc m_desc;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_buffer.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_buffer.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_buffer.inl"
#endif 
    };
}