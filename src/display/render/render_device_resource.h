#pragma once

#include "render_device_resource_sync_state.h"

#include <core/core.h>
#include <core/math/vec.h>
#include <core/containers/resizable_array.h>

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

	// TODO : move when doing resource table / heap abstraction
	enum class render_device_resource_heap_type
	{
		srv,
		uav,
		cbv
	};

	class render_device_resource
	{
	public:
        enum class upload_flags : uint8_t
        {
			none = 0,
			dirty_alloc = 1 << 0,
			dirty_data = 1 << 1,

			dirty_all = dirty_alloc | dirty_data 
        };

	public:
		render_device_resource(render_device& _device);
		virtual ~render_device_resource() = 0;
		
	// common functions 
	public:
		inline const render_device_resource_sync_state_traking& get_sync_state_tracking() const;

		inline render_device_resource_sync_state_traking& get_sync_state_tracking();

		inline upload_flags get_upload_flags() const;

		virtual void upload(copy_command_buffer& _upload_command_buffer, upload_flags _upload_flags) = 0;

        template<typename T>
        void set_data(containers::span<T> _data);

        template<typename T>
        containers::span<const T> get_data() const;

		using bindless_id = uint64_t;
		static constexpr bindless_id s_invalid_bindless_id = static_cast<uint64_t>(-1);

		virtual bindless_id get_bindless_id(render_device_resource_heap_type _heap_type) const = 0;

	protected:
		inline render_device& get_render_device() const;

	protected:
		// reference to the device this resource is bound to
		render_device& m_device;
		render_device_resource_sync_state_traking m_sync_state_tracking{};
		upload_flags m_upload_flags{};

        containers::resizable_array<byte_t> m_data;

#if defined(D3D12_RENDER_BACKEND)
#include "backends/dx12/dx12_render_device_resource.inl"
// #elif defined(VULKAN_RENDER_BACKEND)
// #include "backends/vulkan/vk_render_device_resource.inl"
// #elif defined(METAL_RENDER_BACKEND)
// #include "backends/metal/mtl_render_device_resource.inl"
#endif 
	};

	VEER_ENUM_CLASS_FLAG_OPERATORS(render_device_resource::upload_flags)


	const render_device_resource_sync_state_traking& render_device_resource::get_sync_state_tracking() const
	{
		return m_sync_state_tracking;
	}

	render_device_resource_sync_state_traking& render_device_resource::get_sync_state_tracking()
	{
		return m_sync_state_tracking;
	}

	render_device_resource::upload_flags render_device_resource::get_upload_flags() const
	{
		return m_upload_flags;
	}

	render_device& render_device_resource::get_render_device() const
	{
		return m_device;
	}


    template<typename T>
    void render_device_resource::set_data(containers::span<T> _data)
    {
        m_data.clear();
        const size_t new_size = _data.size() * sizeof(T) / sizeof(byte_t); 
        m_data.resize(new_size);
        std::memcpy(m_data.data(), _data.data(), new_size);
        
        flags::set(m_upload_flags, upload_flags::dirty_data);
    }

    template<typename T>
    containers::span<const T> render_device_resource::get_data() const
    {
        return containers::span<const T>(static_cast<const T*>(m_data.data()), ( m_data.size() / sizeof(T) ) * sizeof(byte_t) );
    }
}