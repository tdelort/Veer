#pragma once

#include "freelist.h"

namespace veer
{
    template<typename T, system_allocator ALLOCATOR>
    freelist<T, ALLOCATOR>::freelist(size_t _reserve_size /*= 0u*/)
    {
        m_pool.reserve(_reserve_size);
        m_free_list.reserve(_reserve_size);
    }

    template<typename T, system_allocator ALLOCATOR>
    freelist<T, ALLOCATOR>::~freelist()
    {
        VEER_ASSERT(m_free_list.size() == m_pool.size(), "Not all freelist elements were released before releasing freelist");
    }

    template<typename T, system_allocator ALLOCATOR>
    template<typename ...ARGS>
    T* freelist<T, ALLOCATOR>::acquire(ARGS&&... _args)
    {
        if (m_free_list.empty())
        {
            // Nothing in free list, alloc new elem
            T& new_elem = m_pool.emplace_back(std::forward<ARGS>(_args)...);
            return &new_elem;
        }
        
        // Something in free list, reuse elem
        const size_t new_elem_index = m_free_list.back();
        m_free_list.pop_back();

        T& recycled_elem = m_pool[new_elem_index];
        
        // Don't forget to call ctor on elem
        return new(&recycled_elem) T(std::forward<ARGS>(_args)...);
    }

    template<typename T, system_allocator ALLOCATOR>
    void freelist<T, ALLOCATOR>::release(T* _ptr)
    {
        const size_t index = _ptr - m_pool.begin();
        VEER_ASSERT(index < m_pool.size(), "Releasing a _ptr that was not acquired");
        VEER_ASSERT(find(m_free_list.begin(), m_free_list.end(), index) == m_free_list.end(), "Releasing a _ptr that was already released");

        // call dtor
		std::destroy_at(_ptr);

        // yield back
        m_free_list.push_back(index);
    }
}