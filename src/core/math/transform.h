#pragma once


#include <core/math/mat.h>

namespace veer::math
{
    // transform :3
    class transform // : public mat3x4f
    {
    public:
        transform();
        transform(vec3f _position, vec3f _rotation, vec3f _scale);

    private:
        mat4x4f m_matrix;
        // TODO : make this a 3x4 since the last row is just 0 0 0 1
        // mat3x4f m_matrix;
    };
}
