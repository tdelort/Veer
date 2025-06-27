#ifndef CORE_APPLICATION_H_INCLUDED
#define CORE_APPLICATION_H_INCLUDED


#include "application_config.h"

#include <core/core.h>

#include <display/render/rendering_service.h>
#include <display/window/windowing_service.h>

namespace veer
{
	class VEER_DLLEXPORT application
	{
	public:
		application( const application_config& _config );
		~application();
		int run();

		const application_config& get_config() const;

	private:
		std::unique_ptr<rendering_service> m_render_service;
		std::unique_ptr<windowing_service> m_window_service;
		application_config m_application_config;
	};
}

#endif // !CORE_APPLICATION_H_INCLUDED
