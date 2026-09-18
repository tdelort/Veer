#pragma once

#include "core/core.h"

namespace veer
{
    template <size_t A, size_t B>
    concept GreaterEqual = A >= B;

    template <size_t A, size_t B>
    concept Greater = A > B;

    template <size_t A, size_t B>
    concept LessEqual = A <= B;

    template <size_t A, size_t B>
    concept Less = A < B;

    template <class DERIVED, class BASE>
    concept Derives = std::is_base_of_v<BASE, DERIVED>;

    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <typename T>
    concept Integral = std::is_integral_v<T>;

    template <typename T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    template <typename T>
    concept SystemAllocator = requires(T a, T::size_type n, T::pointer p) {
        { a.allocate(n) } -> std::same_as<typename T::pointer>;
        { a.deallocate(p) };
    };

} // namespace veer