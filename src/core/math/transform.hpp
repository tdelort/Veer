#pragma once

#include "transform.h"

namespace veer::math
{
    // ! IMPORTANT !
    // All of these are the transpose of what you actually find on wikipedia or most tutorials
    // Most resources are usually in column vector form.
    // > Example for a translation in 2D in column vector form :
    //  |  1  0 tx |   | x |   | x+tx |
    //  |  0  1 ty | * | y |   | y+ty |
    //  |  0  0  1 |   | 1 | = |  1   |
    // But I use row vectors, meaning operations are from left to right (yay), and it helps with interop with slang
    // > Example for a translation in 2D in row vector form :
    //             |  1  0  0 |
    //           * |  0  1  0 |
    // [x, y, 1]   | tx ty  1 | = [x+tx, y+ty, 1]

    // 2D rotations :
    [[nodiscard]] constexpr mat2x2f transform::s_rotation_2d(float _s)
    {
        return s_rotation_2d(vec2f(std::cos(_s), std::sin(_s)));
    }

    [[nodiscard]] constexpr mat2x2f transform::s_rotation_2d(const vec2f& _cos_sin)
    {
        // |  c  s |
        // | -s  c |
        vec2f r0{_cos_sin.x(), _cos_sin.y()};
        vec2f r1{-_cos_sin.y(), _cos_sin.x()};
        return mat2x2f(r0, r1);
    }

    // 3D rotations :
    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d(const vec3f& _euler)
    {
        return transform::s_rotation_3d_x(_euler.x()) * transform::s_rotation_3d_y(_euler.y()) *
               transform::s_rotation_3d_z(_euler.z());
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_x(float _s)
    {
        return transform::s_rotation_3d_x(vec2f(std::cos(_s), std::sin(_s)));
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_x(const vec2f& _cos_sin)
    {
        // |  1  0  0 |
        // |  0  c  s |
        // |  0 -s  c |

        vec3f r0{1.f, 0.f, 0.f};
        vec3f r1{0.f, _cos_sin.x(), _cos_sin.y()};
        vec3f r2{0.f, -_cos_sin.y(), _cos_sin.x()};
        return mat3x3f(r0, r1, r2);
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_y(float _s)
    {
        return transform::s_rotation_3d_y(vec2f(std::cos(_s), std::sin(_s)));
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_y(const vec2f& _cos_sin)
    {
        // |  c  0 -s |
        // |  0  1  0 |
        // |  s  0  c |
        vec3f r0{_cos_sin.x(), 0.f, -_cos_sin.y()};
        vec3f r1{0.f, 1.f, 0.f};
        vec3f r2{_cos_sin.y(), 0.f, _cos_sin.x()};
        return mat3x3f(r0, r1, r2);
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_z(float _s)
    {
        return transform::s_rotation_3d_z(vec2f(std::cos(_s), std::sin(_s)));
    }

    [[nodiscard]] constexpr mat3x3f transform::s_rotation_3d_z(const vec2f& _cos_sin)
    {
        // |  c  s  0 |
        // | -s  c  0 |
        // |  0  0  1 |
        vec3f r0{_cos_sin.x(), _cos_sin.y(), 0.f};
        vec3f r1{-_cos_sin.y(), _cos_sin.x(), 0.f};
        vec3f r2{0.f, 0.f, 1.f};
        return mat3x3f(r0, r1, r2);
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> transform::s_translation_2d(const vec<TYPE, 2u>& _pos)
    {
        // |  1  0  0 |
        // |  0  1  0 |
        // |  x  y  1 |
        mat<TYPE, 3u, 3u> result = mat<TYPE, 3u, 3u>::identity();
        result[2u].set(_pos);
        return result;
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 4u, 4u> transform::s_translation_3d(const vec<TYPE, 3u>& _pos)
    {
        // |  1  0  0  0 |
        // |  0  1  0  0 |
        // |  0  0  1  0 |
        // |  x  y  z  1 |
        mat<TYPE, 4u, 4u> result = mat<TYPE, 4u, 4u>::identity();
        result[3u].set(_pos);
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ELEM_COUNT, ELEM_COUNT> transform::s_scale(const vec<TYPE, ELEM_COUNT>& _scale)
    {
        mat<TYPE, ELEM_COUNT, ELEM_COUNT> result{};
        for (size_t i = 0u; i < ELEM_COUNT; ++i)
            result[i][i] = _scale[i];
        return result;
    }
} // namespace veer::math