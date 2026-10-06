#pragma once

#include "vec.h"
#include <core/debug.h>

namespace veer::math
{
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT>::vec()
        : m_data{0}
    {
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT>::vec(TYPE _e)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] = _e;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    TYPE vec<TYPE, ELEM_COUNT>::operator[](size_t _index) const
    {
        if (!std::is_constant_evaluated())
        {
            VEER_ASSERT(_index < ELEM_COUNT, "Index out of range");
        }

        return m_data[_index];
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    TYPE& vec<TYPE, ELEM_COUNT>::operator[](size_t _index)
    {
        if (!std::is_constant_evaluated())
        {
            VEER_ASSERT(_index < ELEM_COUNT, "Index out of range");
        }

        return m_data[_index];
    }

    ELEM_ACCESS_DEFINE(x, 0u)
    ELEM_ACCESS_DEFINE(y, 1u)
    ELEM_ACCESS_DEFINE(z, 2u)
    ELEM_ACCESS_DEFINE(w, 3u)

    ELEM_ACCESS_DEFINE(r, 0u)
    ELEM_ACCESS_DEFINE(g, 1u)
    ELEM_ACCESS_DEFINE(b, 2u)
    ELEM_ACCESS_DEFINE(a, 3u)

    //----------------------------------------------------------------------------
    //							COMPOUND OPERATORS
    //----------------------------------------------------------------------------

    // +=
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(OTHER_TYPE _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] += static_cast<TYPE>(_other);
        return *this;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] += static_cast<TYPE>(_other.m_data[i]);
        return *this;
    }

    // -=
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(OTHER_TYPE _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] -= static_cast<TYPE>(_other);
        return *this;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] -= static_cast<TYPE>(_other.m_data[i]);
        return *this;
    }

    // *=
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(OTHER_TYPE _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] *= static_cast<TYPE>(_other);
        return *this;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] *= static_cast<TYPE>(_other.m_data[i]);
        return *this;
    }

    // /=
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(OTHER_TYPE _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] /= static_cast<TYPE>(_other);
        return *this;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    template <Arithmetic OTHER_TYPE>
    constexpr vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            m_data[i] /= static_cast<TYPE>(_other.m_data[i]);
        return *this;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _vec)
    {
        vec<TYPE, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = -_vec[i];
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> operator+(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<TYPE, ELEM_COUNT> res(_lhs);
        return res += _rhs;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<TYPE, ELEM_COUNT> res(_lhs);
        return res -= _rhs;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator*(TYPE _s, const vec<TYPE, ELEM_COUNT>& _v)
    {
        vec<TYPE, ELEM_COUNT> res(_v);
        return res *= _s;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _v, TYPE _s)
    {
        vec<TYPE, ELEM_COUNT> res(_v);
        return res *= _s;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<TYPE, ELEM_COUNT> res(_lhs);
        return res *= _rhs;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _v, TYPE _s)
    {
        vec<TYPE, ELEM_COUNT> res(_v);
        return res /= _s;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<TYPE, ELEM_COUNT> res(_lhs);
        return res /= _rhs;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator==(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] == _rhs[i]);
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator!=(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] != _rhs[i]);
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator<(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] < _rhs[i]);
        return result;
    }

    // I could implement other comparisons operators using ==, <, and !, but this means I rely on the compiler
    // to remove the loops caused by each operator
    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator>(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] > _rhs[i]);
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator<=(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] <= _rhs[i]);
        return result;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator>=(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = (_lhs[i] >= _rhs[i]);
        return result;
    }

    template <size_t ELEM_COUNT>
    constexpr vec<bool, ELEM_COUNT> operator!(const vec<bool, ELEM_COUNT>& _vec)
    {
        vec<bool, ELEM_COUNT> result;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            result[i] = !_vec[i];
        return result;
    }

    //----------------------------------------------------------------------------
    //						COMMON GEOMETRIC FUNCTIONS
    //----------------------------------------------------------------------------

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
    {
        TYPE accum = 0.f;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            accum += _lhs[i] * _rhs[i];
        return accum;
    }

    template <Arithmetic TYPE>
    constexpr vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs)
    {
        return vec<TYPE, 3u>(
            _lhs[1] * _rhs[2] - _lhs[2] * _rhs[1], _lhs[2] * _rhs[0] - _lhs[0] * _rhs[2],
            _lhs[0] * _rhs[1] - _lhs[1] * _rhs[0]
        );
    }

    // sq_length

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr float sq_length(const vec<TYPE, ELEM_COUNT>& _v)
    {
        float size_accum = 0.f;
        for (size_t i = 0; i < ELEM_COUNT; ++i)
        {
            const float val = static_cast<float>(_v[i]);
            size_accum += val * val;
        }
        return size_accum;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr float length(const vec<TYPE, ELEM_COUNT>& _v)
    {
        return std::sqrt(sq_length(_v));
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v)
    {
        const float len = length(_v);
        return _v / len;
    }

    template <Arithmetic TYPE, size_t ELEM_COUNT>
    constexpr vec<TYPE, ELEM_COUNT> normalize_safe(const vec<TYPE, ELEM_COUNT>& _v, float _epsilon)
    {
        const float len = length(_v);
        if (len < _epsilon)
            return 0.f;
        return _v / len;
    }

    template <size_t ELEM_COUNT>
    constexpr bool all(const vec<bool, ELEM_COUNT>& _vec)
    {
        bool result = true;
        for (size_t i = 0u; i < ELEM_COUNT; ++i)
            result = result && _vec[i];
        return result;
    }

    template <size_t ELEM_COUNT>
    constexpr bool any(const vec<bool, ELEM_COUNT>& _vec)
    {
        bool result = false;
        for (size_t i = 0u; i < ELEM_COUNT; ++i)
            result = result || _vec[i];
        return result;
    }
} // namespace veer::math
