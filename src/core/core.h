#ifndef CORE_CORE_H_INCLUDED
#define CORE_CORE_H_INCLUDED

// STD lib headers
// TODO : remove them one by one by implementing your own version (we are here to make an apple pie,
// and to make an apple pie, one first need to invent the universe)
// Containers should be the easiest
#include <ostream>
#include <iostream>
#include <sstream>

#include <string>
#include <string_view>
#include <stdexcept>

#include <memory>

#include <algorithm>
#include <vector>
#include <set>
#include <array>

// Custom STD Containers
#include <core/Containers/span.h>



#define VEER_MAKE_STMT(_expr) do { _expr } while(0)
#define VEER_FLOAT_EPSILON FLT_EPSILON
#define VEER_DLLEXPORT __declspec(dllexport)
#define VEER_STATIC_ARRAY_SIZE( _arr ) sizeof( _arr ) / sizeof( *_arr )

#if defined( _WIN32 )
#define VEER_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#endif // defined( _WIN32 )

#endif // CORE_CORE_H_INCLUDED
