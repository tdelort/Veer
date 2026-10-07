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
    mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT> mat<TYPE, ROW_COUNT, COLUMN_COUNT>::get() const
    {
        mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT> result{};
        for (size_t r = 0; r < OTHER_ROW_COUNT; ++r)
        {
            for (size_t c = 0; c < OTHER_COLUMN_COUNT; ++c)
            {
                result[r][c] = m_rows[r][c];
            }
        }
        return result;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    template <size_t OTHER_ROW_COUNT, size_t OTHER_COLUMN_COUNT>
        requires LessEqual<OTHER_ROW_COUNT, ROW_COUNT> && LessEqual<OTHER_COLUMN_COUNT, COLUMN_COUNT>
    void mat<TYPE, ROW_COUNT, COLUMN_COUNT>::set(const mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT>& _m)
    {
        for (size_t r = 0; r < OTHER_ROW_COUNT; ++r)
        {
            for (size_t c = 0; c < OTHER_COLUMN_COUNT; ++c)
            {
                m_rows[r][c] = _m[r][c];
            }
        }
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator+=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _other
    )
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            m_rows[i] += _other[i];
        return *this;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator-=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _other
    )
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            m_rows[i] -= _other[i];
        return *this;
    }

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& mat<TYPE, ROW_COUNT, COLUMN_COUNT>::operator*=(TYPE _s)
    {
        for (size_t i = 0u; i < ROW_COUNT; ++i)
            m_rows[i] *= _s;
        return *this;
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
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator*(
        const vec<TYPE, ROW_COUNT>& _v, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
    )
    {
        vec<TYPE, COLUMN_COUNT> res{};
        for (size_t i = 0; i < ROW_COUNT; ++i)
        {
            for (size_t j = 0; j < ROW_COUNT; ++j)
            {
                res[i] += _m[i][j];
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

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, COLUMN_COUNT, ROW_COUNT> mat_utils::transpose(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
    )
    {
        mat<TYPE, COLUMN_COUNT, ROW_COUNT> res{};
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            for (size_t c = 0; c < COLUMN_COUNT; ++c)
            {
                res[c][r] = _m[r][c];
            }
        }
        return res;
    }

    // 2D rotations :
    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 2u, 2u> mat_utils::rotation_2d(TYPE _s)
    {
        const float cos_s = std::cos(_s);
        const float sin_s = std::sin(_s);
        return mat<TYPE, 2u, 2u>(vec<TYPE, 2u>{cos_s, -sin_s}, vec<TYPE, 2u>{sin_s, cos_s});
    }

    // 3D rotations :
    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> mat_utils::rotation_3d(vec<TYPE, 3u> _euler)
    {
        // row vectors, operations from left to right
        return mat_utils::rotation_3d_x(_euler.x()) * mat_utils::rotation_3d_y(_euler.y()) *
               mat_utils::rotation_3d_z(_euler.z());
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> mat_utils::rotation_3d_x(TYPE _s)
    {
        const float cos_s = std::cos(_s);
        const float sin_s = std::sin(_s);
        return mat<TYPE, 3u, 3u>(
            vec<TYPE, 3u>{1.f, 0.f, 0.f}, vec<TYPE, 3u>{0.f, cos_s, -sin_s}, vec<TYPE, 3u>{0.f, sin_s, cos_s}
        );
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> mat_utils::rotation_3d_y(TYPE _s)
    {
        const float cos_s = std::cos(_s);
        const float sin_s = std::sin(_s);
        return mat<TYPE, 3u, 3u>(
            vec<TYPE, 3u>{cos_s, 0.f, sin_s}, vec<TYPE, 3u>{0.f, 1.f, 0.f}, vec<TYPE, 3u>{-sin_s, 0.f, cos_s}
        );
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> mat_utils::rotation_3d_z(TYPE _s)
    {
        const float cos_s = std::cos(_s);
        const float sin_s = std::sin(_s);
        return mat<TYPE, 3u, 3u>(
            vec<TYPE, 3u>{cos_s, -sin_s, 0.f}, vec<TYPE, 3u>{sin_s, cos_s, 0.f}, vec<TYPE, 3u>{0.f, 0.f, 1.f}
        );
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 3u, 3u> mat_utils::translation_2d(vec<TYPE, 2u> _pos)
    {
        return mat<TYPE, 3u, 3u>(
            vec<TYPE, 3u>{1.f, 0.f, _pos.x()}, vec<TYPE, 3u>{0.f, 1.f, _pos.y()}, vec<TYPE, 3u>{0.f, 0.f, 1.f}
        );
    }

    template <Arithmetic TYPE>
    [[nodiscard]] constexpr mat<TYPE, 4u, 4u> mat_utils::translation_3d(vec<TYPE, 3u> _pos)
    {
        return mat<TYPE, 4u, 4u>(
            vec<TYPE, 4u>{1.f, 0.f, 0.f, _pos.x()}, vec<TYPE, 4u>{0.f, 1.f, 0.f, _pos.y()},
            vec<TYPE, 4u>{0.f, 0.f, 1.f, _pos.z()}, vec<TYPE, 4u>{0.f, 0.f, 0.f, 1.f}
        );
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ELEM_COUNT, ELEM_COUNT> mat_utils::scale(vec<TYPE, ELEM_COUNT> _scale)
    {
        mat<TYPE, ELEM_COUNT, ELEM_COUNT> result{};
        for (size_t i = 0u; i < ELEM_COUNT; ++i)
            result[i][i] = _scale[i];
        return result;
    }
} // namespace veer::math