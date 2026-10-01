#include <display/render/compute_technique.h>

#include "dx12_pch.h"

#include <core/core.h>

#include <display/render/blend_state.h>
#include <display/render/constant_buffer.h>
#include <display/render/render_device.h>
#include <display/render/shader_source.h>
#include <display/render/technique.h>

#include <display/render/backends/dx12/dx12_render_device_data_format.h>
#include <display/render/backends/dx12/dx12_technique_helpers.h>

#define VEER_DX12_BINDLESS_TABLE_SIZE 1024u
#define VEER_DX12_BINDLESS_TEXTURE_2D_TABLE_SPACE 1u

namespace veer::display::render
{
    compute_technique::compute_technique(
        const constant_buffer_definition& _frame_constant_buffer_definition,
        const constant_buffer_definition& _material_constant_buffer_definition, const render_device& _device,
        const shader_stage_source_container_t& _source_code
    )
        : technique(_frame_constant_buffer_definition, _material_constant_buffer_definition)
    {
        m_root_signature = s_create_root_signature(_device);

        const shader_code_memory_blob_t& shader_cs = _source_code[static_cast<size_t>(shader_stage_type::compute)];
        VEER_ASSERT(!shader_cs.empty(), "No compute shader source code supplied");

        D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.pRootSignature = m_root_signature.Get();
        pso_desc.CS.pShaderBytecode = shader_cs.data();
        pso_desc.CS.BytecodeLength = shader_cs.size();

        VEER_LOG("CreateComputePipelineState");
        HRESULT hr = _device.get_api_handle()->CreateComputePipelineState(&pso_desc, IID_PPV_ARGS(&m_pso));
        containers::string hr_string = HrToString(hr);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create PSO. Error (" << hr_string.c_str() << ")");
    }

    compute_technique::~compute_technique()
    {
    }

    ID3D12PipelineState* compute_technique::get_pipeline_state_object() const
    {
        return m_pso.Get();
    }

    ID3D12RootSignature* compute_technique::get_root_signature() const
    {
        return m_root_signature.Get();
    }
} // namespace veer::display::render