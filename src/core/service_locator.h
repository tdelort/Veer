#pragma once

#include <core/core.h>
#include "service.h"

namespace veer
{
	// Mostly based on : https://johnfarrier.com/the-service-locator-pattern-a-robust-c-implementation/
	class service_locator
	{
	public:
		template<typename T, typename ...Args>
		void initialize(Args&&... args);

		template<typename T>
		void destroy();

		template<typename T>
		T* get() const;

	private:
		std::unordered_map< std::type_index, std::unique_ptr<service_interface> > m_services;
		mutable std::mutex m_services_lock;
	};
}

#include "service_locator.hpp"