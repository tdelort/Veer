#pragma once

#include "core/containers/static_array.h"
// TODO use only texture_base whenever possible
#include "display/render/render_device_texture_2d.h"
#include "display/render/render_device_texture_3d.h"
#include "display/render/render_device_buffer.h"
#include "render_device_resource_sync_state.h"

#include <core/math/vec.h>
#include <display/render/technique.h>
#include <display/render/base_types.h>

#if defined(D3D12_RENDER_BACKEND)
#include "display/render/backends/dx12/dx12_pch.h"

namespace veer::display::render
{
	class dx12_render_device; 
};
#endif // defined(D3D12_RENDER_BACKEND)

namespace veer::display::render
{
	class render_device_resource;

	class command_buffer
	{
	public:
		using after_execution_callback_t = std::function<void()>;

	public:
		enum class type
		{
			graphics,
			compute,
			copy,
		};

		command_buffer(command_buffer::type _type);
		virtual ~command_buffer();

		// TODO : this is the low level api, I should implement something to handle state tracking and transitions when multi threaded rendering in an higher level class 
		void transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state);

	public:
		void do_after_execution(after_execution_callback_t&& _callback)
		{
			m_after_execution_callbacks.push_back(_callback);
		}

		void on_execution()
		{
			for( after_execution_callback_t& callback : m_after_execution_callbacks )
			{
				callback();
			}

			m_after_execution_callbacks.clear();
		}

	protected:
		type m_type;

		veer::containers::resizable_array<after_execution_callback_t> m_after_execution_callbacks;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_command_buffer.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_command_buffer.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_command_buffer.inl"
#endif 
	};

	class copy_command_buffer : public command_buffer
	{
	public:
		copy_command_buffer();
		virtual ~copy_command_buffer();

	protected:
		copy_command_buffer(command_buffer::type _type);

	public:
		void copy_texture(const render_device_texture_2d& _dst, const render_device_texture_2d& _src);
		// TODO void copy_texture_region ?

		void copy_buffer(const render_device_buffer& _dst, const render_device_buffer& _src, uint64_t _num_bytes);
		// TODO void copy_buffer_region(const render_device_buffer& _dst, uint64_t _dst_offset, const render_device_buffer& _src, uint64_t _src_offset, uint64_t _num_bytes);
	};

	class compute_command_buffer : public copy_command_buffer
	{
	public:
		compute_command_buffer();
		~compute_command_buffer();

	protected:
		compute_command_buffer(command_buffer::type _type);

	public:
		void set_technique(compute_technique& _technique);

		void clear_texture(const render_device_texture_2d& _texture, math::vec4u _color);
		void clear_texture(const render_device_texture_2d& _texture, math::vec4f _color);
		void clear_texture(const render_device_texture_3d& _texture, math::vec4u _color);
		void clear_texture(const render_device_texture_3d& _texture, math::vec4f _color);

		void clear_buffer(const render_device_buffer& _buffer, uint32_t _value );
		void clear_buffer(const render_device_buffer& _buffer, float _value );

		// TODO : maybe add "set constant buffer"

		void dispatch(size_t _x, size_t _y, size_t _z);
		// TODO : void dispatch_indirect(const render_device_buffer& _buffer, size_t _arg_index);
	};

	class graphics_command_buffer : public compute_command_buffer
	{
	public:
		graphics_command_buffer();
		~graphics_command_buffer();

	public:
		void set_technique(graphics_technique& _technique);

		void set_render_output(render_device_texture_2d* _depth, render_device_texture_2d* _color)
		{
			set_render_output(_depth, containers::span<render_device_texture_2d*>(_color));
		}

		void set_render_output(render_device_texture_2d* _depth, containers::span<render_device_texture_2d*> _colors);

		void set_viewports(containers::span<viewport> _viewports);
		void set_scissors(containers::span<rect> _scissor_rects);

		void clear_render_target(render_device_texture_2d& _render_target_resource, math::vec4f _color);
		void clear_depth_stencil(render_device_texture_3d& _render_target_resource, float _depth, uint8_t _stencil);
		// TODO : check if need more args for clear depth stencil

		// TODO : we might need more complex types for view on buffers to have more control
		// Like for example one buffer with multiple view on it because it contains/interleaves index and vertex data
		void set_index_buffer(const render_device_buffer& _index_buffer);
		void set_vertex_buffer(const render_device_buffer& _vertex_buffer);
	
		void draw_instanced(size_t _vertex_count, size_t _instance_count);
		void draw_indexed_instanced(size_t _index_count, size_t _instance_count );
		void draw_indirect(const render_device_buffer& _buffer, size_t _arg_index);
	};

}