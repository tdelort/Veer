#pragma once

#include <core/concepts.h>
#include <core/core.h>

// TODO : implement it
// #define VEC_SIMD

#define ELEM_ACCESS_DECLARE(_elem, _index)                                                                             \
    [[nodiscard]] constexpr TYPE _elem() const                                                                         \
        requires veer::Greater<ELEM_COUNT, _index>;                                                                    \
    constexpr TYPE& _elem()                                                                                            \
        requires veer::Greater<ELEM_COUNT, _index>;

#define ELEM_ACCESS_DEFINE(_elem, _index)                                                                              \
    template <Arithmetic TYPE, size_t ELEM_COUNT>                                                                      \
    constexpr TYPE vec<TYPE, ELEM_COUNT>::_elem() const                                                                \
        requires veer::Greater<ELEM_COUNT, _index>                                                                     \
    {                                                                                                                  \
        return m_data[_index];                                                                                         \
    }                                                                                                                  \
    template <Arithmetic TYPE, size_t ELEM_COUNT>                                                                      \
    constexpr TYPE& vec<TYPE, ELEM_COUNT>::_elem()                                                                     \
        requires veer::Greater<ELEM_COUNT, _index>                                                                     \
    {                                                                                                                  \
        return m_data[_index];                                                                                         \
    }

namespace veer::math
{
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    struct vec
    {
    public:
        using value_type = TYPE;
        using type = vec<TYPE, ELEM_COUNT>;
        using size_type = size_t;
        static constexpr size_type m_size = ELEM_COUNT;

    private:
#if defined(VEC_SIMD)
        // use some internal simd type like a __m128 if ELEM_COUNT <= 4
#else  // defined( VEC_SIMD )
        TYPE m_data[ELEM_COUNT];
#endif // defined( VEC_SIMD )

    public:
        constexpr vec();

        constexpr vec(TYPE _e);

        template <Arithmetic... ARGS>
            requires(sizeof...(ARGS) == ELEM_COUNT)
        constexpr vec(ARGS... _args)
            : m_data{_args...}
        {
        }

        template <size_t OTHER_ELEM_COUNT, Arithmetic... ARGS>
            requires((OTHER_ELEM_COUNT + sizeof...(ARGS)) == ELEM_COUNT)
        constexpr vec(const vec<TYPE, OTHER_ELEM_COUNT>& _v, ARGS... _args)
        {
            size_t i = 0;
            for (; i < OTHER_ELEM_COUNT; ++i)
                m_data[i] = static_cast<TYPE>(_v[i]);

            ((m_data[i++] = _args), ...);
        }

        // TODO : add copy/move ctors + cast ctors
        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
        {
            for (size_t i = 0; i < ELEM_COUNT; ++i)
                m_data[i] = static_cast<TYPE>(_other[i]);
            return *this;
        }

        template <Arithmetic OTHER_TYPE>
        constexpr vec(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
        {
            *this = _other;
        }

        // not constexpr because of the bounds check assert
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

        template <size_t OTHER_ELEM_COUNT>
            requires LessEqual<OTHER_ELEM_COUNT, ELEM_COUNT>
        vec<TYPE, OTHER_ELEM_COUNT> get(const size_t& _offset = 0u) const;

        template <size_t OTHER_ELEM_COUNT>
            requires LessEqual<OTHER_ELEM_COUNT, ELEM_COUNT>
        void set(const vec<TYPE, OTHER_ELEM_COUNT>& _v, const size_t& _offset = 0u);

        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator+=(OTHER_TYPE _other);
        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator+=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator-=(OTHER_TYPE _other);
        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator-=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator*=(OTHER_TYPE _other);
        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator*=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator/=(OTHER_TYPE _other);
        template <Arithmetic OTHER_TYPE>
        constexpr vec<TYPE, ELEM_COUNT>& operator/=(const vec<OTHER_TYPE, ELEM_COUNT>& _other);

        friend std::ostream& operator<<(std::ostream& _os, const vec<TYPE, ELEM_COUNT>& _vec)
        {
            _os << "(" << _vec[0];
            for (size_t i = 1; i < ELEM_COUNT; ++i)
                _os << ", " << _vec[i];
            _os << ")";
            return _os;
        }
    };

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _vec);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator+(
        const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator-(
        const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator*(TYPE _s, const vec<TYPE, ELEM_COUNT>& _v);
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _v, TYPE _s);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator*(
        const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _v, TYPE _s);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator/(
        const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs
    );

#define VEC_BOOL_BINARY_OPERATOR_DECLARE(_op)                                                                          \
    template <Arithmetic TYPE, size_t ELEM_COUNT>                                                                      \
    [[nodiscard]] constexpr vec<bool, ELEM_COUNT> operator _op(                                                        \
        const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs                                           \
    );

    VEC_BOOL_BINARY_OPERATOR_DECLARE(==)
    VEC_BOOL_BINARY_OPERATOR_DECLARE(!=)
    VEC_BOOL_BINARY_OPERATOR_DECLARE(<)
    VEC_BOOL_BINARY_OPERATOR_DECLARE(<=)
    VEC_BOOL_BINARY_OPERATOR_DECLARE(>)
    VEC_BOOL_BINARY_OPERATOR_DECLARE(>=)

#undef VEC_BOOL_BINARY_OPERATOR_DECLARE

    template <size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<bool, ELEM_COUNT> operator!(const vec<bool, ELEM_COUNT>& _vec);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs);

    // Only defined in R3 (also-ish in R7 but we don't care about this one)
    template <Arithmetic TYPE>
    [[nodiscard]] constexpr vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr float sq_length(const vec<TYPE, ELEM_COUNT>& _v);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr float length(const vec<TYPE, ELEM_COUNT>& _v);

    // Unsafe version
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v);

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> normalize_safe(
        const vec<TYPE, ELEM_COUNT>& _v, float _epsilon = FLT_EPSILON
    );

    template <size_t ELEM_COUNT>
    [[nodiscard]] constexpr bool all(const vec<bool, ELEM_COUNT>& _vec);

    template <size_t ELEM_COUNT>
    [[nodiscard]] constexpr bool any(const vec<bool, ELEM_COUNT>& _vec);

    using vec2u = vec<uint32_t, 2u>;
    using vec2i = vec<int32_t, 2u>;
    using vec2f = vec<float, 2u>;
    using vec2u64 = vec<uint64_t, 2u>;
    using vec2i64 = vec<int64_t, 2u>;
    using vec2f64 = vec<float, 2u>;

    using vec3u = vec<uint32_t, 3u>;
    using vec3i = vec<int32_t, 3u>;
    using vec3f = vec<float, 3u>;
    using vec3u64 = vec<uint64_t, 3u>;
    using vec3i64 = vec<int64_t, 3u>;
    using vec3f64 = vec<float, 3u>;

    using vec4u = vec<uint32_t, 4u>;
    using vec4i = vec<int32_t, 4u>;
    using vec4f = vec<float, 4u>;
    using vec4u64 = vec<uint64_t, 4u>;
    using vec4i64 = vec<int64_t, 4u>;
    using vec4f64 = vec<float, 4u>;

    using vec2b = vec<bool, 2u>;
    using vec3b = vec<bool, 3u>;
    using vec4b = vec<bool, 4u>;
} // namespace veer::math

#include "vec_scalar.hpp"
