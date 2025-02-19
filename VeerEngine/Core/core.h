#ifndef CORE_CORE_H_INCLUDED
#define CORE_CORE_H_INCLUDED

#define VEER_MAKE_STMT(_expr) do { _expr } while(0)
#define VEER_FLOAT_EPSILON FLT_EPSILON
#define VEER_DLLEXPORT __declspec(dllexport)
#define VEER_STATIC_ARRAY_SIZE( _arr ) sizeof( _arr ) / sizeof( *_arr )

#if defined( _WIN32 )
#define VEER_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <wrl.h>

#endif // defined( _WIN32 )

#endif // CORE_CORE_H_INCLUDED
