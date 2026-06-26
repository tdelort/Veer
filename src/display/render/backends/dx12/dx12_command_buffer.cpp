#include <display/render/command_buffer.h>

#include "core/debug.h"
#include "core/math/math.h"
#include "display/render/render_device_resource.h"
#include "display/render/resource_desc.h"
#include "dx12_render_device_resource_sync_state.h"
#include "dx12_render_device.h"

#include "dx12_technique.h"

namespace veer::display::render
{

// --- dx12 specific functions (declared in dx12_command_buffer.inl) ---
	D3D12_COMMAND_LIST_TYPE command_buffer::s_convert(const command_buffer::type _type)
	{
		D3D12_COMMAND_LIST_TYPE dx12_type;
		switch (_type)
		{
		case command_buffer::type::graphics:
			dx12_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case command_buffer::type::compute:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			break;
		case command_buffer::type::copy:
			dx12_type = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		default:
			VEER_ASSERT(false, "All command_buffer::type should be covered");
			break;
		}

		return dx12_type;
	}

	void command_buffer::open(dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator)
	{
		// TODO : actually use real command list type
		const D3D12_COMMAND_LIST_TYPE dx12_type = D3D12_COMMAND_LIST_TYPE_DIRECT; // s_convert(m_type);

		if (m_command_list_handle == nullptr)
		{
			const HRESULT hr = _device.get_api_handle()->CreateCommandList(0, dx12_type, _command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_command_list_handle));
			VEER_ASSERT(SUCCEEDED(hr), "Failed to create command list (" << hr << ")");
		}
		else
		{
			m_command_list_handle->Reset(_command_allocator.Get(), nullptr);
		}
	}

	void command_buffer::close()
	{
		if (m_command_list_handle != nullptr)
		{
			const HRESULT hr = m_command_list_handle->Close();
			VEER_ASSERT(SUCCEEDED(hr), "Failed to properly close command list (" << hr << ")");
		}
	}

	ID3D12GraphicsCommandList* command_buffer::get_api_handle()
	{
		return m_command_list_handle.Get();
	}

// --- command_buffer ---

	command_buffer::command_buffer(command_buffer::type _type)
		: m_type{ _type }
	{

	}

	command_buffer::~command_buffer()
	{
		// Actually not needed since it's a magic ptr
		// m_command_list_handle.Reset();
	}

	void command_buffer::transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = _resource.get_api_handle();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = display::render::s_convert(_from_state);
		barrier.Transition.StateAfter = display::render::s_convert(_to_state);
		get_api_handle()->ResourceBarrier(1, &barrier);

		_resource.get_sync_state_tracking().set_resource_state(_to_state);
	}
	

// --- copy_command_buffer ---

	copy_command_buffer::copy_command_buffer()
		: command_buffer(command_buffer::type::copy)
	{

	}

	copy_command_buffer::copy_command_buffer(command_buffer::type _type)
		: command_buffer(_type)
	{

	}

	copy_command_buffer::~copy_command_buffer()
	{
	}

	void copy_command_buffer::copy_texture(const render_device_texture_2d& _dst, const render_device_texture_2d& _src)
	{
		(void)_dst;
		(void)_src;
		VEER_ASSERT(false, "Not implemented");
	}

	void copy_command_buffer::copy_buffer(const render_device_buffer& _dst, const render_device_buffer& _src, uint64_t _num_bytes)
	{
		(void)_dst;
		(void)_src;
		(void)_num_bytes;
		VEER_ASSERT(false, "Not implemented");
	}

