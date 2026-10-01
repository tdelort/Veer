#pragma once

#include "core/containers/span.h"
#include "render_device_resource.h"
#include "resource_desc.h"


#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
#include <display/render/backends/dx12/dx12_pch.h>

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
                : m_buffer{_buffer}
                , m_ptr{nullptr}
            {
                m_ptr = _buffer.map();
                VEER_ASSERT(m_ptr != nullptr, "Failed to Map buffer");
            }

            byte_t* ptr()
            {
                return m_ptr;
            }

            ~memory_mapping()
            {
                m_buffer.unmap();
            }

        private:
            render_device_buffer& m_buffer;
            byte_t* m_ptr;
        };

    public:
        render_device_buffer(render_device& _device, const buffer_desc& _desc, const char* _debug_name = "render_device_buffer");
        virtual ~render_device_buffer();

        const buffer_desc& get_desc() const
        {
            return m_desc;
        }
        void set_desc(const buffer_desc& _desc)
        {
            if (_desc == m_desc)
                return;

            m_desc = _desc;
            flags::set(m_upload_flags, upload_flags::dirty_alloc);
        }

        bindless_id get_bindless_id(render_device_resource_heap_type _heap_type) const override;

    public:
        void upload(upload_flags _upload_flags);

    private:
        byte_t* map();
        void unmap();

    private:
        buffer_desc m_desc;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_buffer.inl"
#elif defined(VULKAN_RENDER_BACKEND)
#error not implemented
#elif defined(METAL_RENDER_BACKEND)
#error not implemented
#endif
    };
} // namespace veer::display::render