#include "constant_buffer.h"

#include <display/render/render_device_buffer.h>
#include <display/render/render_device_resource.h>
#include <display/render/render_device_texture_base.h>
#include <display/render/render_thread.h>

namespace veer::display::render
{
    // constant_buffer_definition

    const buffer_elem_info& constant_buffer_definition::get_elem_info(const char* _name) const
    {
        std::function<bool(const constant_buffer_elem_info&)> predicate =
            [&_name](const constant_buffer_elem_info& _elem) { return std::strcmp(_elem.m_name, _name) == 0; };

        containers::resizable_array<constant_buffer_elem_info>::const_iterator it =
            veer::containers::find_if(m_elements.cbegin(), m_elements.cend(), predicate);

        VEER_ASSERT(it != m_elements.cend(), "Param not found");

        return it->m_elem_info;
    }

    // constant_buffer::update_context

    constant_buffer::update_context::update_context(constant_buffer& _constant_buffer, render_thread& _render_thread)
        : m_constant_buffer(_constant_buffer)
        , m_render_thread(_render_thread)
    {
    }

    constant_buffer::update_context::~update_context()
    {
        const uint64_t current_frame_index = m_render_thread.get_frame_index();

        m_constant_buffer.m_live_buffer_pool.push_back(std::move(m_constant_buffer.m_current_buffer));

        {
            // First, try to reclaim live buffers
            for (
                auto it = m_constant_buffer.m_live_buffer_pool.begin();
                it != m_constant_buffer.m_live_buffer_pool.end();
            )
            {
                // If filled in frame N, we don't want to use them during frame N, nor frame N+1
                if (current_frame_index >= (it->m_frame_index + 2))
                {
                    // reclaim
                    m_constant_buffer.m_free_buffer_pool.push_back(std::move(it->m_buffer_ptr));
                    it = m_constant_buffer.m_live_buffer_pool.erase(it);
                }
                else
                {
                    // next
                    ++it;
                }
            }
        }

        // then we init our new internal buffer
        unique_ptr<render_device_buffer> buffer{nullptr};
        {
            if (m_constant_buffer.m_free_buffer_pool.empty())
            {
                // Nothing in free list, alloc new elem
                buffer = unique_ptr<render_device_buffer>::make(
                    m_constant_buffer.m_device, m_constant_buffer.get_internal_buffer_desc()
                );

                buffer->upload(render_device_resource::upload_flags::dirty_alloc);
            }
            else
            {
                unique_ptr<render_device_buffer>& recycled_elem = m_constant_buffer.m_free_buffer_pool.back();
                buffer = std::move(recycled_elem);
                m_constant_buffer.m_free_buffer_pool.pop_back();
            }
        }

        // TODO : I could probably use the render_device_resource internal m_data buffer (provided I move all code above
        // in the ctor)
        {
            render_device_buffer::memory_mapping mapping(*buffer);
            memcpy(mapping.ptr(), m_constant_buffer.m_cpu_buffer.data(), m_constant_buffer.size());
        }

        m_constant_buffer.m_current_buffer =
            live_buffer{.m_buffer_ptr = std::move(buffer), .m_frame_index = current_frame_index};
    }

    // set_constant in .hpp

    // vec2u64
    using descriptor_handle_t = math::vec<display::render::render_device_resource::bindless_id, 2u>;

    void constant_buffer::update_context::set_texture_read_only(
        const buffer_elem_info& _elem, const render_device_texture_base& _texture
    )
    {
        set_resource(_elem, _texture, render_device_resource_heap_type::srv);
    }

    void constant_buffer::update_context::set_texture_read_write(
        const buffer_elem_info& _elem, const render_device_texture_base& _texture
    )
    {
        set_resource(_elem, _texture, render_device_resource_heap_type::uav);
    }

    void constant_buffer::update_context::set_buffer_read_only(
        const buffer_elem_info& _elem, const render_device_buffer& _buffer
    )
    {
        set_resource(_elem, _buffer, render_device_resource_heap_type::srv);
    }

    void constant_buffer::update_context::set_buffer_read_write(
        const buffer_elem_info& _elem, const render_device_buffer& _buffer
    )
    {
        set_resource(_elem, _buffer, render_device_resource_heap_type::uav);
    }

    void constant_buffer::update_context::set_resource(
        const buffer_elem_info& _elem, const render_device_resource& _resource, render_device_resource_heap_type _type
    )
    {
        const render_device_resource::bindless_id srv_id = _resource.get_bindless_id(_type);
        const render_device_resource::bindless_id sampler_id = render_device_resource::s_invalid_bindless_id;
        const descriptor_handle_t descriptor_handle(srv_id, sampler_id);

        set_constant(_elem, descriptor_handle);
    }

    // constant_buffer

    constant_buffer::constant_buffer(render_device& _device, const constant_buffer_definition& _def)
        : m_device{_device}
        , m_definition{_def}
    {
        size_t size = 0u;
        for (const constant_buffer_elem_info& _constant_buffer_elem_info : m_definition.m_elements)
        {
            size = math::max(
                size,
                _constant_buffer_elem_info.m_elem_info.get_offset() + _constant_buffer_elem_info.m_elem_info.get_size()
            );
        }

        // This is the only change in size on this buffer
        m_cpu_buffer.resize(size, byte_t(0));
    }

    const constant_buffer_definition& constant_buffer::get_def() const
    {
        return m_definition;
    }

    render_device_buffer* constant_buffer::get_internal_buffer() const
    {
        return m_current_buffer.m_buffer_ptr.get();
    }

    size_t constant_buffer::size() const
    {
        return m_cpu_buffer.size();
    }

    buffer_desc constant_buffer::get_internal_buffer_desc() const
    {
        return display::render::buffer_desc{
            .m_flags = display::render::buffer_desc::usage_flags::constant,
            .m_size = 1u,       // one elem ...
            .m_stride = size(), // ... of size : size() bytes
        };
    }
} // namespace veer::display::render
