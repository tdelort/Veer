#pragma once

#include <core/core.h>
#include <core/math/vec.h>

namespace veer::display::render
{

    template <typename T>
    struct render_data_type_traits : std::false_type
    {
    };

    template <>
    struct render_data_type_traits<bool> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(bool);
        static constexpr size_t dx12_size = 4;
    };

    template <>
    struct render_data_type_traits<int32_t> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(int32_t);
        static constexpr size_t dx12_size = 4;
    };

    template <>
    struct render_data_type_traits<int64_t> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(int64_t);
        static constexpr size_t dx12_size = 8;
    };

    template <>
    struct render_data_type_traits<uint32_t> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(uint32_t);
        static constexpr size_t dx12_size = 4;
    };

    template <>
    struct render_data_type_traits<uint64_t> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(uint64_t);
        static constexpr size_t dx12_size = 8;
    };

    template <>
    struct render_data_type_traits<float> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(float);
        static constexpr size_t dx12_size = 4;
    };

    template <>
    struct render_data_type_traits<double> : std::true_type
    {
        static constexpr size_t cpp_size = sizeof(double);
        static constexpr size_t dx12_size = 8;
    };

    template <typename T, size_t ELEM_COUNT>
        requires veer::LessEqual<ELEM_COUNT, 4>
    struct render_data_type_traits<math::vec<T, ELEM_COUNT>> : std::true_type
    {
        static constexpr size_t cpp_size = render_data_type_traits<T>::cpp_size * ELEM_COUNT;
        static constexpr size_t dx12_size = render_data_type_traits<T>::cpp_size * ELEM_COUNT;
    };

    template <typename T>
    struct is_render_data_type : render_data_type_traits<T>
    {
    };

    template <typename T>
    constexpr bool is_render_data_type_v = is_render_data_type<T>::value;

    template <typename T>
    concept RenderDataType = is_render_data_type_v<T>;
    
    
    // for bool the check is not as strict as for the others because it does not matter
    static_assert(render_data_type_traits<bool>::cpp_size <= render_data_type_traits<bool>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<int32_t>::cpp_size == render_data_type_traits<int32_t>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<int64_t>::cpp_size == render_data_type_traits<int64_t>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<uint32_t>::cpp_size == render_data_type_traits<uint32_t>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<uint64_t>::cpp_size == render_data_type_traits<uint64_t>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<float>::cpp_size == render_data_type_traits<float>::dx12_size, "mismatch");
    static_assert(render_data_type_traits<double>::cpp_size == render_data_type_traits<double>::dx12_size, "mismatch");

} // namespace veer::display::render
