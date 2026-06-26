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
    constexpr T min(T&& _arg0, ARGS&&... _args) requires veer::greater<sizeof...(ARGS), 1>
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
    constexpr T max(T&& _arg0, ARGS&&... _args) requires veer::greater<sizeof...(ARGS), 1>
    {
        T accum = _arg0;
        ((accum = max(_args, accum)),...);
        return accum;
    }
};