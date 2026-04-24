#pragma once

#include <core/core.h>

namespace veer::display::render
{
    // TODO move to its own file 
    struct blend_state
    {
        enum class factor
        {
            zero = 0,
            one,
            src_color, 
            one_minus_src_color, 
            dst_color, 
            one_minus_dst_color, 

            src_alpha, 
            one_minus_src_alpha, 
            dst_alpha, 
            one_minus_dst_alpha, 

            constant_color,
            one_minus_constant_color,
            constant_alpha,
            one_minus_constant_alpha,

            src_alpha_saturate, 

            src1_color, 
            one_minus_src1_color, 
            src1_alpha, 
            one_minus_src1_alpha, 

            COUNT
        };

        enum class operation
        {
            add = 0,
            subtract,
            reverse_subtract,
            min,
            max,

            COUNT
        };

        enum class write_mask : uint8_t
        {
            r = 1 << 0,
            g = 1 << 1,
            b = 1 << 2,
            a = 1 << 3,

            none = 0,
            all = r | g | b | a,
        };

        bool m_enable;

        factor m_src_color_blend_factor{ factor::one };
        factor m_dst_color_blend_factor{ factor::zero };
        operation m_color_blend_operation{ operation::add };

        factor m_src_alpha_blend_factor{ factor::one };
        factor m_dst_alpha_blend_factor{ factor::zero };
        operation m_alpha_blend_operation{ operation::add };

        write_mask m_write_mask{ write_mask::all };
    };

    VEER_ENUM_CLASS_FLAG_OPERATORS( blend_state::write_mask );
}