#ifndef DISPLAY_SWAP_CHAIN_H_INCLUDED
#define DISPLAY_SWAP_CHAIN_H_INCLUDED

#include <Core/Math/vec.h>

namespace veer
{
	class render_device;
	class window;
	class render_device_resource;

	class swap_chain
	{
	public:
		swap_chain();

		virtual ~swap_chain() = 0 {};

		virtual render_device_resource* get_current_backbuffer() = 0;

		virtual size_t get_backbuffer_index() = 0;

		virtual void present(size_t _sync_intervals) = 0;

		static constexpr size_t s_swap_chain_buffer_count = 2u;
	protected:
		bool m_tearing_allowed;
	};
}

#endif // DISPLAY_SWAP_CHAIN_H_INCLUDED
