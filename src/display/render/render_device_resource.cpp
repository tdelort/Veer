#include "render_device_resource.h"

#include <core/debug.h>

namespace veer
{

	render_device_resource::render_device_resource( resource_sync_state _default_state, size_t _subresource_count )
	{
		// TODO : implement resource creation
		m_subresource_count = _subresource_count;
		m_sync_states.resize( m_subresource_count, _default_state);
	}

	size_t render_device_resource::get_subresource_count() const
	{
		// TODO implement sub resources
		return m_subresource_count;
	}

	void render_device_resource::set_resource_state( resource_sync_state _new_state, size_t _subresource_index )
	{
		VEER_ASSERT(_subresource_index < get_subresource_count(), "subresource index bigger than subresource count");
		m_sync_states[_subresource_index] = _new_state;
	}

	resource_sync_state render_device_resource::get_resource_state( size_t _subresource_index ) const
	{
		VEER_ASSERT(_subresource_index < get_subresource_count(), "subresource index bigger than subresource count");
		return m_sync_states[_subresource_index];
	}
}
