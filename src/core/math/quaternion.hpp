
#ifndef CORE_MATH_QUATERNION_HPP_INCLUDED
#define CORE_MATH_QUATERNION_HPP_INCLUDED

#include "quaternion.h"
#include "vec.h"

#include <cmath>

namespace veer
{
	quaternion::quaternion()
		: base_type{ 0.f, 0.f, 0.f, 1.f }
	{
	}

	vec<float, 3> quaternion::to_euler() const
	{
		const float x = m_data[0];
		const float y = m_data[1];
		const float z = m_data[2];
		const float w = m_data[3];
		const float two_x2 = 2.0f * x * x;
		const float two_y2 = 2.0f * y * y;
		const float two_z2 = 2.0f * z * z;
		const vec<float, 3> result(
			atan2( 2.0f * ( w * x + y * z ), 1.0f - two_x2 + two_y2 ),
			asin( 2.0f * ( w * y - z * x ) ),
			atan2( 2.0f * ( w * z + x * y ), 1.0f - two_y2 + two_z2 )
		);
		return result;
	}

	[[nodiscard]] quaternion quaternion::inverse()
	{
		const float sum_of_squares = sq_length( *this );
		return quaternion(
			-m_data[0] / ( sum_of_squares ),
			-m_data[1] / ( sum_of_squares ),
			-m_data[2] / ( sum_of_squares ),
			m_data[3] / ( sum_of_squares )
		);
	}

	quaternion& quaternion::operator*=( const quaternion& _rhs )
	{
		const float x = m_data[0];
		const float y = m_data[1];
		const float z = m_data[2];
		const float w = m_data[3];
		const float rhs_x = m_data[0];
		const float rhs_y = m_data[1];
		const float rhs_z = m_data[2];
		const float rhs_w = m_data[3];
		m_data[0] = w * rhs_x + x * rhs_w + y * rhs_z - z * rhs_y;
		m_data[1] = w * rhs_y + y * rhs_w + z * rhs_x - x * rhs_z;
		m_data[2] = w * rhs_z + z * rhs_w + x * rhs_y - y * rhs_x;
		m_data[3] = w * rhs_w - x * rhs_x - y * rhs_y - z * rhs_z;
		return *this;
	}

	quaternion quaternion::operator*( const quaternion& _rhs ) const
	{
		return quaternion( *this ) *= _rhs;
	}
} // namespace veer

#endif // CORE_MATH_QUATERNION_HPP_INCLUDED
