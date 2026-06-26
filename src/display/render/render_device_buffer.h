#pragma once

#include "core/containers/resizable_array.h"
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
        render_device_buffer(render_device& _device, const buffer_desc& _desc);
        virtual ~render_device_buffer() override;

        const buffer_desc& desc() const { return m_desc; }

    public:
        void upload(copy_command_buffer& _upload_buffer) override;

    public:
        template<typename T>
        void set_data(containers::span<T> _data);

        template<typename T>
        containers::span<const T> get_data() const;
		
    private:
        containers::resizable_array<byte_t> m_data;
        buffer_desc m_desc;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_buffer.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_buffer.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_buffer.inl"
#endif 
    };



    template<typename T>
    void render_device_buffer::set_data(containers::span<T> _data)
    {
        m_data.clear();
        const size_t new_size = _data.size() * sizeof(T) / sizeof(byte_t); 
        m_data.resize(new_size);
        std::memcpy(m_data.data(), _data.data(), new_size);
    }

    template<typename T>
    containers::span<const T> render_device_buffer::get_data() const
    {
        return containers::span<const T>(static_cast<const T*>(m_data.data()), ( m_data.size() / sizeof(T) ) * sizeof(byte_t) );
    }
}