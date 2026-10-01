#pragma once

#include <core/containers/resizable_array.h>
#include <display/render/base_types.h>

namespace veer::display::render
{
    enum class constant_buffer_type
    {
        frame = 0,
        material = 1,

        COUNT
    };

    struct constant_buffer_elem_info
    {
        const char* m_name;
        buffer_elem_info m_elem_info;

        bool operator==(const constant_buffer_elem_info& _other) const
        {
            return std::strcmp(m_name, _other.m_name) == 0 && ( m_elem_info == _other.m_elem_info );
        }
    };

    struct constant_buffer_definition
    {
        containers::resizable_array<constant_buffer_elem_info> m_elements;

        const buffer_elem_info& get_elem_info(const char* _name) const;

        bool operator==(const constant_buffer_definition& _other) const
        {
            if (m_elements.size() != _other.m_elements.size())
                return false;

            for(size_t i = 0; i < m_elements.size(); ++i)
            {
                const bool same = m_elements[i] == _other.m_elements[i];
                if (!same)
                    return false;
            }
            return true;
        }
    };
} // namespace veer::display::render