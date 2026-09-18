#pragma once

#include <core/core.h>
#include <display/render/shader_parameter_id.h>

namespace veer::display::render
{
    struct shader_parameter_info
    {
        const char* m_name;
        shader_parameter_id m_id;
    };
} // namespace veer::display::render