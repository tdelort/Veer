#pragma once

#include "mat.h"

#include <core/concepts.h>
#include <core/core.h>
#include <core/math/math.h>
#include <core/math/vec.h>

namespace veer::math
{
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>::mat()
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            m_rows[i] = vec<TYPE, COLUMN_COUNT>();
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT> mat<TYPE, ROW_COUNT, COLUMN_COUNT>::identity()
    {
        mat<TYPE, ROW_COUNT, COLUMN_COUNT> identity{}; // fully zeroed

        for (size_t i = 0u; i < math::min(COLUMN_COUNT, ROW_COUNT); ++i)
            identity[i][i] = TYPE(1);

        return identity;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    const vec<TYPE, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator[](size_t _index) const
    {
        if (!std::is_constant_evaluated())
        {
            VEER_ASSERT(_index < ROW_COUNT, "Index out of range");
        }

        return m_rows[_index];
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    vec<TYPE, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator[](size_t _index)
    {
        if (!std::is_constant_evaluated())
        {
            VEER_ASSERT(_index < ROW_COUNT, "Index out of range");
        }

        return m_rows[_index];
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    template <size_t OTHER_ROW_COUNT, size_t OTHER_COLUMN_COUNT>
        requires LessEqual<OTHER_ROW_COUNT, ROW_COUNT> && LessEqual<OTHER_COLUMN_COUNT, COLUMN_COUNT>
    constexpr mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT> mat<
        TYPE, ROW_COUNT, COLUMN_COUNT>::get(const vec2u& _offset /*= vec2u(0u)*/) const
    {
        mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT> result{};
        for (size_t r = 0; r < OTHER_ROW_COUNT; ++r)
        {
            result[r] = operator[](_offset.x() + r).template get<OTHER_COLUMN_COUNT>(_offset.y());
        }
        return result;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    template <size_t OTHER_ROW_COUNT, size_t OTHER_COLUMN_COUNT>
        requires LessEqual<OTHER_ROW_COUNT, ROW_COUNT> && LessEqual<OTHER_COLUMN_COUNT, COLUMN_COUNT>
    constexpr void mat<TYPE, ROW_COUNT, COLUMN_COUNT>::set(
        const mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT>& _m, const vec2u& _offset /*= vec2u(0u)*/
    )
    {
        for (size_t r = 0; r < OTHER_ROW_COUNT; ++r)
        {
            operator[](_offset.x() + r).set(_m[r], _offset.y());
        }
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, COLUMN_COUNT, ROW_COUNT> mat<TYPE, ROW_COUNT, COLUMN_COUNT>::transpose() const
    {
        mat<TYPE, COLUMN_COUNT, ROW_COUNT> res{};
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            for (size_t c = 0; c < COLUMN_COUNT; ++c)
            {
                res[c][r] = operator[](r)[c];
            }
        }
        return res;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator+=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _other
    )
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            operator[](i) += _other[i];
        return *this;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator-=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _other
    )
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            operator[](i) -= _other[i];
        return *this;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator*=(TYPE _s)
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            operator[](i) *= _s;
        return *this;
    }

    // casts
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator TYPE()
        requires(Equal<ROW_COUNT, 1> && Equal<COLUMN_COUNT, 1>)
    {
        return operator[](0u)[0u];
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator vec<TYPE, COLUMN_COUNT>()
        requires(Equal<ROW_COUNT, 1>)
    {
        return operator[](0u);
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT> operator+(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    )
    {
        mat<TYPE, ROW_COUNT, COLUMN_COUNT> res(_lhs);
        res += _rhs;
        return res;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator-(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    )
    {
        mat<TYPE, ROW_COUNT, COLUMN_COUNT> res(_lhs);
        res -= _rhs;
        return res;
    }

    // multiply by a ...
    // ... scalar
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator*(TYPE _s, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m)
    {
        mat<TYPE, ROW_COUNT, COLUMN_COUNT> res(_m);
        res *= _s;
        return res;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator*(const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m, TYPE _s)
    {
        mat<TYPE, ROW_COUNT, COLUMN_COUNT> res(_m);
        res *= _s;
        return res;
    }

    // ... vector
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, COLUMN_COUNT> operator*(
        const vec<TYPE, ROW_COUNT>& _v, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
    )
    {
        vec<TYPE, COLUMN_COUNT> res{};
        for (size_t c = 0; c < COLUMN_COUNT; ++c)
        {
            for (size_t r = 0; r < ROW_COUNT; ++r)
            {
                res[c] += _v[r] * _m[r][c];
            }
        }
        return res;
    }

    // ... matrix
    template <Arithmetic TYPE, size_t M, size_t N, size_t P>
    [[nodiscard]] constexpr mat<TYPE, M, P> operator*(const mat<TYPE, M, N>& _lhs, const mat<TYPE, N, P>& _rhs)
    {
        mat<TYPE, M, P> res{};
        for (size_t m = 0; m < M; ++m)
        {
            for (size_t p = 0; p < P; ++p)
            {
                for (size_t n = 0; n < N; ++n)
                {
                    res[m][p] += _lhs[m][n] * _rhs[n][p];
                }
            }
        }
        return res;
    }

#define MAT_BOOL_BINARY_OPERATOR_DEFINE(_op)                                                                           \
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>                                                  \
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator _op(                                           \
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs                 \
    )                                                                                                                  \
    {                                                                                                                  \
        mat<bool, ROW_COUNT, COLUMN_COUNT> res{};                                                                      \
        for (size_t r = 0; r < ROW_COUNT; ++r)                                                                         \
        {                                                                                                              \
            res[r] = (_lhs[r] _op _rhs[r]);                                                                            \
        }                                                                                                              \
        return res;                                                                                                    \
    }

    MAT_BOOL_BINARY_OPERATOR_DEFINE(==)
    MAT_BOOL_BINARY_OPERATOR_DEFINE(!=)
    MAT_BOOL_BINARY_OPERATOR_DEFINE(<)
    MAT_BOOL_BINARY_OPERATOR_DEFINE(<=)
    MAT_BOOL_BINARY_OPERATOR_DEFINE(>)
    MAT_BOOL_BINARY_OPERATOR_DEFINE(>=)

#undef MAT_BOOL_BINARY_OPERATOR_DEFINE

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator!(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat)
    {
        mat<bool, ROW_COUNT, COLUMN_COUNT> res{};
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            res[r] = !_mat[r];
        }
        return res;
    }

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr bool all(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat)
    {
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            if (!all(_mat[r]))
                return false;
        }
        return true;
    }

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr bool any(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat)
    {
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            if (any(_mat[r]))
                return true;
        }
        return false;
    }

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> nearly_equal_relative(
        const mat<float, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<float, ROW_COUNT, COLUMN_COUNT>& _rhs,
        float _relative_epsilon /*= FLT_EPSILON*/
    )
    {
        mat<bool, ROW_COUNT, COLUMN_COUNT> result{};
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            result[i] = math::nearly_equal_relative(_lhs[i], _rhs[i], _relative_epsilon);
        return result;
    }

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> nearly_equal(
        const mat<float, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<float, ROW_COUNT, COLUMN_COUNT>& _rhs,
        float _epsilon /*= FLT_EPSILON*/
    )
    {
        mat<bool, ROW_COUNT, COLUMN_COUNT> result{};
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            result[i] = math::nearly_equal(_lhs[i], _rhs[i], _epsilon);
        return result;
    }
} // namespace veer::math