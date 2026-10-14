#pragma once

#include <core/core.h>
#include <core/math/mat.h>
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
        static constexpr size_t cpu_size = sizeof(bool);
        static constexpr size_t gpu_size = 4;
    };

    template <>
    struct render_data_type_traits<int32_t> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(int32_t);
        static constexpr size_t gpu_size = 4;
    };

    template <>
    struct render_data_type_traits<int64_t> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(int64_t);
        static constexpr size_t gpu_size = 8;
    };

    template <>
    struct render_data_type_traits<uint32_t> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(uint32_t);
        static constexpr size_t gpu_size = 4;
    };

    template <>
    struct render_data_type_traits<uint64_t> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(uint64_t);
        static constexpr size_t gpu_size = 8;
    };

    template <>
    struct render_data_type_traits<float> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(float);
        static constexpr size_t gpu_size = 4;
    };

    template <>
    struct render_data_type_traits<double> : std::true_type
    {
        static constexpr size_t cpu_size = sizeof(double);
        static constexpr size_t gpu_size = 8;
    };

    template <typename T, size_t ELEM_COUNT>
        requires veer::LessEqual<ELEM_COUNT, 4>
    struct render_data_type_traits<math::vec<T, ELEM_COUNT>> : std::true_type
    {
        static constexpr size_t cpu_size = render_data_type_traits<T>::cpu_size * ELEM_COUNT;
        static constexpr size_t gpu_size = render_data_type_traits<T>::gpu_size * ELEM_COUNT;
    };

    template <typename T, size_t ROW_COUNT, size_t COLUMN_COUNT>
        requires veer::LessEqual<ROW_COUNT, 4> && veer::LessEqual<COLUMN_COUNT, 4>
    struct render_data_type_traits<math::mat<T, ROW_COUNT, COLUMN_COUNT>> : std::true_type
    {
        static constexpr size_t cpu_size = render_data_type_traits<T>::cpu_size * ROW_COUNT * COLUMN_COUNT;
        static constexpr size_t gpu_size = render_data_type_traits<T>::gpu_size * ROW_COUNT * COLUMN_COUNT;
    };

    template <typename T>
    struct is_render_data_type : render_data_type_traits<T>
    {
    };

    template <typename T>
    constexpr bool is_render_data_type_v = is_render_data_type<T>::value;

    template <typename T>
    concept RenderDataType = is_render_data_type_v<T>;

    // Common conversion function for render data types that are arithmetic
    template <RenderDataType T>
        requires Arithmetic<T>
    void s_cpu_to_gpu(byte_t* _gpu, const T& _cpu)
    {
        std::memcpy(_gpu, &_cpu, render_data_type_traits<T>::cpu_size);
    }

    template <RenderDataType T>
        requires Arithmetic<T>
    void s_gpu_to_cpu(const byte_t* _gpu, T& _cpu)
    {
        std::memcpy(&_cpu, _gpu, render_data_type_traits<T>::gpu_size);
    }

    // Conversion function vec render data types
    template <RenderDataType T, size_t ELEM_COUNT>
    void s_cpu_to_gpu(byte_t* _gpu, const math::vec<T, ELEM_COUNT>& _cpu)
    {
        // We don't need to iterate through elems of the vec since we know that they are store contiguously
        std::memcpy(_gpu, &_cpu[0], render_data_type_traits<math::vec<T, ELEM_COUNT>>::cpu_size);
    }
    template <RenderDataType T, size_t ELEM_COUNT>
    void s_gpu_to_cpu(const byte_t* _gpu, math::vec<T, ELEM_COUNT>& _cpu)
    {
        std::memcpy(&_cpu[0], _gpu, render_data_type_traits<math::vec<T, ELEM_COUNT>>::gpu_size);
    }

    // Conversion function mat render data types
    template <RenderDataType T, size_t ROW_COUNT, size_t COLUMN_COUNT>
    void s_cpu_to_gpu(byte_t* _gpu, const math::mat<T, ROW_COUNT, COLUMN_COUNT>& _cpu)
    {
        // We need to iterate through rows of the mat since we know that they might NOT be stored contiguously
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            s_cpu_to_gpu(_gpu + render_data_type_traits<math::vec<T, COLUMN_COUNT>>::gpu_size, _cpu[r]);
        }
    }

    template <RenderDataType T, size_t ROW_COUNT, size_t COLUMN_COUNT>
    void s_gpu_to_cpu(const byte_t* _gpu, math::mat<T, ROW_COUNT, COLUMN_COUNT>& _cpu)
    {
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            s_gpu_to_cpu(_gpu + render_data_type_traits<math::vec<T, COLUMN_COUNT>>::gpu_size, _cpu[r]);
        }
    }

    // for bool the check is not as strict as for the others because it does not matter
    static_assert(render_data_type_traits<bool>::cpu_size <= render_data_type_traits<bool>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<int32_t>::cpu_size == render_data_type_traits<int32_t>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<int64_t>::cpu_size == render_data_type_traits<int64_t>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<uint32_t>::cpu_size == render_data_type_traits<uint32_t>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<uint64_t>::cpu_size == render_data_type_traits<uint64_t>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<float>::cpu_size == render_data_type_traits<float>::gpu_size, "mismatch");
    static_assert(render_data_type_traits<double>::cpu_size == render_data_type_traits<double>::gpu_size, "mismatch");

} // namespace veer::display::render
