#include "dx12_technique.h"

#include "core/containers/static_array.h"
#include "core/core.h"
#include "display/render/blend_state.h"
#include "display/render/constant_buffer.h"
#include "display/render/technique.h"
#include "display/render/shader_source.h"
#include "dx12_pch.h"
#include "dx12_render_device.h"
#include "dx12_render_device_data_format.h"


#define VEER_DX12_BINDLESS_TABLE_SIZE 1024u
#define VEER_DX12_BINDLESS_TEXTURE_2D_TABLE_SPACE 1u

namespace veer::display::render
{
	D3D12_INPUT_ELEMENT_DESC s_convert(shader_signature::input_elem _input_elem)
    {
        D3D12_INPUT_ELEMENT_DESC dx12_input_elem = {};
        dx12_input_elem.SemanticName = _input_elem.m_semantic_name;
        dx12_input_elem.SemanticIndex = _input_elem.m_semantic_index;
        dx12_input_elem.Format = s_convert( _input_elem.m_format );

        dx12_input_elem.InputSlot = 0u;
        dx12_input_elem.AlignedByteOffset = _input_elem.m_offset; // D3D12_APPEND_ALIGNED_ELEMENT;
        dx12_input_elem.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        dx12_input_elem.InstanceDataStepRate = 0;

        return dx12_input_elem;
    }

	D3D12_BLEND s_convert(blend_state::factor _blend_factor)
    {
        D3D12_BLEND s_conversionTable[] =
        {
            D3D12_BLEND_ZERO,               // zero,
            D3D12_BLEND_ONE,                // one,
            D3D12_BLEND_SRC_COLOR,          // src_color, 
            D3D12_BLEND_INV_SRC_COLOR,      // one_minus_src_color, 
            D3D12_BLEND_DEST_COLOR,         // dst_color, 
            D3D12_BLEND_INV_DEST_COLOR,     // one_minus_dst_color, 
            D3D12_BLEND_SRC_ALPHA,          // src_alpha, 
            D3D12_BLEND_INV_SRC_ALPHA,      // one_minus_src_alpha, 
            D3D12_BLEND_DEST_ALPHA,         // dst_alpha, 
            D3D12_BLEND_INV_DEST_ALPHA,     // one_minus_dst_alpha, 
            D3D12_BLEND_BLEND_FACTOR,       // constant_color,
            D3D12_BLEND_INV_BLEND_FACTOR,   // one_minus_constant_color,
            D3D12_BLEND_ALPHA_FACTOR,       // constant_alpha,
            D3D12_BLEND_INV_ALPHA_FACTOR,   // one_minus_constant_alpha,
            D3D12_BLEND_SRC_ALPHA_SAT,      // src_alpha_saturate, 
            D3D12_BLEND_SRC1_COLOR,         // src1_color, 
            D3D12_BLEND_INV_SRC1_COLOR,     // one_minus_src1_color, 
            D3D12_BLEND_SRC1_ALPHA,         // src1_alpha, 
            D3D12_BLEND_INV_SRC1_ALPHA      // one_minus_src1_alpha, 
        };

        static_assert( VEER_STATIC_ARRAY_SIZE(s_conversionTable) == static_cast<size_t>(blend_state::factor::COUNT) );

        return s_conversionTable[static_cast<size_t>(_blend_factor)];
    }

	D3D12_BLEND_OP s_convert(blend_state::operation _blend_operation)
    {
        D3D12_BLEND_OP s_conversionTable[] =
        {
            D3D12_BLEND_OP_ADD,             // add
            D3D12_BLEND_OP_SUBTRACT,        // subtract
            D3D12_BLEND_OP_REV_SUBTRACT,    // reverse_subtract
            D3D12_BLEND_OP_MIN,             // min
            D3D12_BLEND_OP_MAX,             // max
        };
        static_assert( VEER_STATIC_ARRAY_SIZE(s_conversionTable) == static_cast<size_t>(blend_state::operation::COUNT) );

        return s_conversionTable[static_cast<size_t>(_blend_operation)];
    }

