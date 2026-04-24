#pragma once

#include "display/render/blend_state.h"
#include "display/render/rasterizer_state.h"
#include "display/render/shader_source.h"
#include "dx12_pch.h"
#include "display/render/technique.h"
#include <d3d12.h>

namespace veer::display::render
{
    class dx12_render_device;

	static D3D12_INPUT_ELEMENT_DESC s_convert( shader_signature::input_elem _input_elem );
	static D3D12_BLEND s_convert( blend_state::factor _blend_factor );
	static D3D12_BLEND_OP s_convert( blend_state::operation _blend_operation );
	static D3D12_RENDER_TARGET_BLEND_DESC s_convert( blend_state _blend_state );
	static D3D12_COLOR_WRITE_ENABLE  s_convert( blend_state::write_mask _write_mask );

	static D3D12_CULL_MODE s_convert( rasterizer_state::cull_mode _cull_mode );
	static D3D12_RASTERIZER_DESC s_convert( rasterizer_state _rasterizer_state );




    class dx12_graphics_technique : public graphics_technique
    {
    public:
        dx12_graphics_technique(dx12_render_device& _device, const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state);
        virtual ~dx12_graphics_technique();

        ID3D12PipelineState* get_pipeline_state_object() const;
        ID3D12RootSignature* get_root_signature() const;
    private:
        ComPtr<ID3D12RootSignature> m_root_signature;
        ComPtr<ID3D12PipelineState> m_pso;
    };



    class dx12_compute_technique : public compute_technique
    {
    public:
        dx12_compute_technique(dx12_render_device& _device, const shader_stage_source_container_t& _source_code);
        virtual ~dx12_compute_technique();

        ID3D12PipelineState* get_pipeline_state_object() const;
        ID3D12RootSignature* get_root_signature() const;
    private:
        ComPtr<ID3D12RootSignature> m_root_signature;
        ComPtr<ID3D12PipelineState> m_pso;
    };
}