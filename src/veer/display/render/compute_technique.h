#pragma once

#include <veer/core/core.h>

#include <veer/display/render/shader_source.h>
#include <veer/display/render/technique.h>

#if defined(D3D12_RENDER_BACKEND)
#include <veer/display/render/backends/dx12/dx12_pch.h>
#endif // defined(D3D12_RENDER_BACKEND)

namespace veer::display::render
{
    class render_device;

    class compute_technique : public technique
    {
    public:
        compute_technique(
            const constant_buffer_definition& _frame_constant_buffer_definition,
            const constant_buffer_definition& _material_constant_buffer_definition,
            const render_device& _device,
            const shader_stage_source_container_t& _source_code
        );
        virtual ~compute_technique();

#include VEER_INL(compute_technique)
    };
} // namespace veer::display::render