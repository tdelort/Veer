#include "rendering_service.h"

namespace veer
{	
	void rendering_service::start_frame(uint64_t _frame_index)
	{
		m_current_frame_index = _frame_index;
	}

	void rendering_service::end_frame()
	{
	}

	render_device& rendering_service::get_render_device() const
	{
		// TODO: checks for thread safety here
		VEER_ASSERT(m_device != nullptr, "Render device wasn't allocated" );
		return *m_device;
	}
}
