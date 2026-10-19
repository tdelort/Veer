#pragma once

#include "span.h"

namespace veer::containers
{
	template<typename T>
	constexpr span<T>::span()
		: m_data{ nullptr }, m_size{ 0 }
	{
	}

	template<typename T>
	constexpr span<T>::span(T& _data)
		: m_data{ &_data }, m_size{ 1u }
	{

	}

	template<typename T>
	constexpr span<T>::span(T* _data, size_t _count)
		: m_data{ _data }, m_size{ _count }
	{

	}

	template<typename T>
	template<typename COLLECTION_TYPE>
	constexpr span<T>::span(COLLECTION_TYPE& _collection)
		: span<T>( _collection.data(), _collection.size() )
	{

	}

}