#pragma once

#include "debug.h"

#define VEER_LOG_ALLOCS

namespace veer
{
	template<typename T>
	class basic_allocator
	{
		using value_type = T;

		// TODO implement Allocator concept

		basic_allocator() = default;
 
		template<class U>
		constexpr basic_allocator(const basic_allocator<U>&) noexcept {}
	 
		[[nodiscard]] T* allocate( size_t _n )
		{
			VEER_ASSERT( _n <= ( std::numeric_limits<size_t>::max() / sizeof(T) ), "Alloc size way too big" );

			T* ptr = static_cast<T*>(std::malloc(_n * sizeof(T)));
			VEER_ASSERT( ptr != nullptr, "Alloc Failed" );

#if defined( VEER_LOG_ALLOCS )
			VEER_LOG( "[ALLOC] " << _n << " elems at " << ptr );
#endif // defined( VEER_LOG_ALLOCS )
			return ptr;
		}
	 
		void deallocate(T* _ptr, size_t _n) noexcept
		{
#if defined( VEER_LOG_ALLOCS )
			VEER_LOG( "[DEALLOC] " << _n << " elems from " << _ptr );
#endif // defined( VEER_LOG_ALLOCS )
			std::free( _ptr );
		}
	};
}