#pragma once

#include <core/math/mat.h>

namespace veer::math
{
    namespace projection
    {
        mat4x4f s_perspective(float _vertical_fov, float _aspect, float _near, float _far);
        mat4x4f s_orthographic(const vec3f& _min, const vec3f& _max);
    } // namespace projection
} // namespace veer::math