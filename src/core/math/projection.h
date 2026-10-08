#pragma once

#include <core/math/mat.h>

namespace veer::math
{
    namespace projection
    {
        static constexpr mat4x4f s_perspective(float _vertical_fov, float _aspect, float _near, float _far);
        static constexpr mat4x4f s_perspective(const vec2f& _fov, float _near, float _far);
        static constexpr mat4x4f s_orthographic(const vec3f& _min, const vec3f& _max);
    };
}