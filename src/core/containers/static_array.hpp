#ifndef CORE_CONTAINER_STATIC_ARRAY_HPP_INCLUDED
#define CORE_CONTAINER_STATIC_ARRAY_HPP_INCLUDED

#include "static_array.h"

namespace veer
{
	template<typename T, size_t N>
	constexpr static_array<T, N>::static_array(const_reference _val /*= T()*/)
	{
		for (size_t i = 0; i < N; ++i)
			m_data[i] = _val;
	}

	template<typename T, size_t N>
	constexpr static_array<T, N>::static_array(std::initializer_list<T> _list)
		: m_data{ _list }
	{
		static_assert( _list.size() == N )
	}

	template<typename T, size_t N>
	static_array<T, N>::static_array(const static_array& _other)
	{
		for (size_t i = 0; i < N; ++i)
			m_data[i] = _other[i];
	}

	template<typename T, size_t N>
	static_array<T, N>& static_array<T, N>::operator=(const static_array& _other)
	{
		for (size_t i = 0; i < N; ++i)
			m_data[i] = _other[i];
		return *this;
	}

	template<typename T, size_t N>
	static_array<T,N>::static_array(static_array&& _other)
	{
		for (size_t i = 0; i < N; ++i)
			m_data[i] = std::move( _other[i] );
	}

	template<typename T, size_t N>
	static_array<T,N>& static_array<T,N>::operator=(static_array&& _other)
	{
		for (size_t i = 0; i < N; ++i)
			m_data[i] = std::move( _other[i] );
		return *this;
	}
}
#endif // CORE_CONTAINER_STATIC_ARRAY_HPP_INCLUDED
