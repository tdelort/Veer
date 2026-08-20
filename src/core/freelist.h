#pragma once

#include <core/concepts.h>
#include <core/containers/resizable_array.h>

namespace veer
{
    template<typename T, system_allocator ALLOCATOR>
    class freelist
    {
    public:
        T* get();
    
    private:
        containers::resizable_array<size_t, ALLOCATOR> m_free_list;
        containers::resizable_array<T, ALLOCATOR> m_pool;
    };
}