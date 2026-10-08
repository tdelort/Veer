#include "transform.h"

namespace veer::math
{
    transform::transform()
    {
    }

    transform::transform(vec3f _translation, vec3f _rotation_euler, vec3f _scale)
        : m_translation(_translation)
        , m_rotation_euler(_rotation_euler)
        , m_scale(_scale)
    {
    }

    void transform::set_translation(vec3f _new_pos)
    {
        m_translation = _new_pos;
    }

    const vec3f& transform::get_translation() const
    {
        return m_translation;
    }

    void transform::set_rotation(vec3f _new_rotation_euler)
    {
        m_rotation_euler = _new_rotation_euler;
    }

    const vec3f& transform::get_rotation() const
    {
        return m_rotation_euler;
    }

    void transform::set_scale(vec3f _new_scale)
    {
        m_scale = _new_scale;
    }

    const vec3f& transform::get_scale() const
    {
        return m_scale;
    }

    mat4x4f transform::generate_matrix() const
    {
        mat4x4f result = mat4x4f::identity();

        {
            mat4x4f translation = transform::s_translation_3d(m_translation);
            result = result * translation;
        }

        {
            mat4x4f rotation = mat4x4f::identity();
            rotation.set(transform::s_rotation_3d(m_rotation_euler));
            result = result * rotation;
        }


        {
            mat4x4f scale = mat4x4f::identity();
            scale.set(transform::s_scale(m_scale));
            result = result * scale;
        }

        return result;
    }

    mat4x4f transform::generate_inverse_matrix() const
    {
        // generate_matrix() returns a matrix M = T * R * S where T : translation, R : rotation, S : scale
        // we are looking for M-1 such that M * M-1 = I <=> T * R * S * M-1 = I
        // thus M-1 = S-1 * R-1 * T-1

        mat4x4f result = mat4x4f::identity();

        {
            mat4x4f inv_scale = mat4x4f::identity();
            static constexpr vec3f s_epsilon = vec3f(FLT_EPSILON);
            const vec3f sanitized_scale(select(m_scale > s_epsilon, m_scale, s_epsilon));
            inv_scale.set(transform::s_scale(vec3f(1.f) / sanitized_scale));

            result = result * inv_scale;
        }

        {
            mat4x4f inv_rotation = mat4x4f::identity();
            inv_rotation.set(transform::s_rotation_3d(-m_rotation_euler));
            result = result * inv_rotation;
        }

        {
            mat4x4f inv_translation = transform::s_translation_3d(-m_translation);
            result = result * inv_translation;
        }

        return result;
    }

#if 0
    transform transform::inverse() const
    {
        // M-1 = S-1 * R-1 * T-1
        // To get a transform which generates the inverse matrix, we need :
        // T', R', S' such that M' = T' * R' * S' = S-1 * R-1 * T-1 = M-1
        // Which can be satisfied with the following :
        // T' = (S-1 * R-1 * T-1 * R * S) 
        // R' = (S-1 * R-1 * S) <=> R' = (1/s)I R-1 sI
        // S' = (S-1)
    }
#endif // 0
}; // namespace veer::math