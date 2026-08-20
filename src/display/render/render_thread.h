#pragma once

#include <core/core.h>

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/swap_chain.h>
#include <display/render/backends/dx12/dx12_pch.h>
// #elif defined(VULKAN_RENDER_BACKEND)
// #elif defined(METAL_RENDER_BACKEND)
#endif 

namespace veer::display::render
{
	class command_buffer;
	class render_device;

	class render_thread
	{
	public:
		render_thread(const render_device& _device);
		~render_thread();
		
		void start_frame(uint64_t _frame_index);
		void end_frame();

		void open_command_buffer(command_buffer& command_buffer);
		void close_command_buffer(command_buffer& command_buffer);

	private:
		size_t m_current_frame_index;
		const render_device& m_device;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_thread.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_thread.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_thread.inl"
#endif 
	};
}