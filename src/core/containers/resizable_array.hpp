#pragma once

#include "resizable_array.h"

#include <core/debug.h>
#include <core/core.h>

namespace veer::containers
{
	template<typename T>
	resizable_array<T>::resizable_array()
		: m_data{nullptr}, m_size{0u}, m_capacity{0u}
	{

	}

	template<typename T>
	resizable_array<T>::resizable_array(const resizable_array<T>& _other)
	{
		*this = _other; 
	}

	template<typename T>
	resizable_array<T>& resizable_array<T>::operator=(const resizable_array<T>& _other)
	{
		clear(); // also resets m_size to 0

		if ( m_capacity < _other.m_capacity )
			alloc( _other.m_capacity );

		// copy data
		for(size_t i = 0; i < _other.m_size; ++i)
			push_back(_other[i]);

		m_size = _other.m_size;
	}

	template<typename T>
	resizable_array<T>::resizable_array(resizable_array<T>&& _other)
	{
		*this = std::move(_other);
	}

	template<typename T>
	resizable_array<T>& resizable_array<T>::operator=(resizable_array<T>&& _other)
	{
		// clean self
		destroy();

		// steal
		m_capacity = _other.m_capacity;
		m_size = _other.m_size;
		m_data = _other.m_data;

		// clean other
		_other.m_capacity = 0u;
		_other.m_size = 0u;
		_other.m_data = nullptr;

		return *this;
	}

	template<typename T>
	T& resizable_array<T>::back()
	{
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
	}

	template<typename T>
	const T& resizable_array<T>::back() const
	{
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
	}

	template<typename T>
	void resizable_array<T>::pop_back()
	{
		if (empty())
			return;

		const size_t last_index = ( size() - 1 );
		std::destroy_at(m_data + last_index);
		m_size--;
	}

	template<typename T>
	void resizable_array<T>::push_back(const T& _val)
	{
		emplace_back(_val);
	}

	template<typename T>
	void resizable_array<T>::push_back(T&& _val)
	{
		emplace_back(std::forward<T&&>(_val));
	}

	template<typename T>
	template<typename... ARGS>
	resizable_array<T>::reference resizable_array<T>::emplace_back(ARGS&&... _args)
	{
		if ( size() + 1 > capacity() )
			grow();

		T* ptr = new(end()) T(std::forward<ARGS>(_args)...);
		m_size++;

		return *ptr;
	}

	template<typename T>
	resizable_array<T>::iterator resizable_array<T>::erase(iterator _it)
	{
		VEER_ASSERT(_it >= begin() && _it < end() && size() != 0u, "Iterator out of bounds");
		VEER_ASSERT(_it != nullptr, "Iterator is invalid");

		size_t index = ( _it - begin() ) / sizeof(T); 
		std::destroy_at(m_data + index);

		for(size_t i = index; i < size() - 1; ++i)
		{
			m_data[i] = m_data[i + 1];
		}

		m_size--;

		return begin() + index;
	}

	template<typename T>
	void resizable_array<T>::reserve(size_t _new_capacity)
	{
		if (_new_capacity != 0)
			alloc(_new_capacity);
	}

	template<typename T>
	void resizable_array<T>::resize(size_t _new_size, const_reference _new_values /*= T()*/)
	{
		for (size_t i = _new_size; i < m_size; ++i)
			std::destroy_at( m_data + i );

		if (m_capacity < _new_size)
			alloc(_new_size);

		for (size_t i = m_size; i < _new_size; ++i)
			new(m_data + i) T(_new_values);

		m_size = _new_size;
	}


	template<typename T>
	void resizable_array<T>::grow()
	{
		const size_t new_capacity = m_capacity == 0u ? 8u : 2 * m_capacity;
		alloc( new_capacity );
	}

	template<typename T>
	void resizable_array<T>::alloc(size_t _new_capacity)
	{
		VEER_ASSERT(_new_capacity > m_size, "resizable_array::alloc call with new_capacity (" << _new_capacity << ") smaller than size " << m_size << " !");
		T* new_data = static_cast<T*>(std::malloc(_new_capacity * sizeof(T)));

		if ( m_data != nullptr )
		{
			for (size_t i = 0; i < m_size; ++i)
				new_data[i] = std::move(m_data[i]);

			std::free(m_data);
		}

		m_data = new_data; 
		m_capacity = _new_capacity;
	}

	template<typename T>
	void resizable_array<T>::clear()
	{
		for(size_t i = 0; i < m_size; ++i)
			std::destroy_at( m_data + i );

		m_size = 0u;
	}

	template<typename T>
	void resizable_array<T>::destroy()
	{
		clear();

		if ( m_data != nullptr )
			std::free(m_data);
	}
}
