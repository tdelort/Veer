#pragma once

#include <core/concepts.h>
#include <core/veer_system_allocator.h>
#include <core/containers/resizable_array.h>

namespace veer
{
    template<typename T, system_allocator ALLOCATOR = veer_system_allocator>
    class freelist
    {
    public:
        freelist(size_t _reserve_size = 0u);
        ~freelist();

    public:
        template<typename ...ARGS>
        T* acquire(ARGS&&... _args);
        void release(T* _ptr);
    
    private:
        containers::resizable_array<size_t, ALLOCATOR> m_free_list;
        containers::resizable_array<T, ALLOCATOR> m_pool;
    };
}