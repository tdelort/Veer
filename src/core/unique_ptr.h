#pragma once

#include <core/concepts.h>
#include <core/veer_system_allocator.h>

namespace veer
{
    template<typename T, system_allocator ALLOCATOR = veer_system_allocator>
    class unique_ptr
    {
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using value_type = T;
		using allocator_type = ALLOCATOR;

    public:
        unique_ptr();
        unique_ptr(nullptr_t);
        unique_ptr(T* _ptr, ALLOCATOR&& _allocator);

		unique_ptr(const unique_ptr& _other) = delete;
		unique_ptr& operator=(const unique_ptr& _other) = delete;

        template<typename U>
		unique_ptr(unique_ptr<U,ALLOCATOR> && _other);
        template<typename U>
		unique_ptr& operator=(unique_ptr<U,ALLOCATOR>&& _other);

        ~unique_ptr();

        template<typename ...ARGS>
        static unique_ptr<T, ALLOCATOR> make(ARGS&&... _args);

        pointer get() const;

        reference operator*() const;

        pointer operator->() const;

        void reset();
        pointer release();

        // Not a big fan
        // bool operator()() const;
    private:
        T* m_ptr{nullptr};
        ALLOCATOR m_allocator{};
    };


    template<typename T, system_allocator ALLOCATOR>
    bool operator==(const unique_ptr<T, ALLOCATOR>& _ptr, nullptr_t);

    template<typename T, system_allocator ALLOCATOR>
    bool operator==(nullptr_t, unique_ptr<T, ALLOCATOR> _ptr);

    template<typename T, system_allocator ALLOCATOR>
    bool operator!=(const unique_ptr<T, ALLOCATOR>& _ptr, nullptr_t);

    template<typename T, system_allocator ALLOCATOR>
    bool operator!=(nullptr_t, unique_ptr<T, ALLOCATOR> _ptr);
}

#include "unique_ptr.hpp"