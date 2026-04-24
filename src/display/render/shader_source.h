#pragma once

#include "core/containers/resizable_array.h"
#include <core/core.h>
#include <core/containers/span.h>
#include <core/containers/string.h>
#include <core/containers/static_array.h>
#include <display/render/blend_state.h>
#include <display/render/rasterizer_state.h>
#include <display/render/render_device_data_format.h>
#include <memory>

namespace veer::display::render
{
    enum class shader_stage_type
    {
        vertex,
        pixel,
        compute,

        // add others when needed
        COUNT
    };

    struct shader_signature
    {
        struct input_elem
        {
            // very DX12, for vulkan use only the order
            const char* m_semantic_name;
            size_t m_semantic_index;
            render_device_data_format m_format;
            // uint32_t m_offset;
        };

        struct output_elem
        {
            render_device_data_format m_format;
        };

        // input / output
        veer::containers::span<input_elem> m_input_elems;
        veer::containers::span<output_elem> m_output_elems;
    };

    struct shader_render_state
    {
        veer::containers::span<blend_state> m_blend_states;
        rasterizer_state m_rasterizer_state;
        // depth_state ? m_depth_enable / m_stencil_enable ?
    };

    using shader_code_memory_blob_t = veer::containers::resizable_array<byte_t>;
    using shader_stage_source_container_t = veer::containers::static_array<shader_code_memory_blob_t, static_cast<size_t>(shader_stage_type::COUNT)>;

    struct shader_source
    {
        shader_source( const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state );
    };
}