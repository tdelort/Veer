#pragma once

#include "dx12_pch.h"
#include <veer/core/core.h>


#include <veer/display/render/render_device_resource_sync_state.h>

namespace veer::display::render
{
    static D3D12_RESOURCE_STATES s_convert(render_device_resource_sync_state _states)
    {
        static constexpr std::pair<D3D12_RESOURCE_STATES, render_device_resource_sync_state> s_conversionArray[] = {
            {D3D12_RESOURCE_STATE_COMMON,                     render_device_resource_sync_state::Common                 },
            {D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,      render_device_resource_sync_state::PixelShaderResource    },
            {D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,  render_device_resource_sync_state::NonPixelShaderResource },
            {D3D12_RESOURCE_STATE_UNORDERED_ACCESS,           render_device_resource_sync_state::UnorderedAccessView    },
            {D3D12_RESOURCE_STATE_PRESENT,                    render_device_resource_sync_state::Present                },
            {D3D12_RESOURCE_STATE_RENDER_TARGET,              render_device_resource_sync_state::RenderTarget           },
            {D3D12_RESOURCE_STATE_DEPTH_WRITE,                render_device_resource_sync_state::DepthWrite             },
            {D3D12_RESOURCE_STATE_DEPTH_READ,                 render_device_resource_sync_state::DepthRead              },
            {D3D12_RESOURCE_STATE_COPY_SOURCE,                render_device_resource_sync_state::CopySource             },
            {D3D12_RESOURCE_STATE_COPY_DEST,                  render_device_resource_sync_state::CopyDest               },
            {D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, render_device_resource_sync_state::VertexAndConstantBuffer},
            {D3D12_RESOURCE_STATE_INDEX_BUFFER,               render_device_resource_sync_state::IndexBuffer            },
            {D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,          render_device_resource_sync_state::IndirectArgs           }
        };

        unsigned int input = (unsigned int)_states;
        D3D12_RESOURCE_STATES output{};

        for (size_t i = 0; i < VEER_STATIC_ARRAY_SIZE(s_conversionArray); ++i)
        {
            const std::pair<D3D12_RESOURCE_STATES, render_device_resource_sync_state> mapping = s_conversionArray[i];

            if (input & (unsigned int)mapping.second)
                output |= mapping.first;
        }

        return output;
    }
} // namespace veer::display::render