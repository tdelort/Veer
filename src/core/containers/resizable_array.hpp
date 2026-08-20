#pragma once

#include "resizable_array.h"

#include <core/math/math.h>
#include <core/debug.h>
#include <core/core.h>

namespace veer::containers
{
	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>::resizable_array()
		: m_data{nullptr}, m_size{0u}, m_capacity{0u}, m_allocator()
	{

	}
	
	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>::~resizable_array()
	{
		destroy();
	}

	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>::resizable_array(const resizable_array<T,ALLOCATOR>& _other)
	{
		*this = _other; 
	}

	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>& resizable_array<T,ALLOCATOR>::operator=(const resizable_array<T,ALLOCATOR>& _other)
	{
		clear(); // also resets m_size to 0

		if ( m_capacity < _other.m_capacity )
			alloc(_other.m_capacity);

		// copy data
        insert(begin(), _other.cbegin(), _other.cend());

		m_size = _other.m_size;

		return *this;
	}

	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>::resizable_array(resizable_array<T,ALLOCATOR>&& _other)
	{
		*this = std::move(_other);
	}

	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>& resizable_array<T,ALLOCATOR>::operator=(resizable_array<T,ALLOCATOR>&& _other)
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

	template<typename T, system_allocator ALLOCATOR>
	T& resizable_array<T,ALLOCATOR>::back()
	{
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
	}

	template<typename T, system_allocator ALLOCATOR>
	const T& resizable_array<T,ALLOCATOR>::back() const
	{
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::pop_back()
	{
		if (empty())
			return;

		const size_t last_index = ( size() - 1 );
		std::destroy_at(m_data + last_index);
		m_size--;
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::push_back(const T& _val)
	{
		insert(end(), _val);
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::push_back(T&& _val)
	{
		insert(end(), std::forward<T&&>(_val));
	}

	template<typename T, system_allocator ALLOCATOR>
	template<typename... ARGS>
	resizable_array<T,ALLOCATOR>::reference resizable_array<T,ALLOCATOR>::emplace_back(ARGS&&... _args)
	{
		if ( size() + 1 > capacity() )
			grow();

		T* ptr = new(end()) T(std::forward<ARGS>(_args)...);
		m_size++;

		return *ptr;
	}

	template<typename T, system_allocator ALLOCATOR>
    void resizable_array<T,ALLOCATOR>::insert(iterator _pos, const T& _elem) 
    {
		const size_t index = _pos - begin(); 

		if (size() + 1 > capacity())
			grow(size() + 1);

		if (size() > 0u)
		{
			// move all elems after insert 'count' elems to the right 
			for (size_t i = size() - 1; i >= index + 1; --i)
				m_data[i + 1] = std::move(m_data[i]);
		}

		// add new elem  
        m_data[index] = _elem;
		m_size++;
    }

	template<typename T, system_allocator ALLOCATOR>
	template<typename INPUT_ITERATOR> 
	void resizable_array<T,ALLOCATOR>::insert(iterator _pos, INPUT_ITERATOR _first, INPUT_ITERATOR _last)
	{
		const size_t index = _pos - begin(); 
		const size_t count = _last - _first;

		if (count == 0u)
			return;

		if (size() + count > capacity())
			grow(size() + count);

		if (size() > 0u)
		{
			// move all elems after insert 'count' elems to the right 
			for (size_t i = size() - 1; i >= index + count; --i)
				m_data[i + count] = std::move(m_data[i]);
		}

		// add all new elems  
		for (size_t i = index; i < index + count; ++i)
		{
			m_data[i] = *_first;
			_first++;
		}

		m_size = size() + count;
	}

	template<typename T, system_allocator ALLOCATOR>
	resizable_array<T,ALLOCATOR>::iterator resizable_array<T,ALLOCATOR>::erase(iterator _it)
	{
		VEER_ASSERT(_it >= begin() && _it < end() && size() != 0u, "Iterator out of bounds");
		VEER_ASSERT(_it != nullptr, "Iterator is invalid");

		const size_t index = _it - begin();
		std::destroy_at(m_data + index);

		for(size_t i = index; i < size() - 1; ++i)
			m_data[i] = std::move(m_data[i + 1]);

		m_size--;

		return begin() + index;
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::reserve(size_t _new_capacity)
	{
		if (_new_capacity != 0)
			alloc(_new_capacity);
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::resize(size_t _new_size, const_reference _new_values /*= T()*/)
	{
		for (size_t i = _new_size; i < m_size; ++i)
			std::destroy_at( m_data + i );

		if (m_capacity < _new_size)
			alloc(_new_size);

		for (size_t i = m_size; i < _new_size; ++i)
			new(m_data + i) T(_new_values);

		m_size = _new_size;
	}


	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::grow(size_t _min_capacity_needed)
	{
		// TODO : replace with the analytic version
		size_t new_capacity = math::max(m_capacity, 8ull);
		while(new_capacity < _min_capacity_needed)
			new_capacity *= 2;

		alloc(new_capacity);
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::grow()
	{
		grow(m_capacity + 1);
		// const size_t new_capacity = m_capacity == 0u ? 8u : 2 * m_capacity;
		// alloc(new_capacity);
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::alloc(size_t _new_capacity)
	{
		VEER_ASSERT(_new_capacity > m_size, "resizable_array::alloc call with new_capacity (" << _new_capacity << ") smaller than size " << m_size << " !");
		T* new_data = static_cast<T*>(m_allocator.allocate(_new_capacity * sizeof(T)));


		if ( m_data != nullptr )
		{
			for (size_t i = 0; i < m_size; ++i)
				new_data[i] = std::move(m_data[i]);

			m_allocator.deallocate(m_data);
		}

		m_data = new_data; 
		m_capacity = _new_capacity;

	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::clear()
	{
		for(size_t i = 0; i < m_size; ++i)
			std::destroy_at( m_data + i );

		m_size = 0u;
	}

	template<typename T, system_allocator ALLOCATOR>
	void resizable_array<T,ALLOCATOR>::destroy()
	{
		clear();

		if (m_data != nullptr)
		{
			m_allocator.deallocate(m_data);
		}
	}

	template<typename ITERATOR, typename T>
	ITERATOR find(ITERATOR _from, ITERATOR _to, const T& _value)
	{
		VEER_ASSERT(_to >= _from, "Iterator _from and _to swapped");
		VEER_ASSERT((_from == nullptr) == (_to == nullptr), "Iterator _from or _to is invalid");

		while(_from != _to)
		{
			if (*_from == _value)
				return _from;

			_from++;
		}
    
		return _to;
	}

}
