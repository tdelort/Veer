#include "render_device_resource.h"

#include "render_device_resource_sync_state.h"

#include <core/debug.h>

namespace veer::display::render
{
	render_device_resource::render_device_resource( render_device& _device )
		: m_device( _device )
	{
		
	}

	render_device_resource::~render_device_resource()
	{
		
	}

	const render_device_resource_sync_state_traking& render_device_resource::get_sync_state_tracking() const
	{
		return m_sync_state_tracking;
	}

	render_device_resource_sync_state_traking& render_device_resource::get_sync_state_tracking()
	{
		return m_sync_state_tracking;
	}

	bool render_device_resource::get_is_upload_dirty_alloc() const
	{
		return flags::get( m_upload_flags, upload_flags::dirty_alloc );
	}

	bool render_device_resource::get_is_upload_dirty_data() const
	{
		return flags::get( m_upload_flags, upload_flags::dirty_data );
	}

	void render_device_resource::set_is_upload_dirty_alloc( bool _val )
	{
		flags::set( m_upload_flags, upload_flags::dirty_alloc );
	}

	void render_device_resource::set_is_upload_dirty_data( bool _val )
	{
		flags::set( m_upload_flags, upload_flags::dirty_data );
	}
}
