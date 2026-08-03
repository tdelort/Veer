#pragma once

#include "string.h"

#include <core/debug.h>
#include <core/core.h>
#include <core/math/math.h>

namespace veer::containers
{
    template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>::base_string()
		: m_data{nullptr}, m_size{0u}, m_capacity{0u}, m_allocator()
	{

	}

	template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>::~base_string()
	{
		destroy();
	}

	template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>::base_string(const base_string<T,ALLOCATOR>& _other)
	{
		*this = _other; 
	}

	template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>& base_string<T,ALLOCATOR>::operator=(const base_string<T,ALLOCATOR>& _other)
	{
		clear(); // also resets m_size to 0

		if (m_capacity < _other.m_capacity)
			alloc(_other.m_capacity);

		// copy data
        insert(begin(), _other.cbegin(), _other.cend());

		return *this;
	}

	template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>::base_string(base_string<T,ALLOCATOR>&& _other)
	{
		*this = std::move(_other);
	}

	template<typename T, allocator ALLOCATOR>
	base_string<T,ALLOCATOR>& base_string<T,ALLOCATOR>::operator=(base_string<T,ALLOCATOR>&& _other)
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


	template<typename T, allocator ALLOCATOR>
    T& base_string<T,ALLOCATOR>::back()
    {
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
    }

	template<typename T, allocator ALLOCATOR>
    const T& base_string<T,ALLOCATOR>::back() const
    {
		VEER_ASSERT(!empty(), "");
		const size_t last_index = size() - 1u;
		return m_data[last_index];
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::push_back(const T& _val) 
    { 
        insert(end(), _val);
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::pop_back()
    { 
        if (empty())
            return;

		const size_t last_index = size() - 1;
		std::destroy_at(m_data + last_index);
        m_data[last_index] = T();
		m_size--;
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::insert(iterator _pos, const T& _elem) 
    {
		const size_t index = (_pos - begin()) / sizeof(T); 

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

	template<typename T, allocator ALLOCATOR>
    template<typename INPUT_ITERATOR> 
    void base_string<T,ALLOCATOR>::insert(iterator _pos, INPUT_ITERATOR _first, INPUT_ITERATOR _last) 
    { 
		const size_t index = (_pos - begin()) / sizeof(T); 
		const size_t count = (_last - _first) / sizeof(T);

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

	template<typename T, allocator ALLOCATOR>
    template<typename CHAR_TYPE>
    void base_string<T,ALLOCATOR>::append(CHAR_TYPE* _str) 
    { 
        insert(end(), _str, _str + std::char_traits<CHAR_TYPE>::length(_str)); 
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::clear() 
    {
		for(size_t i = 0; i < m_size; ++i)
        {
			std::destroy_at(m_data + i);
            m_data[i] = T();
        }

		m_size = 0u;
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::destroy() 
    {
		clear();

		if (m_data != nullptr)
			m_allocator.deallocate(m_data);
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::grow(size_t _min_capacity_needed)
    {
		size_t new_capacity = math::max(m_capacity, 8ull);
		while(new_capacity < _min_capacity_needed)
			new_capacity *= 2;

		alloc(new_capacity);
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::grow()
    {
        grow(m_capacity + 1);
    }

	template<typename T, allocator ALLOCATOR>
    void base_string<T,ALLOCATOR>::alloc(size_t _new_capacity)
    {
		VEER_ASSERT(_new_capacity > m_size, "base_string::alloc call with new_capacity (" << _new_capacity << ") smaller than size " << m_size << " !");
        // Always alloc one more than needed for '\0'
        const size_t new_capacity_with_null_char = _new_capacity + 1; 
		T* new_data = static_cast<T*>(m_allocator.allocate(new_capacity_with_null_char * sizeof(T)));

		if (m_data != nullptr)
		{
			for (size_t i = 0; i < m_size; ++i)
				new_data[i] = std::move(m_data[i]);

			m_allocator.deallocate(m_data);
		}

		m_data = new_data; 
		m_capacity = _new_capacity;

        // fill everything else with '\0'
        for (size_t i = m_size; i < new_capacity_with_null_char; ++i)
            m_data[i] = T();
    }
}
