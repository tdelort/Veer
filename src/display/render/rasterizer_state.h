#pragma once

#include <core/core.h>

namespace veer::display::render
{ 
    struct rasterizer_state
    {
        enum class cull_mode
        {
            none,
            front,
            back,

            COUNT
        };
        
        // enum class winding_mode
        // {
        //     clockwise,
        //     counter_clockwise,
        // };

        bool m_wireframe{false};
        cull_mode m_cull_mode{cull_mode::back};
        // winding_mode m_winding;
    };
}