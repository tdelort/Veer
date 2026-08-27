#pragma once

#include <core/unique_ptr.h>
#include <core/containers/span.h>

#include <display/render/command_queue_base.h>
#include <display/render/command_buffer.h>


#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#endif // defined(D3D12_RENDER_BACKEND)

namespace veer::display::render
{
	// different implementation for all backends, still abstract
	class command_queue : public command_queue_base
	{
	public:
		command_queue(render_device& _device, command_buffer::type _type);

		void enqueue(command_buffer&& _command_buffers) override;

		void signal(uint64_t _value) override;
		void wait_for_value(uint64_t _value) override;

		virtual ~command_queue() = 0;

		void flush() override;

	private:
	
#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_command_queue.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_command_queue.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_command_queue.inl"
#endif 
	};

	template<typename T>
	class command_buffer_factory
	{
	public:
		// ptr, do whatever you want with it
		template<typename U>
		unique_ptr<T> get_command_buffer();
	};

		
	class copy_command_queue : public command_queue
	{
	public:
		copy_command_queue(render_device& _device);
		~copy_command_queue();

	protected:
		copy_command_queue(render_device& _device, command_buffer::type _type);
	};

	class compute_command_queue : public copy_command_queue
	{
	public:
		compute_command_queue(render_device& _device);
		~compute_command_queue();

	protected:
		compute_command_queue(render_device& _device, command_buffer::type _type);
	};

	class graphics_command_queue : public compute_command_queue
	{
	public:
		graphics_command_queue(render_device& _device);
		~graphics_command_queue();
	};
}