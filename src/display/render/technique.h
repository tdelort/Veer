#pragma once

#include <core/containers/resizable_array.h>
#include <core/containers/static_array.h>
#include <core/core.h>

#include <display/render/constant_buffer_definition.h>

namespace veer::display::render
{
    class technique
    {
    public:
        technique(
            const constant_buffer_definition& _frame_constant_buffer_definition,
            const constant_buffer_definition& _material_constant_buffer_definition
        );

        const constant_buffer_definition& get_constant_buffer_definition(constant_buffer_type _type) const;

    private:
        using shader_constant_buffer_definitions_t = veer::containers::static_array<
            constant_buffer_definition, static_cast<size_t>(constant_buffer_type::COUNT)>;

        shader_constant_buffer_definitions_t m_constant_buffer_definitions;
    };
} // namespace veer::display::render