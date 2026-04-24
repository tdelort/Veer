#pragma once

#include <core/core.h>
#include <core/math/vec.h>
#include <display/render/render_device_data_format.h>

namespace veer::display::render
{
    struct resource_desc
    {
        // Add more, like mips, usage flags, etc
        render_device_data_format m_format;
    };

    struct texture_desc : public resource_desc
    {
        enum class usage_flags : uint8_t
        {
            storage         = 1 << 0,
            render_target   = 1 << 1,   
            depth_stencil   = 1 << 2,   
        };

        // Add more, like mips, usage flags, etc
        usage_flags m_flags{};
    };
    
    VEER_ENUM_CLASS_FLAG_OPERATORS( texture_desc::usage_flags )

    template<size_t DIMENSION>
    struct texture_desc_generic : public texture_desc
    {
        static constexpr size_t s_dimension = DIMENSION;
        using size_type = veer::math::vec<size_t, s_dimension>;

        size_type m_size{};
    };

    using texture_2d_desc = texture_desc_generic<2u>;
    using texture_3d_desc = texture_desc_generic<3u>;

    struct buffer_desc : public resource_desc
    {
        enum class usage_flags : uint8_t
        {
            index           = 1 << 0,
            vertex          = 1 << 1,
            constant        = 1 << 2,
            storage         = 1 << 3,
            indirect_args   = 1 << 4
        };
        using size_type = size_t;

        usage_flags m_flags;
        size_type m_size;
    };

    VEER_ENUM_CLASS_FLAG_OPERATORS( buffer_desc::usage_flags )
}