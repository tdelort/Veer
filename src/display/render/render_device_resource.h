#pragma once

#include "render_device_resource_sync_state.h"

#include <core/core.h>
#include <core/math/vec.h>

#if defined(D3D12_RENDER_BACKEND)
#include <display/render/backends/dx12/dx12_pch.h>
#include <display/render/resource_desc.h>
#include <core/containers/span.h>
#endif 

using backend_handle = void*;
namespace veer::display::render
{
	class render_device;
	class copy_command_buffer;

	class render_device_resource
	{
	public:
        enum class upload_flags : uint8_t
        {
			dirty_alloc = 1 << 0,
			dirty_data = 1 << 1,

			dirty_all = dirty_alloc | dirty_data 
        };

	public:
		render_device_resource(render_device& _device);
		virtual ~render_device_resource() = 0;
		
	// common functions 
	public:
		const render_device_resource_sync_state_traking& get_sync_state_tracking() const
		{
			return m_sync_state_tracking;
		}

		render_device_resource_sync_state_traking& get_sync_state_tracking()
		{
			return m_sync_state_tracking;
		}

		upload_flags get_upload_flags() const
		{
			return m_upload_flags;
		}
	protected:
		render_device& get_render_device() const
		{
			return m_device;
		}

	// functions to implement 
	public:
		virtual void upload(copy_command_buffer& _upload_command_buffer) = 0;

	protected:
		// reference to the device this resource is bound to
		render_device& m_device;
		render_device_resource_sync_state_traking m_sync_state_tracking{};
		upload_flags m_upload_flags{};

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_resource.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_resource.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_resource.inl"
#endif 
	};

	VEER_ENUM_CLASS_FLAG_OPERATORS(render_device_resource::upload_flags)
}