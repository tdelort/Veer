#pragma once

namespace veer
{
	struct application_config
	{
		enum class windowing_service_type
		{
			glfw
		};

		enum class rendering_service_type
		{
			dx12,
			vulkan
		};

		windowing_service_type m_windowing_service_choice;
		rendering_service_type m_rendering_service_choice;
	};
}