#pragma once

#include <veer/core/core.h>

#include <veer/display/render/shader_parameter_id.h>

namespace veer::display::render
{
    struct shader_parameter_info
    {
        const char* m_name;
        shader_parameter_id m_id;
    };
} // namespace veer::display::render