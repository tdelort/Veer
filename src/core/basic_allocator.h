#pragma once
#ifndef CORE_BASIC_ALLOCATOR_H_INCLUDED
#define CORE_BASIC_ALLOCATOR_H_INCLUDED

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
	 
		[[nodiscard]] T* allocate( std::size_t _n )
		{
			if ( _n > std::numeric_limits<std::size_t>::max() / sizeof(T) )
				throw std::bad_array_new_length();

			T* ptr = static_cast<T*>(std::malloc(_n * sizeof(T)));

			if ( !ptr )
			{
				throw std::bad_alloc();
			}

#if defined( VEER_LOG_ALLOCS )
			VEER_LOG( "Allocate " << _n " bytes at " << ptr );
#endif // defined( VEER_LOG_ALLOCS )
			return p;
		}
	 
		void deallocate(T* _p, std::size_t _n) noexcept
		{
#if defined( VEER_LOG_ALLOCS )
			VEER_LOG( "Allocate " << _n " bytes at " << ptr );
#endif // defined( VEER_LOG_ALLOCS )
			std::free( _p );
		}
	};
}

#endif // CORE_BASIC_ALLOCATOR_H_INCLUDED