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
}