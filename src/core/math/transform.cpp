#include "transform.h"

namespace veer::math
{
    transform::transform()
        : m_matrix(mat4x4f::identity())
    {
    }

    transform::transform(vec3f _position, vec3f _rotation, vec3f _scale)
    {
        m_matrix = mat_utils::translation_3d(_position);

        mat4x4f rotation = mat4x4f::identity();
        rotation.set(mat_utils::rotation_3d(_rotation));
        m_matrix = m_matrix * rotation;

        mat4x4f scale = mat4x4f::identity();
        scale.set(mat_utils::scale(_scale));
        m_matrix = m_matrix * scale;
    }
}; // namespace veer::math