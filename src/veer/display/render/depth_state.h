#pragma once

#include <veer/core/core.h>

#include <veer/display/render/base_types.h>

namespace veer::display::render
{
    struct depth_state
    {
        enum class func
        {
            none,
            never,
            equal,
            not_equal,
            less,
            less_equal,
            greater,
            greater_equal,
            always,

            COUNT,
        };

        access_mode m_access{access_mode::none};
        func m_func{func::none};
    };
} // namespace veer::display::render