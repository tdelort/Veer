#pragma once

#include <core/core.h>
#include <display/render/shader_source.h>
#include <display/render/technique.h>

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#endif // defined(D3D12_RENDER_BACKEND)

namespace veer::display::render
{
    class render_device;

    class graphics_technique : public technique
    {
    public:
        graphics_technique(
            const constant_buffer_definition& _frame_constant_buffer_definition,
            const constant_buffer_definition& _material_constant_buffer_definition, const render_device& _device,
            const shader_stage_source_container_t& _source_code, const shader_signature& _signature,
            const shader_render_state& _render_state
        );
        virtual ~graphics_technique();

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_graphics_technique.inl"
#elif defined(VULKAN_RENDER_BACKEND)
#error not implemented
#elif defined(METAL_RENDER_BACKEND)
#error not implemented
#endif
    };
} // namespace veer::display::render