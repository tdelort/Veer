#pragma once

#include "unique_ptr.h"

namespace veer 
{
    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::unique_ptr()
    {
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::unique_ptr(nullptr_t)
        : m_ptr(nullptr)
    {
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::unique_ptr(T* _ptr, ALLOCATOR&& _allocator)
        : m_ptr(_ptr), m_allocator(_allocator)
    {
    }

    template<typename T, system_allocator ALLOCATOR>
    template<typename U>
    unique_ptr<T,ALLOCATOR>::unique_ptr(unique_ptr<U, ALLOCATOR>&& _other)
    {
		*this = std::move(_other);
    }

    template<typename T, system_allocator ALLOCATOR>
    template<typename U>
    unique_ptr<T,ALLOCATOR>& unique_ptr<T,ALLOCATOR>::operator=(unique_ptr<U, ALLOCATOR>&& _other)
    {
        static_assert(std::is_base_of_v<T, U> || std::is_same_v<T, U>, "Only supported conversion between unique_ptr are from derived to base class");

        reset();
        m_ptr = static_cast<T*>(_other.release());

        return *this;
    }


    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::~unique_ptr()
    {
        reset();
    }

    template<typename T, system_allocator ALLOCATOR>
    template<typename ...ARGS>
    unique_ptr<T,ALLOCATOR> unique_ptr<T,ALLOCATOR>::make(ARGS&&... _args)
    {
        ALLOCATOR allocator;
        static_assert(std::is_array_v<T> == false, "using unique_ptr for array is not supported (use unique_ptr<container> instead)");
        T* ptr = new(allocator.allocate(sizeof(T))) T(std::forward<ARGS>(_args)...);

        return unique_ptr<T,ALLOCATOR>(ptr, std::move(allocator));
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::pointer unique_ptr<T,ALLOCATOR>::get() const
    {
        return m_ptr;
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::reference unique_ptr<T,ALLOCATOR>::operator*() const
    {
        return *get();
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::pointer unique_ptr<T,ALLOCATOR>::operator->() const
    {
        return get();
    }

    template<typename T, system_allocator ALLOCATOR>
    void unique_ptr<T,ALLOCATOR>::reset()
    {
        if (m_ptr != nullptr)
        {
			std::destroy_at(m_ptr);

            static_assert(std::is_array_v<T> == false, "using unique_ptr for array is not supported (use unique_ptr<container> instead)");
            m_allocator.deallocate(m_ptr);

            m_ptr = nullptr;
        }
    }

    template<typename T, system_allocator ALLOCATOR>
    unique_ptr<T,ALLOCATOR>::pointer unique_ptr<T,ALLOCATOR>::release()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }


    template<typename T, system_allocator ALLOCATOR>
    bool operator==(const unique_ptr<T, ALLOCATOR>& _ptr, nullptr_t)
    {
        return _ptr.get() == nullptr;
    }

    template<typename T, system_allocator ALLOCATOR>
    bool operator==(nullptr_t _null_ptr, unique_ptr<T, ALLOCATOR> _ptr)
    {
        return _ptr == nullptr;
    }

    template<typename T, system_allocator ALLOCATOR>
    bool operator!=(const unique_ptr<T, ALLOCATOR>& _ptr, nullptr_t _null_ptr)
    {
        return !(_ptr == nullptr);
    }

    template<typename T, system_allocator ALLOCATOR>
    bool operator!=(nullptr_t _null_ptr, unique_ptr<T, ALLOCATOR> _ptr)
    {
        return !(_ptr == nullptr);
    }
}