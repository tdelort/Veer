#pragma once

#include <core/freelist.h>
#include <core/unique_ptr.h>
#include <display/render/constant_buffer_definition.h>
#include <display/render/render_data_type.h>
#include <display/render/render_device_resource.h>


namespace veer::display::render
{
    class command_buffer;
    class render_thread;
    class render_device_buffer;
    class render_device_texture_base;

    class constant_buffer
    {
    public:
        struct update_context
        {
            update_context(constant_buffer& _constant_buffer, render_thread& _render_thread);
            ~update_context();

            template <RenderDataType T>
            void set_constant(const buffer_elem_info& _elem, T _val);
            template <RenderDataType T, size_t ELEM_COUNT>
            void set_constant(const buffer_elem_info& _elem, math::vec<T, ELEM_COUNT> _val)
                requires veer::LessEqual<ELEM_COUNT, 4>;
            void set_texture_read_only(const buffer_elem_info& _elem, const render_device_texture_base& _texture);
            void set_texture_read_write(const buffer_elem_info& _elem, const render_device_texture_base& _texture);
            void set_buffer_read_only(const buffer_elem_info& _elem, const render_device_buffer& _buffer);
            void set_buffer_read_write(const buffer_elem_info& _elem, const render_device_buffer& _buffer);

        private:
            void set_resource(
                const buffer_elem_info& _elem, const render_device_resource& _resource,
                render_device_resource_heap_type _type
            );

        private:
            constant_buffer& m_constant_buffer;
            render_thread& m_render_thread;
        };

    public:
        constant_buffer(render_device& _device, const constant_buffer_definition& _def, const char* _debug_name = "constant_buffer");

    public:
        const constant_buffer_definition& get_def() const;
        render_device_buffer* get_internal_buffer() const;
        size_t size() const;

    private:
        buffer_desc get_internal_buffer_desc() const;

    private:
        render_device& m_device;
        constant_buffer_definition m_definition;

        // m_cpu_buffer.size() == m_gpu_buffer.size_in_bytes()
        containers::resizable_array<byte_t> m_cpu_buffer;

        struct live_buffer
        {
            unique_ptr<render_device_buffer> m_buffer_ptr{nullptr};
            uint64_t m_frame_index{0u};
        };

        live_buffer m_current_buffer;

        containers::resizable_array<unique_ptr<render_device_buffer>> m_free_buffer_pool;
        containers::resizable_array<live_buffer> m_live_buffer_pool;
        containers::string m_debug_name;
    };

} // namespace veer::display::render

#include "constant_buffer.hpp"