	D3D12_COLOR_WRITE_ENABLE s_convert( blend_state::write_mask _write_mask )
    {
        return D3D12_COLOR_WRITE_ENABLE( 
                 ( flags::get( _write_mask, blend_state::write_mask::r ) ? D3D12_COLOR_WRITE_ENABLE_RED : 0u ) 
               | ( flags::get( _write_mask, blend_state::write_mask::g ) ? D3D12_COLOR_WRITE_ENABLE_GREEN : 0u )
               | ( flags::get( _write_mask, blend_state::write_mask::b ) ? D3D12_COLOR_WRITE_ENABLE_BLUE : 0u )
               | ( flags::get( _write_mask, blend_state::write_mask::a ) ? D3D12_COLOR_WRITE_ENABLE_ALPHA : 0u )
        );
    }

	D3D12_RENDER_TARGET_BLEND_DESC s_convert( blend_state _blend_state )
    {
        D3D12_RENDER_TARGET_BLEND_DESC dx12_blend_desc = {};
        dx12_blend_desc.BlendEnable = _blend_state.m_enable;
        dx12_blend_desc.SrcBlend = s_convert( _blend_state.m_src_color_blend_factor );
        dx12_blend_desc.DestBlend = s_convert( _blend_state.m_dst_color_blend_factor );
        dx12_blend_desc.BlendOp = s_convert( _blend_state.m_color_blend_operation );

        dx12_blend_desc.SrcBlendAlpha = s_convert( _blend_state.m_src_alpha_blend_factor );
        dx12_blend_desc.DestBlendAlpha = s_convert( _blend_state.m_dst_alpha_blend_factor );
        dx12_blend_desc.BlendOpAlpha = s_convert( _blend_state.m_alpha_blend_operation );

        dx12_blend_desc.RenderTargetWriteMask = s_convert( _blend_state.m_write_mask );

        // not yet supported
        dx12_blend_desc.LogicOpEnable = false; 
        dx12_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;

        return dx12_blend_desc;
    }

	D3D12_CULL_MODE s_convert(rasterizer_state::cull_mode _cull_mode)
    {
        D3D12_CULL_MODE s_conversionTable[] =
        {
            D3D12_CULL_MODE_NONE,   // none
            D3D12_CULL_MODE_FRONT,  // front
            D3D12_CULL_MODE_BACK,   // back
        };
        static_assert( VEER_STATIC_ARRAY_SIZE(s_conversionTable) == static_cast<size_t>(rasterizer_state::cull_mode::COUNT) );

        return s_conversionTable[static_cast<size_t>(_cull_mode)];
    }

	D3D12_RASTERIZER_DESC s_convert(rasterizer_state _rasterizer_state)
    {
        D3D12_RASTERIZER_DESC dx12_rasterizer_state_desc = {};      
        dx12_rasterizer_state_desc.FillMode = _rasterizer_state.m_wireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
        dx12_rasterizer_state_desc.CullMode = s_convert(_rasterizer_state.m_cull_mode);
        dx12_rasterizer_state_desc.FrontCounterClockwise = FALSE;
        dx12_rasterizer_state_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        dx12_rasterizer_state_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        dx12_rasterizer_state_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        dx12_rasterizer_state_desc.DepthClipEnable = TRUE;
        dx12_rasterizer_state_desc.MultisampleEnable = FALSE;
        dx12_rasterizer_state_desc.AntialiasedLineEnable = FALSE;
        dx12_rasterizer_state_desc.ForcedSampleCount = 0;
        dx12_rasterizer_state_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        return dx12_rasterizer_state_desc; 
    }

    size_t s_get_root_param_index(constant_buffer_type _constant_buffer)
    {
        return static_cast<size_t>(_constant_buffer);
    }

	inline containers::string HrToString(HRESULT hr)
	{
		char s_str[64] = {};
		sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
		containers::string output;
        output = s_str;
        return output;
	}

