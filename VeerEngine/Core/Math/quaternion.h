#pragma once
#ifndef _QUAT_INC_
#define _QUAT_INC_

#include "vec.h"

namespace veer
{
	/*
	 * x : i axis
	 * y : j axis
	 * z : k axis
	 * w : real part
	 */
	class quaternion : public vec4f
	{
	public:
		using base_type = vec4f;

		quaternion();
		using vec4f::vec4f;

		[[nodiscard]] vec3f to_euler() const;
		[[nodiscard]] quaternion inverse();

		quaternion& operator*=(const quaternion& _rhs);
		quaternion operator*(const quaternion& _rhs) const;
	};
}

#include "quaternion.hpp"
#endif // _QUAT_INC_
