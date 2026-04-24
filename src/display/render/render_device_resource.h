#pragma once

#include "render_device.h"
#include "render_device_data_format.h"
#include "render_device_resource_sync_state.h"

#include "command_buffer.h"

#include <core/core.h>

#include <core/math/vec.h>

using backend_handle = void*;
namespace veer::display::render
{
	class render_device_resource
	{
	public:
        enum class upload_flags : uint8_t
        {
			dirty_alloc = 1 << 0,
			dirty_data = 1 << 1,
        };

	public:
		render_device_resource( render_device& _device );
		virtual ~render_device_resource() = 0;
		
	// common functions 
	public:
		const render_device_resource_sync_state_traking& get_sync_state_tracking() const;
		render_device_resource_sync_state_traking& get_sync_state_tracking();

	protected:
		bool get_is_upload_dirty_alloc() const;
		bool get_is_upload_dirty_data() const;
		void set_is_upload_dirty_alloc( bool _val );
		void set_is_upload_dirty_data( bool _val );

	// functions to implement 
	public:
		virtual void upload( command_buffer& _upload_command_buffer ) = 0;

		// avoids using a tag to upcast then downcast into backend version
		virtual backend_handle as_backend() = 0;
		
	protected:
		// reference to the device this resource is bound to
		render_device& m_device;
		render_device_resource_sync_state_traking m_sync_state_tracking{};
		upload_flags m_upload_flags{};
	};

	VEER_ENUM_CLASS_FLAG_OPERATORS( render_device_resource::upload_flags )

	// I don't want to resort to this
	// #define VEER_RENDER_DEVICE_RESOURCE_DERIVED()								\
	// 	virtual backend_handle as_backend() override { return this }; 				\
	// 	virtual const backend_handle as_backend() const override { return this };	\

	// helper to access platform specific derived types from render_device_resources
	// that's a bit barbaric but couldn't find any other less convoluted way
	template<typename T>
	T& static_resource_cast( render_device_resource& _res )
	{
		T* backend_ptr = static_cast<T*>( _res.as_backend() );
		return *backend_ptr;
	}

	template<typename T>
	const T& static_resource_cast( const render_device_resource& _res )
	{
		render_device_resource& non_const_res = const_cast<render_device_resource&>(_res);
		const T* backend_ptr = static_cast<const T*>( non_const_res.as_backend() );
		return *backend_ptr;
	}
	
	/*
	template<size_t DIMENSIONS>
	class render_device_resource
	{
		static constexpr size_t s_dimension = DIMENSIONS;
		using size_type = vec<size_t, s_dimension>;

		void set_size( size_type _val )
		{
			m_size = _val;
		}

		const size_type& get_size() const
		{
			return m_size;
		}
		
	private:
		vec<size_t, s_dimension> m_size;
	};
	*/
}