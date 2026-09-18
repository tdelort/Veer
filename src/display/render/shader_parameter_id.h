#pragma once

#include <core/core.h>
#include <core/debug.h>
#include <display/render/constant_buffer.h>

namespace veer::display::render
{

#if defined(METAL_RENDER_BACKEND)
#error there will probably be some things to change here
#endif // defined(METAL_RENDER_BACKEND)

    struct shader_parameter_id
    {
        constexpr shader_parameter_id(constant_buffer_type _constant_buffer_type, size_t _offset, size_t _size)
            : m_constant_buffer_type(static_cast<uint32_t>(_constant_buffer_type))
            , m_offset(_offset)
            , m_size(_size)
        {
        }

        constant_buffer_type get_constant_buffer_type()
        {
            return static_cast<constant_buffer_type>(m_constant_buffer_type);
        }

        size_t get_offset()
        {
            return static_cast<size_t>(m_offset);
        }

        size_t get_size()
        {
            return static_cast<size_t>(m_size);
        }

    private:
        uint32_t m_constant_buffer_type : 4;
        uint32_t m_size                 : 8;
        uint32_t m_offset               : 20;
    };
} // namespace veer::display::render