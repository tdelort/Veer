#pragma once

#include "core/core.h"

namespace veer
{
    template <size_t A, size_t B>
    concept greater_equal = A >= B;

    template <size_t A, size_t B>
    concept greater = A > B;

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template <typename T>
    concept integral = std::is_integral_v<T>;

    template <typename T>
    concept floating_point = std::is_floating_point_v<T>;

    template <typename T>
    concept system_allocator = requires(T a, T::size_type n, T::pointer p) {
        { a.allocate(n) } -> std::same_as<typename T::pointer>;
        { a.deallocate(p) };
    };
} // namespace veer