#pragma once

#include "core/concepts.h"
namespace veer::math
{
    static constexpr double s_pi = 3.1415926535897932; // 16 decimals ought to do it

    template <typename T>
    constexpr const T& min(const T& _lhs, const T& _rhs)
    {
        return _lhs < _rhs ? _lhs : _rhs;
    }

    // fold version for any number of args
    template <typename T, typename... ARGS>
    constexpr T min(const T& _arg0, const ARGS&... _args)
        requires veer::Greater<sizeof...(ARGS), 1>
    {
        T accum = _arg0;
        ((accum = min(_args, accum)), ...);
        return accum;
    }

    template <typename T>
    constexpr const T& max(const T& _lhs, const T& _rhs)
    {
        return _lhs < _rhs ? _rhs : _lhs;
    }

    // fold version for any number of args
    template <typename T, typename... ARGS>
    constexpr T max(const T& _arg0, const ARGS&... _args)
        requires veer::Greater<sizeof...(ARGS), 1>
    {
        T accum = _arg0;
        ((accum = max(_args, accum)), ...);
        return accum;
    }

    template <typename T>
    constexpr T abs(const T& _val)
    {
        return max(_val, -_val);
    }

    template <typename T>
    constexpr const T& clamp(const T& _value, const T& _min, const T& _max)
    {
        return min(max(_value, _min), _max);
    }

    template <typename T>
    constexpr T ceiled_remainder(const T& _val, const T& _quotient)
    {
        return (_val + _quotient - T(1)) / _quotient;
    }


    constexpr bool nearly_equal_relative(const float& _lhs, const float& _rhs, float _relative_epsilon = FLT_EPSILON)
    {
        return math::abs(_lhs - _rhs) <= (_relative_epsilon * math::max(math::abs(_lhs), math::abs(_rhs))); 
    }

    constexpr bool nearly_equal(const float& _lhs, const float& _rhs, float _epsilon = FLT_EPSILON)
    {
        return math::abs(_lhs - _rhs) <= _epsilon;
    }

    template <typename T, typename U>
        requires(sizeof(T) == sizeof(U))
    constexpr U bit_cast(T _value)
    {
        U result;
        std::memcpy(&result, &_value, sizeof(T));
        return result;
    }

    // 127 - 15
    static constexpr int64_t s_f32_to_f16_exponent_offset = 112;

    constexpr uint16_t to_float_16(float _value)
    {
        uint32_t u_32 = bit_cast<float, uint32_t>(_value);

        // sign 1, exponent 8, mantissa 23
        // -1 ^ s_32 * 2 ^ (e_32 - 127) * (1.m_32)
        const uint32_t s_32 = (u_32 >> 31) & 0x01;
        const uint32_t e_32 = (u_32 >> 23) & 0xFF;
        const uint32_t m_32 = (u_32) & 0x7FFFFF;

        const uint16_t tmp =
            static_cast<uint16_t>(clamp<int32_t>(static_cast<int32_t>(e_32) - s_f32_to_f16_exponent_offset, 0, 31));

        // sign 1, exponent 5, mantissa 10
        // -1 ^ s_16 * 2 ^ (e_16 - 15) * (1.m_32)
        const uint16_t s_16 = (s_32 << 15) & 0x8000;
        const uint16_t e_16 = (tmp << 10) & 0x7C00;
        const uint16_t m_16 = (m_32 >> 13) & 0x03FF;

        return s_16 | e_16 | m_16;
    }

    constexpr float to_float_32(uint16_t _value)
    {
        // sign 1, exponent 5, mantissa 10
        // -1 ^ s_16 * 2 ^ (e_16 - 15) * (1.m_32)
        const uint16_t s_16 = (_value >> 15) & 0x1;
        const uint16_t e_16 = (_value >> 10) & 0x1F;
        const uint16_t m_16 = (_value) & 0x3FF;

        const uint16_t tmp = e_16 + s_f32_to_f16_exponent_offset;

        // sign 1, exponent 8, mantissa 23
        // -1 ^ s_32 * 2 ^ (e_32 - 127) * (1.m_32)
        const uint32_t s_32 = (s_16 << 31) & 0x80000000;
        const uint32_t e_32 = (tmp << 23) & 0x7F800000;
        const uint32_t m_32 = (m_32 << 13) & 0x007FFFFF;

        const uint32_t u_32 = s_32 | e_32 | m_32;

        return bit_cast<uint32_t, float>(u_32);
    }

    //------------------------------------------------------------------------
    inline static float convertHalfToFloat(uint16_t _value) noexcept
    {
        union U32toF32
        {
            uint32_t u;
            float f;
        } val;

        const uint32_t sign = (((const uint32_t)_value) & 0x8000) << 16;
        const uint32_t mant = (_value & 0x3ffu) << 13; // 13 bits are zeros
        const uint32_t exp = (_value & 0x7fff) == 0 ? 0 : (((((const uint32_t)_value) >> 10) & 0x1f) - 15 + 127) << 23;

        val.u = sign | exp | mant;
        return val.f;
    }
}; // namespace veer::math