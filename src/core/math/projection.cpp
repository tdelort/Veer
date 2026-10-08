#include "projection.h"

#include <core/math/mat.h>

namespace veer::math
{
    constexpr mat4x4f projection::s_perspective(float _vertical_fov, float _aspect, float _near, float _far)
    {
        return s_perspective(vec2f(_vertical_fov, _vertical_fov * _aspect), _near, _far);
    }

    constexpr mat4x4f projection::s_perspective(const vec2f& _fov, float _near, float _far)
    {
        const float tan_half_horizontal_fov = std::tan(0.5f * _fov.x());
        const float tan_half_vertical_fov = std::tan(0.5f * _fov.y());
        const float minus_one_over_range = 1.f / (_near - _far);
        const vec4f r0{1.f / tan_half_horizontal_fov, 0.f, 0.f, 0.f};
        const vec4f r1{0.f, 1.f / tan_half_vertical_fov, 0.f, 0.f};
        const vec4f r2{0.f, 0.f, (_far + _near) * minus_one_over_range, -1.f};
        const vec4f r3{0.f, 0.f, 2.f * _far * _near * minus_one_over_range, 0.f};
        return mat4x4f(r0, r1, r2, r3);
    }

    constexpr mat4x4f projection::s_orthographic(const vec3f& _min, const vec3f& _max)
    {
        const float one_over_width = 1.f / (_max.x() - _min.x());
        const float one_over_height = 1.f / (_max.y() - _min.y());
        const float one_over_z_range = 1.f / (_max.z() - _min.z());
        const vec4f r0{2.f * one_over_width, 0.f, 0.f, 0.f};
        const vec4f r1{0.f, 2.f * one_over_height, 0.f, 0.f};
        const vec4f r2{0.f, 0.f, -2.f * one_over_z_range, 0.f};
        const vec4f r3{
            -(_min.x() + _max.x()) * one_over_width, -(_min.y() + _max.y()) * one_over_height,
            -(_min.z() + _max.z()) * one_over_z_range, 1.f
        };
        return mat4x4f(r0, r1, r2, r3);
    }
} // namespace veer::math