#pragma once

#include <core/core.h>
#include <core/unique_ptr.h>

#include <display/render/shader_source.h>

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#include <display/render/backends/dx12/dx12_descriptor_heap.h>
// #elif defined(VULKAN_RENDER_BACKEND)
// #elif defined(METAL_RENDER_BACKEND)
#endif 

namespace veer::display::window
{
	class window;
}

namespace veer::display::render
{
	class copy_command_queue;
	class compute_command_queue;
	class graphics_command_queue;
	class swap_chain;
	class graphics_technique;
	class compute_technique;
	class render_device_buffer;
	class buffer_desc;

	class render_device
	{
	public:
		render_device();
		~render_device();

		template<typename T, typename... ARGS> 
		unique_ptr<T> alloc(ARGS&&... _args) { return alloc_internal(std::forward<ARGS&&>(_args)...); }

		copy_command_queue& get_copy_command_queue();
		compute_command_queue& get_compute_command_queue();
		graphics_command_queue& get_graphics_command_queue();

	protected:
		// alloc only viable candidates
		unique_ptr<swap_chain> alloc_internal(veer::display::window::window& _window);
        unique_ptr<graphics_technique> alloc_internal(const shader_stage_source_container_t& _source_code, const shader_signature& _signature, const shader_render_state& _render_state);
        unique_ptr<compute_technique> alloc_internal(const shader_stage_source_container_t& _source_code);

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device.inl"
#endif 
	};
}