    // TODO : this will be almost the same everywhere, reuse 
    ComPtr<ID3D12RootSignature> s_create_root_signature(dx12_render_device& _device)
    {
#if 0
        // TODO : description could be brought up in the platform agnostic part 
        struct bindless_resource_table_desc
        {
            D3D12_DESCRIPTOR_RANGE_TYPE m_type;
            size_t m_space;
            size_t m_size;
        };

        static constexpr size_t bindless_resource_table_count = 4u;
        static constexpr containers::static_array<bindless_resource_table_desc, bindless_resource_table_count> resource_table_descs =
        {
            // TEX 2D
            bindless_resource_table_desc(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, VEER_DX12_BINDLESS_TABLE_SIZE),
            bindless_resource_table_desc(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2u, VEER_DX12_BINDLESS_TABLE_SIZE),
            // TEX 3D
            bindless_resource_table_desc(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3u, VEER_DX12_BINDLESS_TABLE_SIZE),
            bindless_resource_table_desc(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 4u, VEER_DX12_BINDLESS_TABLE_SIZE)
        };

        // resource tables
        for(const bindless_resource_table_desc& desc : resource_table_descs)
        {
            D3D12_DESCRIPTOR_RANGE range;
            range.BaseShaderRegister = 0;
            range.NumDescriptors = desc.m_size;
            range.OffsetInDescriptorsFromTableStart = 0;
            range.RangeType = desc.m_type;
            range.RegisterSpace = desc.m_space;
                            
            D3D12_ROOT_PARAMETER& param = root_parameters[param_index];
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            param.DescriptorTable.NumDescriptorRanges = 1;
            param.DescriptorTable.pDescriptorRanges = &range;

            param_index++;
        }
#endif // 0

        // Build root signature using above config
        size_t param_index = 0u;
        containers::static_array<D3D12_ROOT_PARAMETER, static_cast<size_t>(constant_buffer_type::COUNT)> root_parameters;
        // constant buffers
        for(size_t constant_buffer_index = 0u; constant_buffer_index < static_cast<size_t>(constant_buffer_type::COUNT); ++constant_buffer_index)
        {
            D3D12_ROOT_PARAMETER& param = root_parameters[s_get_root_param_index(static_cast<constant_buffer_type>(constant_buffer_index))];
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            param.Descriptor.RegisterSpace = 0;
            param.Descriptor.ShaderRegister = constant_buffer_index;
            param_index++;
        }


        D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
        root_signature_desc.NumParameters = root_parameters.size();
        root_signature_desc.pParameters = root_parameters.data();
        root_signature_desc.NumStaticSamplers = 0u;
        root_signature_desc.pStaticSamplers = nullptr;
        root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED | D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;

        ComPtr<ID3DBlob> root_signature_blob, error_blob;
        HRESULT hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_signature_blob, &error_blob);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to serialize root signature. Error (" << hr << ")");
        if( error_blob != nullptr )
        {
            VEER_LOG( "Root signature serialization output : " << (const char*)error_blob->GetBufferPointer() );
            error_blob->Release();
        }

        const void* root_signature_data = root_signature_blob->GetBufferPointer();
        size_t root_signature_data_size = root_signature_blob->GetBufferSize();

		ComPtr<ID3D12RootSignature> root_signature_obj;
        VEER_LOG("CreateRootSignature");
        hr = _device.get_api_handle()->CreateRootSignature(0, root_signature_data, root_signature_data_size, IID_PPV_ARGS(&root_signature_obj));
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create root signature. Error (" << hr << ")");

        return root_signature_obj;
    }



    dx12_graphics_technique::dx12_graphics_technique(dx12_render_device& _device, const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state)
        : graphics_technique(_source_code, _signature, _render_state)
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
        
        // TODO : GRAPHICS PSO only

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
        pso_desc.DepthStencilState.DepthEnable = FALSE;
        pso_desc.DepthStencilState.StencilEnable = FALSE;

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

    dx12_graphics_technique::~dx12_graphics_technique()
    {
    }

    ID3D12PipelineState* dx12_graphics_technique::get_pipeline_state_object() const
    {
        return m_pso.Get();
    }

    ID3D12RootSignature* dx12_graphics_technique::get_root_signature() const
    {
        return m_root_signature.Get();
    }
    

    dx12_compute_technique::dx12_compute_technique(dx12_render_device& _device, const shader_stage_source_container_t& _source_code)
        : compute_technique(_source_code)
    {
		m_root_signature = s_create_root_signature(_device);
        
        D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.pRootSignature = m_root_signature.Get();
        pso_desc.CS.pShaderBytecode = _source_code.data();
        pso_desc.CS.BytecodeLength = _source_code.size();

        VEER_LOG("CreateComputePipelineState");
        HRESULT hr = _device.get_api_handle()->CreateComputePipelineState(&pso_desc, IID_PPV_ARGS(&m_pso));
        containers::string hr_string = HrToString(hr);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to create PSO. Error (" << hr_string.c_str() << ")");
    }

    dx12_compute_technique::~dx12_compute_technique()
    {
    }

    ID3D12PipelineState* dx12_compute_technique::get_pipeline_state_object() const
    {
        return m_pso.Get();
    }

    ID3D12RootSignature* dx12_compute_technique::get_root_signature() const
    {
        return m_root_signature.Get();
    }
}