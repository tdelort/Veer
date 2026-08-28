#pragma once

#include "debug.h"
#include <core/concepts.h>

#define VEER_LOG_ALLOCS

namespace veer
{
	class veer_system_allocator
	{
	public:
		using pointer = void*;
		using const_pointer = const void*;
		using size_type = size_t;

		veer_system_allocator() = default;
 
		constexpr veer_system_allocator(const veer_system_allocator&) noexcept {}
	 
		template<typename T>
		[[nodiscard]] pointer allocate(size_type _count)
		{
			return allocate(_count * sizeof(T));
		}

		[[nodiscard]] pointer allocate(size_type _size_in_bytes)
		{
			pointer ptr = static_cast<pointer>(std::malloc(_size_in_bytes));
			VEER_ASSERT(ptr != nullptr, "Alloc Failed");

#if defined(VEER_LOG_ALLOCS)
			VEER_LOG("[ALLOC] " << _size_in_bytes << " bytes at " << ptr);
#endif // defined( VEER_LOG_ALLOCS )

			return ptr;
		}
	 
		void deallocate(pointer _ptr) noexcept
		{
#if defined(VEER_LOG_ALLOCS)
			VEER_LOG("[DEALLOC] from " << _ptr);
#endif // defined( VEER_LOG_ALLOCS )
			std::free(_ptr);
		}
	};

	static_assert(system_allocator<veer_system_allocator>, "veer_system_allocator does not satifies system_allocator concept");
}