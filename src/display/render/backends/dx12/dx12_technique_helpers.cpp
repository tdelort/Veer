#include "dx12_technique_helpers.h"

#include "dx12_pch.h"
#include <core/constexpr_table.h>
#include <display/render/backends/dx12/dx12_render_device_data_format.h>
#include <display/render/render_device.h>
#include <display/render/sampler_state.h>

namespace veer::display::render
{
    D3D12_INPUT_ELEMENT_DESC s_convert(shader_signature::input_elem _input_elem)
    {
        D3D12_INPUT_ELEMENT_DESC dx12_input_elem = {};
        dx12_input_elem.SemanticName = _input_elem.m_semantic_name;
        dx12_input_elem.SemanticIndex = _input_elem.m_semantic_index;
        dx12_input_elem.Format = s_convert(_input_elem.m_format);

        dx12_input_elem.InputSlot = 0u;
        dx12_input_elem.AlignedByteOffset = _input_elem.m_offset; // D3D12_APPEND_ALIGNED_ELEMENT;
        dx12_input_elem.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        dx12_input_elem.InstanceDataStepRate = 0;

        return dx12_input_elem;
    }

    D3D12_BLEND s_convert(blend_state::factor _blend_factor)
    {
        using conversion_table_t = constexpr_conversion_table<
            blend_state::factor,
            D3D12_BLEND,
            {blend_state::factor::zero, D3D12_BLEND_ZERO},
            {blend_state::factor::one, D3D12_BLEND_ONE},
            {blend_state::factor::src_color, D3D12_BLEND_SRC_COLOR},
            {blend_state::factor::one_minus_src_color, D3D12_BLEND_INV_SRC_COLOR},
            {blend_state::factor::dst_color, D3D12_BLEND_DEST_COLOR},
            {blend_state::factor::one_minus_dst_color, D3D12_BLEND_INV_DEST_COLOR},
            {blend_state::factor::src_alpha, D3D12_BLEND_SRC_ALPHA},
            {blend_state::factor::one_minus_src_alpha, D3D12_BLEND_INV_SRC_ALPHA},
            {blend_state::factor::dst_alpha, D3D12_BLEND_DEST_ALPHA},
            {blend_state::factor::one_minus_dst_alpha, D3D12_BLEND_INV_DEST_ALPHA},
            {blend_state::factor::constant_color, D3D12_BLEND_BLEND_FACTOR},
            {blend_state::factor::one_minus_constant_color, D3D12_BLEND_INV_BLEND_FACTOR},
            {blend_state::factor::constant_alpha, D3D12_BLEND_ALPHA_FACTOR},
            {blend_state::factor::one_minus_constant_alpha, D3D12_BLEND_INV_ALPHA_FACTOR},
            {blend_state::factor::src_alpha_saturate, D3D12_BLEND_SRC_ALPHA_SAT},
            {blend_state::factor::src1_color, D3D12_BLEND_SRC1_COLOR},
            {blend_state::factor::one_minus_src1_color, D3D12_BLEND_INV_SRC1_COLOR},
            {blend_state::factor::src1_alpha, D3D12_BLEND_SRC1_ALPHA},
            {blend_state::factor::one_minus_src1_alpha, D3D12_BLEND_INV_SRC1_ALPHA}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(blend_state::factor::COUNT), "");
        return conversion_table_t::get(_blend_factor);
    }

    D3D12_BLEND_OP s_convert(blend_state::operation _blend_operation)
    {
        using conversion_table_t = constexpr_conversion_table<
            blend_state::operation,
            D3D12_BLEND_OP,
            {blend_state::operation::add, D3D12_BLEND_OP_ADD},
            {blend_state::operation::subtract, D3D12_BLEND_OP_SUBTRACT},
            {blend_state::operation::reverse_subtract, D3D12_BLEND_OP_REV_SUBTRACT},
            {blend_state::operation::min, D3D12_BLEND_OP_MIN},
            {blend_state::operation::max, D3D12_BLEND_OP_MAX}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(blend_state::operation::COUNT), "");
        return conversion_table_t::get(_blend_operation);
    }

    D3D12_COLOR_WRITE_ENABLE s_convert(blend_state::write_mask _write_mask)
    {
        return D3D12_COLOR_WRITE_ENABLE(
            (flags::get(_write_mask, blend_state::write_mask::r) ? D3D12_COLOR_WRITE_ENABLE_RED : 0u) |
            (flags::get(_write_mask, blend_state::write_mask::g) ? D3D12_COLOR_WRITE_ENABLE_GREEN : 0u) |
            (flags::get(_write_mask, blend_state::write_mask::b) ? D3D12_COLOR_WRITE_ENABLE_BLUE : 0u) |
            (flags::get(_write_mask, blend_state::write_mask::a) ? D3D12_COLOR_WRITE_ENABLE_ALPHA : 0u)
        );
    }

    D3D12_RENDER_TARGET_BLEND_DESC s_convert(blend_state _blend_state)
    {
        D3D12_RENDER_TARGET_BLEND_DESC dx12_blend_desc = {};
        dx12_blend_desc.BlendEnable = _blend_state.m_enable;
        dx12_blend_desc.SrcBlend = s_convert(_blend_state.m_src_color_blend_factor);
        dx12_blend_desc.DestBlend = s_convert(_blend_state.m_dst_color_blend_factor);
        dx12_blend_desc.BlendOp = s_convert(_blend_state.m_color_blend_operation);

        dx12_blend_desc.SrcBlendAlpha = s_convert(_blend_state.m_src_alpha_blend_factor);
        dx12_blend_desc.DestBlendAlpha = s_convert(_blend_state.m_dst_alpha_blend_factor);
        dx12_blend_desc.BlendOpAlpha = s_convert(_blend_state.m_alpha_blend_operation);

        dx12_blend_desc.RenderTargetWriteMask = s_convert(_blend_state.m_write_mask);

        // not yet supported
        dx12_blend_desc.LogicOpEnable = false;
        dx12_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;

        return dx12_blend_desc;
    }

    D3D12_CULL_MODE s_convert(rasterizer_state::cull_mode _cull_mode)
    {
        using conversion_table_t = constexpr_conversion_table<
            rasterizer_state::cull_mode,
            D3D12_CULL_MODE,
            {rasterizer_state::cull_mode::none, D3D12_CULL_MODE_NONE},
            {rasterizer_state::cull_mode::front, D3D12_CULL_MODE_FRONT},
            {rasterizer_state::cull_mode::back, D3D12_CULL_MODE_BACK}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(rasterizer_state::cull_mode::COUNT), "");

        return conversion_table_t::get(_cull_mode);
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

    D3D12_COMPARISON_FUNC s_convert(depth_state::func _depth_comparison_func)
    {
        using conversion_table_t = constexpr_conversion_table<
            depth_state::func,
            D3D12_COMPARISON_FUNC,
            {depth_state::func::none, D3D12_COMPARISON_FUNC_NONE},
            {depth_state::func::never, D3D12_COMPARISON_FUNC_NEVER},
            {depth_state::func::equal, D3D12_COMPARISON_FUNC_EQUAL},
            {depth_state::func::not_equal, D3D12_COMPARISON_FUNC_NOT_EQUAL},
            {depth_state::func::less, D3D12_COMPARISON_FUNC_LESS},
            {depth_state::func::less_equal, D3D12_COMPARISON_FUNC_LESS_EQUAL},
            {depth_state::func::greater, D3D12_COMPARISON_FUNC_GREATER},
            {depth_state::func::greater_equal, D3D12_COMPARISON_FUNC_GREATER_EQUAL},
            {depth_state::func::always, D3D12_COMPARISON_FUNC_ALWAYS}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(depth_state::func::COUNT));

        return conversion_table_t::get(_depth_comparison_func);
    }

    D3D12_FILTER s_convert(sampler_desc::filter _filter)
    {
        using conversion_table_t = constexpr_conversion_table<
            sampler_desc::filter,
            D3D12_FILTER,
            {sampler_desc::filter::point, D3D12_FILTER_MIN_MAG_MIP_POINT},
            {sampler_desc::filter::linear, D3D12_FILTER_MIN_MAG_MIP_LINEAR},
            {sampler_desc::filter::anisotropic, D3D12_FILTER_ANISOTROPIC}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(sampler_desc::filter::COUNT));

        return conversion_table_t::get(_filter);
    }

    D3D12_TEXTURE_ADDRESS_MODE s_convert(sampler_desc::address_mode _mode)
    {
        using conversion_table_t = constexpr_conversion_table<
            sampler_desc::address_mode,
            D3D12_TEXTURE_ADDRESS_MODE,
            {sampler_desc::address_mode::clamp, D3D12_TEXTURE_ADDRESS_MODE_CLAMP},
            {sampler_desc::address_mode::wrap, D3D12_TEXTURE_ADDRESS_MODE_WRAP},
            {sampler_desc::address_mode::mirror, D3D12_TEXTURE_ADDRESS_MODE_MIRROR}>;

        static_assert(conversion_table_t::size == static_cast<size_t>(sampler_desc::address_mode::COUNT));

        return conversion_table_t::get(_mode);
    }

    size_t s_get_root_param_index(constant_buffer_type _constant_buffer)
    {
        return static_cast<size_t>(_constant_buffer);
    }

    // TODO : this will be almost the same everywhere, reuse
    ComPtr<ID3D12RootSignature> s_create_root_signature(const render_device& _device)
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
        containers::static_array<D3D12_ROOT_PARAMETER, static_cast<size_t>(constant_buffer_type::COUNT)> root_parameters{};
        for (size_t constant_buffer_index = 0u; constant_buffer_index < root_parameters.size(); ++constant_buffer_index)
        {
            D3D12_ROOT_PARAMETER& param = root_parameters[s_get_root_param_index(static_cast<constant_buffer_type>(constant_buffer_index))];
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            param.Descriptor.RegisterSpace = 0;
            param.Descriptor.ShaderRegister = constant_buffer_index;
            param_index++;
        }

        containers::static_array<D3D12_STATIC_SAMPLER_DESC, VEER_STATIC_ARRAY_SIZE(s_static_samplers)> static_samplers_desc{};
        for (size_t static_sampler_index = 0u; static_sampler_index < static_samplers_desc.size(); ++static_sampler_index)
        {
            D3D12_STATIC_SAMPLER_DESC sampler_desc = {};
            sampler_desc.Filter = s_convert(s_static_samplers[static_sampler_index].m_filter);
            D3D12_TEXTURE_ADDRESS_MODE address_mode = s_convert(s_static_samplers[static_sampler_index].m_uvw_address_mode);
            sampler_desc.AddressU = address_mode;
            sampler_desc.AddressV = address_mode;
            sampler_desc.AddressW = address_mode;

            // default values
            sampler_desc.MipLODBias = 0;
            sampler_desc.MaxAnisotropy = s_max_anisotropy_level; // TODO : maybe add more aniso samplers
            sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NONE;
            sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            sampler_desc.MinLOD = 0;
            sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;

            // register things
            sampler_desc.RegisterSpace = 0;
            sampler_desc.ShaderRegister = static_sampler_index;
            sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

            static_samplers_desc[static_sampler_index] = sampler_desc;
        }

        D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
        root_signature_desc.NumParameters = root_parameters.size();
        root_signature_desc.pParameters = root_parameters.data();
        root_signature_desc.NumStaticSamplers = static_samplers_desc.size();
        root_signature_desc.pStaticSamplers = static_samplers_desc.data();
        root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED |
                                    D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;

        ComPtr<ID3DBlob> root_signature_blob, error_blob;
        HRESULT hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_signature_blob, &error_blob);
        VEER_ASSERT(SUCCEEDED(hr), "Failed to serialize root signature. Error (" << hr << ")");
        if (error_blob != nullptr)
        {
            VEER_LOG("Root signature serialization output : " << static_cast<const char*>(error_blob->GetBufferPointer()));
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
} // namespace veer::display::render