#pragma once

#include "quaternion.h"
#include "vec.h"

#include <core/core.h>

namespace veer::math
{
    quaternion::quaternion()
        : base_type{0.f, 0.f, 0.f, 1.f}
    {
    }

    vec<float, 3> quaternion::to_euler() const
    {
        const float two_x2 = 2.0f * x() * x();
        const float two_y2 = 2.0f * y() * y();
        const float two_z2 = 2.0f * z() * z();
        const vec<float, 3> result(
            atan2(2.0f * (w() * x() + y() * z()), 1.0f - two_x2 + two_y2), asin(2.0f * (w() * y() - z() * x())),
            atan2(2.0f * (w() * z() + x() * y()), 1.0f - two_y2 + two_z2)
        );
        return result;
    }

    [[nodiscard]] quaternion quaternion::inverse()
    {
        const float sum_of_squares = sq_length(*this);
        return quaternion(
            -x() / (sum_of_squares), -y() / (sum_of_squares), -z() / (sum_of_squares), w() / (sum_of_squares)
        );
    }

    quaternion& quaternion::operator*=(const quaternion& _rhs)
    {
        x() = w() * _rhs.x() + x() * _rhs.w() + y() * _rhs.z() - z() * _rhs.y();
        y() = w() * _rhs.y() + y() * _rhs.w() + z() * _rhs.x() - x() * _rhs.z();
        z() = w() * _rhs.z() + z() * _rhs.w() + x() * _rhs.y() - y() * _rhs.x();
        w() = w() * _rhs.w() - x() * _rhs.x() - y() * _rhs.y() - z() * _rhs.z();
        return *this;
    }

    quaternion quaternion::operator*(const quaternion& _rhs) const
    {
        return quaternion(*this) *= _rhs;
    }
} // namespace veer::math