// --- compute_command_buffer ---

	compute_command_buffer::compute_command_buffer()
		: copy_command_buffer(command_buffer::type::compute)
	{

	}

	compute_command_buffer::compute_command_buffer(command_buffer::type _type)
		: copy_command_buffer(_type)
	{

	}

	compute_command_buffer::~compute_command_buffer()
	{
	}

	void compute_command_buffer::set_technique(compute_technique& _technique)
	{
		dx12_compute_technique& dx12_technique = static_cast<dx12_compute_technique&>(_technique);
		get_api_handle()->SetPipelineState(dx12_technique.get_pipeline_state_object());
		get_api_handle()->SetGraphicsRootSignature(dx12_technique.get_root_signature());
	}

	void compute_command_buffer::clear_texture(const render_device_texture_2d& _texture, math::vec4u _color)
	{
		(void)_texture;
		(void)_color;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::clear_texture(const render_device_texture_2d& _texture, math::vec4f _color)
	{
		(void)_texture;
		(void)_color;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::clear_texture(const render_device_texture_3d& _texture, math::vec4u _color)
	{
		(void)_texture;
		(void)_color;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::clear_texture(const render_device_texture_3d& _texture, math::vec4f _color)
	{
		(void)_texture;
		(void)_color;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::clear_buffer(const render_device_buffer& _buffer, uint32_t _value)
	{
		(void)_buffer;
		(void)_value;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::clear_buffer(const render_device_buffer& _buffer, float _value)
	{
		(void)_buffer;
		(void)_value;
		VEER_ASSERT(false, "Not implemented");
	}

	void compute_command_buffer::dispatch(size_t _x, size_t _y, size_t _z)
	{
		(void)_x, _y, _z;
		VEER_ASSERT(false, "Not implemented");
	}


// --- graphics_command_buffer ---

	graphics_command_buffer::graphics_command_buffer()
		: compute_command_buffer(command_buffer::type::graphics)
	{

	}

	graphics_command_buffer::~graphics_command_buffer()
	{
	}


	void graphics_command_buffer::set_technique(graphics_technique& _technique)
	{
		dx12_graphics_technique& dx12_technique = static_cast<dx12_graphics_technique&>(_technique);
		get_api_handle()->SetPipelineState(dx12_technique.get_pipeline_state_object());
		get_api_handle()->SetGraphicsRootSignature(dx12_technique.get_root_signature());
	}

	void graphics_command_buffer::set_render_output(render_device_texture_2d* _depth, containers::span<render_device_texture_2d*> _colors)
	{
		VEER_ASSERT(_colors.size() < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT, "Too many render targets set on command buffer for current backend (set " << _colors.size() << " but supports at most " << D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT << " )" );

		const size_t count = math::min<size_t>(_colors.size(), D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT);

		containers::static_array<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> color_descriptors{};
		for(size_t i = 0; i < count; ++i)
		{
			if(_colors[i] == nullptr)
			{
				VEER_LOG_ERROR("Invalid render target passed in set_render_output at _color[" << i << "]");
				continue;
			}

			color_descriptors[i] = _colors[i]->get_render_target_view().m_handle;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE depth_descriptor{};
		if(_depth)
		{
			depth_descriptor = _depth->get_render_target_view().m_handle;
		}

		get_api_handle()->OMSetRenderTargets(count, color_descriptors.data(), false, _depth != nullptr ? &depth_descriptor : nullptr);
	}

	void graphics_command_buffer::set_viewports(containers::span<viewport> _viewports)
	{
		containers::resizable_array<D3D12_VIEWPORT> dx12_viewports;
		dx12_viewports.reserve(_viewports.size());

		for(viewport& vp : _viewports)
		{
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = vp.m_position.x();
			viewport.TopLeftY = vp.m_position.y();
			viewport.Width = vp.m_size.x();
			viewport.Height = vp.m_size.y();
			viewport.MinDepth = vp.m_depth[0];
			viewport.MaxDepth = vp.m_depth[1];
			dx12_viewports.push_back(viewport);
		}

		get_api_handle()->RSSetViewports(dx12_viewports.size(), dx12_viewports.data());
	}

	void graphics_command_buffer::set_scissors(containers::span<rect> _scissor_rects)
	{
		containers::resizable_array<D3D12_RECT> dx12_rects;
		dx12_rects.reserve(_scissor_rects.size());

		for(rect& rect : _scissor_rects)
		{
			D3D12_RECT scissor_rect;
			scissor_rect.left = rect.m_min.x();
			scissor_rect.top = rect.m_min.y();
			scissor_rect.right = rect.m_max.x();
			scissor_rect.bottom = rect.m_max.y();
			dx12_rects.push_back(scissor_rect);
		}

		get_api_handle()->RSSetScissorRects(dx12_rects.size(), dx12_rects.data());

	}

	void graphics_command_buffer::clear_render_target(render_device_texture_2d& _render_target_resource, math::vec4f _color)
	{
		const float arr[4] = { _color[0], _color[1], _color[2], _color[3] };
		get_api_handle()->ClearRenderTargetView(_render_target_resource.get_render_target_view().m_handle, arr, 0, nullptr);
	}

	void graphics_command_buffer::clear_depth_stencil(render_device_texture_3d& _render_target_resource, float _depth, uint8_t _stencil)
	{
		(void)_render_target_resource, _depth, _stencil;
		VEER_ASSERT(false, "Not implemented");
	}

	void graphics_command_buffer::set_index_buffer(const render_device_buffer& _index_buffer)
	{
		get_api_handle()->IASetIndexBuffer(&_index_buffer.get_index_buffer_view());
	}

	void graphics_command_buffer::set_vertex_buffer(const render_device_buffer& _vertex_buffer)
	{
		get_api_handle()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		get_api_handle()->IASetVertexBuffers(0 /*first index*/, 1 /*vertex buffer count*/, &_vertex_buffer.get_vertex_buffer_view());

	}

	void graphics_command_buffer::draw_instanced(size_t _vertex_count, size_t _instance_count)
	{
		(void)_vertex_count, _instance_count;
		VEER_ASSERT(false, "Not implemented");
	}

	void graphics_command_buffer::draw_indexed_instanced(size_t _index_count, size_t _instance_count)
	{
		get_api_handle()->DrawIndexedInstanced(_index_count, _instance_count, 0, 0, 0);
	}

	void graphics_command_buffer::draw_indirect(const render_device_buffer& _buffer, size_t _arg_index)
	{
		(void)_buffer, _arg_index;
		VEER_ASSERT(false, "Not implemented");
	}
}
