#pragma once

#include <core/core.h>
#include <core/concepts.h>

// TODO : implement it
// #define VEC_SIMD


#define ELEM_ACCESS_DECLARE(_elem, _index)											\
	[[nodiscard]] TYPE _elem () const requires veer::greater<ELEM_COUNT, _index>; 	\
	TYPE& _elem () requires veer::greater<ELEM_COUNT, _index>;								

#define ELEM_ACCESS_DEFINE(_elem, _index)																	\
	template<arithmetic TYPE, size_t ELEM_COUNT>															\
	[[nodiscard]] TYPE vec<TYPE, ELEM_COUNT>:: _elem () const requires veer::greater<ELEM_COUNT, _index> 	\
	{ 																										\
		return m_data[_index]; 																				\
	}																										\
	template<arithmetic TYPE, size_t ELEM_COUNT>															\
	TYPE& vec<TYPE, ELEM_COUNT>:: _elem () requires veer::greater<ELEM_COUNT, _index> 						\
	{ 																										\
		return m_data[_index]; 																				\
	}																										


namespace veer::math
{
    template<arithmetic TYPE, size_t ELEM_COUNT>
    struct vec
    {
    public:
        using value_type = TYPE;
        using type = vec<TYPE, ELEM_COUNT>;
		using size_type = size_t;
        size_type m_size = ELEM_COUNT;

    protected:
#if defined( VEC_SIMD )
		// use some internal simd type like a __m128 if ELEM_COUNT <= 4
#else // defined( VEC_SIMD )
        TYPE m_data[ELEM_COUNT];
#endif // defined( VEC_SIMD )

    public:

        vec();
        ~vec();
        
        vec(TYPE _e);

		template<arithmetic ...ARGS, typename = typename std::enable_if<(sizeof...(ARGS) == ELEM_COUNT)>::type>
        vec(ARGS&&... _args)
            : m_data{ std::forward<TYPE>(_args)... }
        { }
		
		// TODO : add copy/move ctors + cast ctors
		template<arithmetic OTHER_TYPE, size_t OTHER_ELEM_COUNT, typename = typename std::enable_if<ELEM_COUNT == OTHER_ELEM_COUNT>::type>
		vec( const vec<OTHER_TYPE, OTHER_ELEM_COUNT>& _other )
		{
			for( size_t i = 0; i < ELEM_COUNT; ++i )
				m_data[i] = _other[i];
		}
		
        [[nodiscard]] TYPE operator[](size_type _index) const;
        TYPE& operator[](size_type _index);


		ELEM_ACCESS_DECLARE(x, 0u)
		ELEM_ACCESS_DECLARE(y, 1u)
		ELEM_ACCESS_DECLARE(z, 2u)
		ELEM_ACCESS_DECLARE(w, 3u)

		ELEM_ACCESS_DECLARE(r, 0u)
		ELEM_ACCESS_DECLARE(g, 1u)
		ELEM_ACCESS_DECLARE(b, 2u)
		ELEM_ACCESS_DECLARE(a, 3u)


		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator+=(OTHER_TYPE _other);                     
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator+=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);     
																				
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator-=(OTHER_TYPE _other);                     
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator-=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);     
																				
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator*=(OTHER_TYPE _other);                     
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator*=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator/=(OTHER_TYPE _other);                     
		template<arithmetic OTHER_TYPE>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator/=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

    };

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-( const vec<TYPE, ELEM_COUNT>& _vec);


    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator+(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);


	template<arithmetic TYPE, size_t ELEM_COUNT>
	TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

	// Only defined on R3
	template<arithmetic TYPE>
	vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs);

	template<arithmetic TYPE, size_t ELEM_COUNT>
	float sq_length(const vec<TYPE, ELEM_COUNT>& _v);

	template<arithmetic TYPE, size_t ELEM_COUNT>
	float length(const vec<TYPE, ELEM_COUNT>& _v);

	// Unsafe version
	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v);
	
	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize_safe(const vec<TYPE, ELEM_COUNT>& _v, float _epsilon = FLT_EPSILON);


	using vec2u = vec<unsigned int, 2u>;
	using vec2i = vec<int, 2u>;
    using vec2f = vec<float, 2u>;

	using vec3u = vec<unsigned int, 3u>;
	using vec3i = vec<int, 3u>;
    using vec3f = vec<float, 3u>;

	using vec4u = vec<unsigned int, 4u>;
	using vec4i = vec<int, 4u>;
	using vec4f = vec<float, 4u>;
}

#include "vec_scalar.hpp"
