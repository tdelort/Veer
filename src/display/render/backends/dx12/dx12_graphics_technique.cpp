#include <display/render/graphics_technique.h>

#include <core/core.h>

#include <display/render/backends/dx12/dx12_technique_helpers.h>

#include <display/render/blend_state.h>
#include <display/render/constant_buffer.h>
#include <display/render/render_device.h>
#include <display/render/shader_source.h>
#include <display/render/technique.h>

#include <display/render/backends/dx12/dx12_render_device_data_format.h>

#define VEER_DX12_BINDLESS_TABLE_SIZE 1024u
#define VEER_DX12_BINDLESS_TEXTURE_2D_TABLE_SPACE 1u

namespace veer::display::render
{

    graphics_technique::graphics_technique(
        const constant_buffer_definition& _frame_constant_buffer_definition,
        const constant_buffer_definition& _material_constant_buffer_definition, const render_device& _device,
        const shader_stage_source_container_t& _source_code, const shader_signature& _signature,
        const shader_render_state& _render_state
    )
        : technique(_frame_constant_buffer_definition, _material_constant_buffer_definition)
    {
        // TODO : get from device instead of creating it each time since it will be the same for most techniques
        m_root_signature = s_create_root_signature(_device);

        containers::resizable_array<D3D12_INPUT_ELEMENT_DESC> input_elems_desc;
        for (const shader_signature::input_elem& elem : _signature.m_input_elems)
        {
            input_elems_desc.push_back(s_convert(elem));
        }

        D3D12_BLEND_DESC blend_state = {};
        blend_state.AlphaToCoverageEnable = FALSE;
        blend_state.IndependentBlendEnable = _render_state.m_blend_states.size() > 1;
        for (size_t i = 0; i < _render_state.m_blend_states.size(); ++i)
        {
            blend_state.RenderTarget[i] = s_convert(_render_state.m_blend_states[i]);
        }

        D3D12_RASTERIZER_DESC rasterizer_state = s_convert(_render_state.m_rasterizer_state);

        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.pRootSignature = m_root_signature.Get();

        const shader_code_memory_blob_t& shader_vs = _source_code[static_cast<size_t>(shader_stage_type::vertex)];
        if (!shader_vs.empty())
        {
            pso_desc.VS.pShaderBytecode = shader_vs.data();
            pso_desc.VS.BytecodeLength = shader_vs.size();
        }

        const shader_code_memory_blob_t& shader_ps = _source_code[static_cast<size_t>(shader_stage_type::pixel)];
        if (!shader_ps.empty())
        {
            pso_desc.PS.pShaderBytecode = shader_ps.data();
            pso_desc.PS.BytecodeLength = shader_ps.size();
        }

        pso_desc.RasterizerState = rasterizer_state;
        pso_desc.BlendState = blend_state;
        pso_desc.InputLayout.NumElements = input_elems_desc.size();
        pso_desc.InputLayout.pInputElementDescs = input_elems_desc.data();

        // TODO depth state
        pso_desc.DepthStencilState.DepthEnable = _render_state.m_depth_enable;
        pso_desc.DepthStencilState.StencilEnable = _render_state.m_stencil_enable;

        // TODO other state data
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.SampleMask = UINT_MAX;
        pso_desc.SampleDesc.Count = 1;

        for (size_t i = 0; i < _signature.m_output_elems.size(); ++i)
        {
            pso_desc.RTVFormats[i] = s_convert(_signature.m_output_elems[i].m_format);
        }
        pso_desc.NumRenderTargets = _signature.m_output_elems.size();

        VEER_LOG("CreateGraphicsPipelineState");
        HRESULT hr = _device.get_api_handle()->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pso));
        if (!SUCCEEDED(hr))
        {
            containers::string hr_string = HrToString(hr);
            VEER_LOG_ERROR("Failed to create PSO. Error (" << hr_string.c_str() << ")");
        }
    }

    graphics_technique::~graphics_technique()
    {
    }

    ID3D12PipelineState* graphics_technique::get_pipeline_state_object() const
    {
        return m_pso.Get();
    }

    ID3D12RootSignature* graphics_technique::get_root_signature() const
    {
        return m_root_signature.Get();
    }
} // namespace veer::display::render