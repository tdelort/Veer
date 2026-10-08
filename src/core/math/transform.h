#pragma once

#include <core/math/mat.h>

namespace veer::math
{
    // transform :3
    class transform
    {
    public:
        transform();
        transform(vec3f _translation, vec3f _rotation_euler, vec3f _scale);

        void set_translation(vec3f _new_pos);
        const vec3f& get_translation() const;

        void set_rotation(vec3f _new_rotation_euler);
        const vec3f& get_rotation() const;

        void set_scale(vec3f _new_scale);
        const vec3f& get_scale() const;

        mat4x4f generate_matrix() const;
        mat4x4f generate_inverse_matrix() const;
        // TODO : maybe do this if needed
        // transform inverse() const;

    public:
        // Transform matrix toolkit
        [[nodiscard]] static constexpr mat2x2f s_rotation_2d(float _s);
        [[nodiscard]] static constexpr mat2x2f s_rotation_2d(const vec2f& _cos_sin);

        [[nodiscard]] static constexpr mat3x3f s_rotation_3d(const vec3f& _euler);

        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_x(float _s);
        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_x(const vec2f& _cos_sin);

        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_y(float _s);
        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_y(const vec2f& _cos_sin);

        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_z(float _s);
        [[nodiscard]] static constexpr mat3x3f s_rotation_3d_z(const vec2f& _cos_sin);

        template <Arithmetic TYPE>
        [[nodiscard]] static constexpr mat<TYPE, 3u, 3u> s_translation_2d(const vec<TYPE, 2u>& _pos);

        template <Arithmetic TYPE>
        [[nodiscard]] static constexpr mat<TYPE, 4u, 4u> s_translation_3d(const vec<TYPE, 3u>& _pos);

        template <Arithmetic TYPE, size_t ELEM_COUNT>
        [[nodiscard]] static constexpr mat<TYPE, ELEM_COUNT, ELEM_COUNT> s_scale(const vec<TYPE, ELEM_COUNT>& _scale);

    private:
        // mat3x4f m_matrix;
        // I prefer storing it as 3 vectors since it makes it way easier to change them
        vec3f m_translation{};
        // TODO : maybe use quaternions here ?
        vec3f m_rotation_euler{};
        vec3f m_scale{1.f, 1.f, 1.f};
    };
} // namespace veer::math

#include "transform.hpp"
