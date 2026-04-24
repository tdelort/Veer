#pragma once

// STD lib headers
// TODO : remove them one by one by implementing your own version (we are here to make an apple pie,
// and to make an apple pie, one first need to invent the universe)
// Containers should be the easiest
// Use #include.*<[^.]*> to find their usages
#include <ostream>
#include <iostream>
#include <sstream>
#include <ios>
#include <stdexcept>
#include <memory>
#include <string>
#include <string_view>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <mutex>
#include <cstdint>


#define VEER_MAKE_STMT(_expr) do { _expr } while(0)
#define VEER_FLOAT_EPSILON FLT_EPSILON
#define VEER_DLLEXPORT __declspec(dllexport)
#define VEER_STATIC_ARRAY_SIZE( _arr ) ((size_t)(sizeof( _arr ) / sizeof( _arr[0] ) ) )

#define VEER_ENUM_CLASS_COMPOUND_OP( _cls, _op )                        \
constexpr inline _cls& operator _op( _cls& _lhs, _cls _rhs )            \
{                                                                       \
    using cls_underlying_type = std::underlying_type_t<_cls>;           \
    cls_underlying_type lhs = static_cast<cls_underlying_type>(_lhs);   \
    lhs _op static_cast<cls_underlying_type>(_rhs);                     \
    _lhs = static_cast<_cls>( lhs );                                    \
    return _lhs;                                                        \
}
// VEER_ENUM_CLASS_COMPOUND_OP's last line

#define VEER_ENUM_CLASS_BINARY_OP( _cls, _op )                                                                      \
constexpr inline _cls operator _op( _cls _lhs, _cls _rhs )                                                          \
{                                                                                                                   \
    using cls_underlying_type = std::underlying_type_t<_cls>;                                                       \
    return static_cast<_cls>(static_cast<cls_underlying_type>(_lhs) _op static_cast<cls_underlying_type>(_rhs));    \
}

#define VEER_ENUM_CLASS_UNARY_OP( _cls, _op )                               \
constexpr inline _cls operator _op( _cls _lhs )                             \
{                                                                           \
    using cls_underlying_type = std::underlying_type_t<_cls>;               \
    return static_cast<_cls>(_op static_cast<cls_underlying_type>(_lhs));   \
}

#define VEER_ENUM_CLASS_FLAG_OPERATORS( cls )    \
VEER_ENUM_CLASS_BINARY_OP( cls, & )         \
VEER_ENUM_CLASS_BINARY_OP( cls, | )         \
VEER_ENUM_CLASS_BINARY_OP( cls, ^ )         \
VEER_ENUM_CLASS_UNARY_OP( cls, ~ )          \
VEER_ENUM_CLASS_COMPOUND_OP( cls, &= )      \
VEER_ENUM_CLASS_COMPOUND_OP( cls, |= )      \
VEER_ENUM_CLASS_COMPOUND_OP( cls, ^= )
// VEER_ENUM_CLASS_OPERATORS's last line

namespace veer::flags
{
    template<typename ENUM_TYPE>
    constexpr bool any( ENUM_TYPE _val )
    {
        using underlying_type = std::underlying_type_t<ENUM_TYPE>;                                                       \
        return static_cast<underlying_type>(_val) != static_cast<underlying_type>(0);
    }

    template<typename ENUM_TYPE>
    constexpr bool none( ENUM_TYPE _val )
    {
        using underlying_type = std::underlying_type_t<ENUM_TYPE>;                                                       \
        return static_cast<underlying_type>(_val) != static_cast<underlying_type>(0);
    }
    
    template<typename ENUM_TYPE>
    constexpr bool get( const ENUM_TYPE& _mask, ENUM_TYPE _val )
    {
        return flags::any( _mask & _val );
    }

    template<typename ENUM_TYPE>
    constexpr void set( ENUM_TYPE& _mask, ENUM_TYPE _val )
    {
        _mask = _mask | _val;
    }

    template<typename ENUM_TYPE>
    constexpr void unset( ENUM_TYPE& _mask, ENUM_TYPE _val )
    {
        _mask = _mask & ~_val;                
    }
    
}

using byte_t = uint8_t;

#if defined( _WIN32 )
#define VEER_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#endif // defined( _WIN32 )
