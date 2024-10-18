#ifndef CORE_SERVICE_H_INCLUDED
#define CORE_SERVICE_H_INCLUDED

#include <typeindex>

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
		std::type_index get_type_id() const
		{
			return std::type_index(typeid(T));
		}
	};
}

#endif // CORE_SERVICE_H_INCLUDED