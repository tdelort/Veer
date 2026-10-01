#pragma once

#include <core/math/vec.h>

namespace veer::display::render
{
    struct viewport
    {
        math::vec2u m_position{0u, 0u};
        math::vec2u m_size{0u, 0u};
        math::vec2f m_depth{0.f, 1.f};
    };

    struct rect
    {
        math::vec2u m_min{0u, 0u};
        math::vec2u m_max{0u, 0u};
    };

    static constexpr size_t s_max_viewport_and_scissors_count = 16u;
    static constexpr size_t s_max_color_render_targets = 8u;

    struct buffer_elem_info
    {
        constexpr buffer_elem_info()
            : buffer_elem_info(0, 0)
        {
        }

        constexpr buffer_elem_info(size_t _offset, size_t _size)
            : m_offset(_offset)
            , m_size(_size)
        {
        }

        constexpr size_t get_offset() const
        {
            return static_cast<size_t>(m_offset);
        }

        constexpr size_t get_size() const
        {
            return static_cast<size_t>(m_size);
        }

        bool operator==(const buffer_elem_info& _other) const
        {
            return (get_size() == _other.get_size()) && (get_offset() == _other.get_offset());
        }

    private:
        uint32_t m_size   : 8;
        uint32_t m_offset : 24;
    };
} // namespace veer::display::render