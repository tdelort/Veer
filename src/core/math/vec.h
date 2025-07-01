#ifndef CORE_MATH_VEC_H_INCLUDED
#define CORE_MATH_VEC_H_INCLUDED

#include <core/core.h>

namespace veer
{
    template<typename TYPE, size_t ELEM_COUNT>
    struct vec
    {
    public:
        using value_type = TYPE;
        using type = vec<TYPE, ELEM_COUNT>;
        size_t m_size = ELEM_COUNT;

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

		template<typename ...ARGS, typename = typename std::enable_if<(sizeof...(ARGS) == ELEM_COUNT)>::type>
        vec(ARGS&&... _args)
            : m_data{ std::forward<ARGS>(_args)... }
        { }

		//template<size_t VEC_SIZE, typename ...ARGS, typename = typename std::enable_if<( (sizeof...(Args) + VEC_SIZE) == COUNT)>::type>
        //vec(const vec<TYPE, VEC_SIZE> _vec, ARGS&&... _args)
        //{
        //}

        [[nodiscard]] TYPE operator[](size_t _index) const;
        TYPE& operator[](size_t _index);

		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator+=(E0 _other);                     
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator+=(const vec<E0, ELEM_COUNT>& _other);     
																				
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator-=(E0 _other);                     
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator-=(const vec<E0, ELEM_COUNT>& _other);     
																				
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator*=(E0 _other);                     
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator*=(const vec<E0, ELEM_COUNT>& _other);

		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator/=(E0 _other);                     
		template<typename E0>                                                   
		[[nodiscard]] vec<TYPE, ELEM_COUNT>& operator/=(const vec<E0, ELEM_COUNT>& _other);

    };

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-( const vec<TYPE, ELEM_COUNT>& _vec);


    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator+(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);


	template<typename TYPE, size_t ELEM_COUNT>
	TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

	// Only defined on R3
	template<typename TYPE>
	vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs);

	template<typename TYPE, size_t ELEM_COUNT>
	float sq_length(const vec<TYPE, ELEM_COUNT>& _v);

	template<typename TYPE, size_t ELEM_COUNT>
	float length(const vec<TYPE, ELEM_COUNT>& _v);

	// Unsafe version
	template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v);
	
	template<typename TYPE, size_t ELEM_COUNT>
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

#endif // CORE_MATH_VEC_H_INCLUDED
