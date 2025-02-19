#ifndef DISPLAY_RENDER_DEVICE_RESOURCE_H_INCLUDED
#define DISPLAY_RENDER_DEVICE_RESOURCE_H_INCLUDED

#include "resource_synchronisation.h"

namespace veer
{
	class render_device_resource
	{
	public:
		render_device_resource( resource_sync_state _default_state, size_t _subresource_count );
		virtual ~render_device_resource() = 0 {};

		size_t get_subresource_count() const;

		virtual void set_resource_state( resource_sync_state _new_state, size_t _subresource_index );
		virtual resource_sync_state get_resource_state( size_t _subresource_index ) const;

	private:
		// V0 mono threaded API :
		// We can simply issue transition when we want since we don't have to worry about another thread creating command lists
		// Thus state tracking can be solely on the resource

		// one sync state per sub resource
		size_t m_subresource_count;
		std::vector<resource_sync_state> m_sync_states;
	};
}

#endif // DISPLAY_RENDER_DEVICE_RESOURCE_H_INCLUDED
