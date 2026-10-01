#include "constant_buffer_definition.h"

#include <core/containers/resizable_array.h>
#include <display/render/base_types.h>

namespace veer::display::render
{
    const buffer_elem_info& constant_buffer_definition::get_elem_info(const char* _name) const
    {
        std::function<bool(const constant_buffer_elem_info&)> predicate =
            [&_name](const constant_buffer_elem_info& _elem) { return std::strcmp(_elem.m_name, _name) == 0; };

        containers::resizable_array<constant_buffer_elem_info>::const_iterator it =
            veer::containers::find_if(m_elements.cbegin(), m_elements.cend(), predicate);

        VEER_ASSERT(it != m_elements.cend(), "Param not found");

        return it->m_elem_info;
    }
} // namespace veer::display::render