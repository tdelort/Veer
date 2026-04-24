#pragma once

#include <core/core.h>

namespace veer
{
	class service_interface
	{
	public:
		virtual ~service_interface() = default;

		virtual std::type_index get_type_id() const = 0;
	};

	template<typename T>
	class service_generic : public T
	{
	public:
		virtual ~service_generic() {}

		std::type_index get_type_id() const
		{
			return std::type_index(typeid(T));
		}
	};
}