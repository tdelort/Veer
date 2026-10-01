#pragma once

#include <core/core.h>
#include <core/math/vec.h>
#include <display/render/render_device_data_format.h>

namespace veer::display::render
{
    template <size_t DIMENSION>
    struct texture_desc_generic
    {
        static constexpr size_t s_dimension = DIMENSION;
        using size_type = veer::math::vec<size_t, s_dimension>;

        enum class usage_flags : uint8_t
        {
            storage = 1 << 0,
            render_target = 1 << 1,
            depth_stencil = 1 << 2,
        };

        bool operator==(const texture_desc_generic<DIMENSION>& _other) const
        {
            return all(m_size == _other.m_size) && (m_format == _other.m_format) && (m_flags == _other.m_flags);
        }

        size_type m_size{0u};
        render_device_data_format m_format{render_device_data_format::unknown};
        usage_flags m_flags{0u};
    };

    using texture_2d_desc = texture_desc_generic<2u>;
    using texture_3d_desc = texture_desc_generic<3u>;

    VEER_ENUM_CLASS_FLAG_OPERATORS(texture_2d_desc::usage_flags);
    VEER_ENUM_CLASS_FLAG_OPERATORS(texture_3d_desc::usage_flags);

    static_assert(std::is_trivially_destructible<veer::math::vec<size_t,3>>::value, "da hell");
    static_assert(std::is_trivially_destructible<texture_2d_desc>::value, "da hell");

    struct buffer_desc
    {
        enum class usage_flags : uint8_t
        {
            index = 1 << 0,
            vertex = 1 << 1,
            constant = 1 << 2,
            storage = 1 << 3,
            indirect_args = 1 << 4
        };
        using size_type = size_t;

        bool operator==(const buffer_desc& _other) const
        {
            return (m_size == _other.m_size) && (m_stride == _other.m_stride) && (m_flags == _other.m_flags);
        }

        usage_flags m_flags{0u};
        size_type m_size{0u};   // number of elements
        size_type m_stride{0u}; // number of values of type format per elements
    };

    VEER_ENUM_CLASS_FLAG_OPERATORS(buffer_desc::usage_flags)
} // namespace veer::display::render