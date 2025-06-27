#ifndef CORE_CALLBACK_SET_HPP_INCLUDED
#define CORE_CALLBACK_SET_HPP_INCLUDED

#include "callback_set.h"

#include <functional>

namespace veer
{
	template<typename ...Args>
	void callback_set<Args...>::operator+=(callback_type _callback)
	{
		add(_callback);
	}

	template<typename ...Args>
	void callback_set<Args...>::operator-=(callback_type _callback)
	{
		remove(_callback);
	}

	template<typename ...Args>
	void callback_set<Args...>::operator()( Args... _args )
	{
		call( _args );
	}

	template<typename ...Args>
	void callback_set<Args...>::add(callback_type _callback)
	{
		m_callbacks.push_back( _callback );
	}

	template<typename ...Args>
	bool callback_set<Args...>::remove(callback_type _callback)
	{
		// TODO
	}

	template<typename ...Args>
	void callback_set<Args...>::call( Args... _args )
	{
		for( callback_type& callback : m_callbacks )
		{
			callback( _args... );
		}
	}

	template<typename ...Args>
	void callback_set<Args...>::clear()
	{
		m_callbacks.clear();
	}

	template<typename ...Args>
	size_t callback_set<Args...>::size() const
	{
		return m_callbacks.size();
	}
}
#endif // CORE_CALLBACK_SET_HPP_INCLUDED
