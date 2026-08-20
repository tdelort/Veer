#pragma once

// TODO : move render backend macros in platform_specific.h file or smth
#define D3D12_RENDER_BACKEND
// #define VULKAN_RENDER_BACKEND
// #define METAL_RENDER_BACKEND

// TODO after above TODO : set RENDER_BACKEND macros via build system
#define RENDER_BACKEND D3D12_RENDER_BACKEND 

#if defined(D3D12_RENDER_BACKEND) 
#define RENDER_BACKEND_PLATFORM_PREFIX "dx12"
#elif defined(VULKAN_RENDER_BACKEND) 
#define RENDER_BACKEND_PLATFORM_PREFIX "vulkan"
#elif defined(METAL_RENDER_BACKEND) 
#define RENDER_BACKEND_PLATFORM_PREFIX "metal"
#endif

// STD lib headers
// TODO : remove them one by one by implementing your own version (we are here to make an apple pie,
// and to make an apple pie, one first need to invent the universe)
// Containers should be the easiest
// Use #include.*<[^.]*> or std:: to find their usages

/*
 *      std::move, std::forward, std::pair
 */
#include <utility>

/*
 *  core/veer_system_allocator
 *      std::malloc, std::free
 */
#include <cstdlib>

/*
 *  resizable_array string unique_ptr
 *      std::destroy_at
 */
#include <memory>

/*
 *  core/concepts, core/unique_ptr
 *      std::same_as
 */
#include <concepts>

/*
 *  core/concepts
 *      std::is_arithmetic_v, std::is_integral_v, std::is_floating_point_v
 *  core/core
 *      std::underlying_type_t
 *  core/unique_ptr
 *      std::is_base_of_v, std::is_array_v
 *  core/math/vec
 *      std::is_constant_evaluated, std::enable_if
 */
#include <type_traits>

/*
 *  core/math/vec
 *      std::sqrt
 */
#include <cmath>

/*
 *  containers/string
 *      std::char_traits
 */
#include <string>

/*
 *      std::memcpy
 */
#include <cstring>

/*
 *  display/render/command_buffer
 *      std::function
 */
#include <functional>

/*
 *  core/debug
 *      std::stringstream
 */
#include <sstream>

/*
 *  core/debug
 *      std::ostream, std::cerr, std::cout, std::endl
 */
#include <iostream>

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
        return static_cast<underlying_type>(_val) == static_cast<underlying_type>(0);
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
