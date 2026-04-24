#pragma once

#include <core/core.h>
#include <display/render/shader_source.h>

namespace veer::display::render
{
    class graphics_technique
    {
    public:
        graphics_technique(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state );
        virtual ~graphics_technique();
    };

    class compute_technique
    {
    public:
        compute_technique( const shader_stage_source_container_t& _source_code );
        virtual ~compute_technique();
    };
}