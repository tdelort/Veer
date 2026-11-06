#include "transform.h"

// TODO REMOVE
#include <veer/core/timer.h>

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
        // M = S * R * T
        // When you expand the above by hand, you get :
        //     |         0 |
        // M = |  S * R  0 |
        //     |         0 |
        //     |    p    1 |
        // Which transforms 2 mat4x4 multiplication (128 multiplication) into only 1 mat3x3 (27)

        const mat3x3f scale = transform::s_scale(m_scale);
        const mat3x3f scale_rotation = scale * transform::s_rotation_3d(m_rotation_euler);
        mat4x4f result = mat4x4f::identity();
        result.set(scale_rotation);
        result[3u].set(m_translation);

        return result;
    }

    mat4x4f transform::generate_inverse_matrix() const
    {
        // generate_matrix() returns a matrix M = T * R * S where T : translation, R : rotation, S : scale
        // we are looking for M-1 such that M * M-1 = I <=> S * R * T * M-1 = I
        // thus M-1 = T-1 * R-1 *  S-1
        // When you expand the above by hand, you get :
        //       |                   0 |
        // M-1 = |    R-1 * S-1      0 |
        //       |                   0 |
        //       |  -p * R-1 * S-1   1 |
        // Which transforms 2 mat4x4 multiplication (128 multiplication) into 1 mat3x3 (27) and 1 vec3/mat3x3 (9) multiplication (36 multiplications !!)

        static constexpr vec3f s_epsilon = vec3f(FLT_EPSILON);
        const vec3f sanitized_scale(select(m_scale > s_epsilon, m_scale, s_epsilon));
        const mat3x3f inv_scale = transform::s_scale(vec3f(1.f) / sanitized_scale);
        const mat3x3f inv_scale_rotation = transform::s_rotation_3d_inv(m_rotation_euler) * inv_scale;

        const vec3f scale_and_rotated_position = (-m_translation) * inv_scale_rotation;

        mat4x4f result = mat4x4f::identity();
        result.set(inv_scale_rotation);
        result[3u].set(scale_and_rotated_position);

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