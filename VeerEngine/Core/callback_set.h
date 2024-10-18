#ifndef CORE_CALLBACK_SET_H_INCLUDED
#define CORE_CALLBACK_SET_H_INCLUDED

#include "Core/core.h"
#include <functional>

namespace veer
{
	template<typename ...Args>
	class callback_set
	{
		using callback_type = std::function<void(Args...)>;
	public:
		void operator+=(callback_type _callback);
		void operator-=(callback_type _callback);
		void operator()( Args... _args);

		void add(callback_type _callback);
		bool remove(callback_type _callback);
		void call( Args... _args );

		void clear();

		size_t size() const;

	private:
		std::vector<callback_type> m_callbacks;
	};
}

#include "callback_set.hpp"

#endif // CORE_CALLBACK_SET_H_INCLUDED
