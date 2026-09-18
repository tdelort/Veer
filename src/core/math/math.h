#pragma once

#include "core/concepts.h"
namespace veer::math
{
    template<typename T>
    constexpr const T& min(const T& _lhs, const T& _rhs)
    {
        return _lhs < _rhs ? _lhs : _rhs;
    }

    // fold version for any number of args
	template<typename T, typename... ARGS>
    constexpr T min(const T& _arg0, const ARGS&... _args) requires veer::Greater<sizeof...(ARGS), 1>
    {
        T accum = _arg0;
        ((accum = min(_args, accum)),...);
        return accum;
    }


    template<typename T>
    constexpr const T& max(const T& _lhs, const T& _rhs)
    {
        return _lhs < _rhs ? _rhs : _lhs;
    }

    // fold version for any number of args
	template<typename T, typename... ARGS>
    constexpr T max(const T& _arg0, const ARGS&... _args) requires veer::Greater<sizeof...(ARGS), 1>
    {
        T accum = _arg0;
        ((accum = max(_args, accum)),...);
        return accum;
    }

	template<Arithmetic T>
    constexpr T ceiled_remainder(const T& _val, const T& _quotient)
    {
        return (_val + _quotient - 1) / _quotient;
    }
};