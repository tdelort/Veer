#include "technique.h"

#include <core/containers/resizable_array.h>
#include <core/core.h>

#include <display/render/shader_parameter_id.h>
#include <display/render/shader_parameter_info.h>
#include <display/render/shader_source.h>

namespace veer::display::render
{
    technique::technique(
        const constant_buffer_definition& _frame_constant_buffer_definition,
        const constant_buffer_definition& _material_constant_buffer_definition
    )
    {
        m_constant_buffer_definitions[static_cast<size_t>(constant_buffer_type::frame)] = _frame_constant_buffer_definition;
        m_constant_buffer_definitions[static_cast<size_t>(constant_buffer_type::material)] = _material_constant_buffer_definition;
        static_assert(static_cast<size_t>(constant_buffer_type::COUNT) == 2u, "Update code here");
    }

    const constant_buffer_definition& technique::get_constant_buffer_definition(constant_buffer_type _type) const
    {
        return m_constant_buffer_definitions[static_cast<size_t>(_type)];
    }
} // namespace veer::display::render