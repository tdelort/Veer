#ifndef CORE_CONTAINER_SPAN_HPP_INCLUDED
#define CORE_CONTAINER_SPAN_HPP_INCLUDED

#include "span.h"

namespace veer
{
	template<typename T>
	span<T>::span()
		: m_data{ nullptr }, m_size{ 0 }
	{
	}

	template<typename T>
	span<T>::span(T* _data)
		: m_data{ _data }, m_size{ 1u }
	{

	}

	template<typename T>
	span<T>::span(T* _data, size_t _count)
		: m_data{ _data }, m_size{ _count }
	{

	}

}
#endif // CORE_CONTAINER_SPAN_HPP_INCLUDED
