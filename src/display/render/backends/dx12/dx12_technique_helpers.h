#pragma once

#include "display/render/blend_state.h"
#include "display/render/rasterizer_state.h"
#include "display/render/shader_source.h"
#include "dx12_pch.h"
#include "display/render/technique.h"
#include "display/render/constant_buffer.h"

namespace veer::display::render
{
    class render_device;

	D3D12_INPUT_ELEMENT_DESC s_convert(shader_signature::input_elem _input_elem);
	D3D12_BLEND s_convert(blend_state::factor _blend_factor);
	D3D12_BLEND_OP s_convert(blend_state::operation _blend_operation);
	D3D12_RENDER_TARGET_BLEND_DESC s_convert(blend_state _blend_state);
	D3D12_COLOR_WRITE_ENABLE  s_convert(blend_state::write_mask _write_mask);

	D3D12_CULL_MODE s_convert(rasterizer_state::cull_mode _cull_mode);
	D3D12_RASTERIZER_DESC s_convert(rasterizer_state _rasterizer_state);

    // One big root signature to rule them all so this can be static
    size_t s_get_root_param_index(constant_buffer_type _constant_buffer);

    ComPtr<ID3D12RootSignature> s_create_root_signature(const render_device& _device);
}