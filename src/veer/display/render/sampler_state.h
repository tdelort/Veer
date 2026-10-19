#pragma once

namespace veer::display::render
{
    struct sampler_desc
    {
        enum class filter
        {
            point,
            linear,
            anisotropic,
            COUNT
        };

        enum class address_mode
        {
            clamp,
            wrap,
            mirror,
            COUNT
        };

        filter m_filter;
        address_mode m_uvw_address_mode;
    };

    static constexpr size_t s_max_anisotropy_level = 8;

    static constexpr sampler_desc s_static_samplers[] = {
        // point
        {sampler_desc::filter::point,       sampler_desc::address_mode::clamp },
        {sampler_desc::filter::point,       sampler_desc::address_mode::wrap  },
        {sampler_desc::filter::point,       sampler_desc::address_mode::mirror},
        // linear
        {sampler_desc::filter::linear,      sampler_desc::address_mode::clamp },
        {sampler_desc::filter::linear,      sampler_desc::address_mode::wrap  },
        {sampler_desc::filter::linear,      sampler_desc::address_mode::mirror},
        // aniso
        {sampler_desc::filter::anisotropic, sampler_desc::address_mode::clamp },
        {sampler_desc::filter::anisotropic, sampler_desc::address_mode::wrap  },
        {sampler_desc::filter::anisotropic, sampler_desc::address_mode::mirror},
    };
} // namespace veer::display::render