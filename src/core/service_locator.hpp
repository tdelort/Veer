#pragma once

namespace veer
{
	template<typename T, typename ...Args>
	void service_locator::initialize(Args&&... args)
	{
		static_assert(std::is_base_of<service_interface, T>::value, "T must inherit from service_interface");

		std::unique_ptr<T> new_service{ new T(std::forward<Args>(args)...) };
		const std::type_index key = new_service->get_type_id();

		{
			const std::lock_guard<std::mutex> lock(m_services_lock);
			const auto result = m_services.insert({key, std::move(new_service)});
			VEER_ASSERT(result.second, "You tried to initialize a service twice");
		}
	}

	template<typename T>
	void service_locator::destroy()
	{
		const std::type_index key = std::type_index(typeid(T));

		{
			const std::lock_guard<std::mutex> lock(m_services_lock);
			const size_t nb_erased = m_services.erase(key);
			// TODO Assert nb_erased == 1
			VEER_ASSERT(nb_erased == 1, "Could not find the service to destroy");
		}
	}

	template<typename T>
	T* service_locator::get() const
	{
		const std::type_index key = std::type_index(typeid(T));

		T* found_service = nullptr;
		{
			const std::lock_guard<std::mutex> lock(m_services_lock);
			const auto it = m_services.find(key);
			VEER_ASSERT( it != m_services.end(), "Could not find the service");
			found_service = static_cast<T*>( (it->second).get() );
		}

		return found_service;
	}
}