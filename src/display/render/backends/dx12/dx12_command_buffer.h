#pragma once

#include "dx12_pch.h"
#include "dx12_render_device.h"
#include "dx12_render_device_resource.h"
#include "dx12_technique.h"

#include <display/render/command_buffer.h>

namespace veer::display::render
{
	static D3D12_COMMAND_LIST_TYPE s_convert(const command_buffer::type _type);

	class dx12_command_buffer_implementation
	{
	public:
		dx12_command_buffer_implementation(command_buffer::type _type);
		~dx12_command_buffer_implementation();

	public: // command_buffer
		void transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state);

	public: // copy_command_buffer
		void copy_texture(const render_device_texture_2d& _dst, const render_device_texture_2d& _src);
		// TODO virtual void copy_texture_region ?

		void copy_buffer(const render_device_buffer& _dst, const render_device_buffer& _src, uint64_t _num_bytes);

	public: // compute_command_buffer
		void set_technique(compute_technique& _technique);

		void clear_texture(const render_device_texture_2d& _texture, math::vec4u _color);
		void clear_texture(const render_device_texture_2d& _texture, math::vec4f _color);
		void clear_texture(const render_device_texture_3d& _texture, math::vec4u _color);
		void clear_texture(const render_device_texture_3d& _texture, math::vec4f _color);

		void clear_buffer(const render_device_buffer& _buffer, uint32_t _value );
		void clear_buffer(const render_device_buffer& _buffer, float _value );

		void dispatch(size_t _x, size_t _y, size_t _z);

	public: // graphics_command_buffer 
		void set_technique(graphics_technique& _technique);

		void set_render_output(render_device_texture_2d* _depth, containers::span<render_device_texture_2d*> _colors);

		void clear_render_target(render_device_texture_2d& _render_target_resource, math::vec4f _color);
		void clear_depth_stencil(render_device_texture_3d& _render_target_resource, float _depth, uint8_t _stencil);

		void set_index_buffer(const render_device_buffer& _index_buffer);
		void set_vertex_buffer(const render_device_buffer& _index_buffer);
	
		void draw_instanced(size_t _vertex_count, size_t _instance_count);
		void draw_indexed_instanced(size_t _index_count, size_t _instance_count );
		void draw_indirect(const render_device_buffer& _buffer, size_t _arg_index);

	public: // misc
		void open( dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator );
		void close();

	private:
		// We might need another handle type on other platforms (XBox DMA CommandLists)
		ComPtr<ID3D12CommandList> m_command_list_handle;
	};

	class dx12_copy_command_buffer : public copy_command_buffer
	{
	public:
		dx12_copy_command_buffer();
		virtual ~dx12_copy_command_buffer() = 0;

	public: // command_buffer
		void transition_barrier(render_device_resource& _resource, render_device_resource_sync_state _from_state, render_device_resource_sync_state _to_state) override;

	public: // copy_command_buffer
		void copy_texture(const render_device_texture_2d& _dst, const render_device_texture_2d& _src) override;
		void copy_buffer(const render_device_buffer& _dst, const render_device_buffer& _src, uint64_t _num_bytes) override;

	private: 
		dx12_command_buffer_implementation m_impl;
	};


	class dx12_compute_command_buffer : public compute_command_buffer
	{
		void set_technique(compute_technique& _technique) override;

		void clear_texture(const render_device_texture_2d& _texture, math::vec4u _color) override;
		void clear_texture(const render_device_texture_2d& _texture, math::vec4f _color) override;
		void clear_texture(const render_device_texture_3d& _texture, math::vec4u _color) override;
		void clear_texture(const render_device_texture_3d& _texture, math::vec4f _color) override;

		void clear_buffer(const render_device_buffer& _buffer, uint32_t _value ) override;
		void clear_buffer(const render_device_buffer& _buffer, float _value ) override;

		void dispatch(size_t _x, size_t _y, size_t _z) override;
	};

	class dx12_graphics_command_buffer : public graphics_command_buffer
	{
		void set_technique(graphics_technique& _technique) override;

		void set_render_output(render_device_texture_2d* _depth, containers::span<render_device_texture_2d*> _colors) override;

		void clear_render_target(render_device_texture_2d& _render_target_resource, math::vec4f _color) override;
		void clear_depth_stencil(render_device_texture_3d& _render_target_resource, float _depth, uint8_t _stencil) override;

		void set_index_buffer(const render_device_buffer& _index_buffer) override;
		void set_vertex_buffer(const render_device_buffer& _index_buffer) override;
	
		void draw_instanced(size_t _vertex_count, size_t _instance_count) override;
		void draw_indexed_instanced(size_t _index_count, size_t _instance_count ) override;
		void draw_indirect(const render_device_buffer& _buffer, size_t _arg_index) override;
	};
}
