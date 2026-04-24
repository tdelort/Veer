#pragma once

#include <core/core.h>
#include <core/containers/span.h>

namespace veer::containers
{
	template<typename T, size_t N>
	class static_array
	{
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = T*;
		using const_iterator = const T*;
	public:
		pointer data() { return m_data; }
		const_pointer data() const { return m_data; }

		iterator begin() { return m_data; }
		const_iterator cbegin() const { return m_data; }

		iterator end() { return m_data + m_size; }
		const_iterator cend() const { return m_data + m_size; }

		constexpr size_t size() const { return m_size; }

		reference operator[](size_t _i) { return m_data[_i]; }
		const_reference operator[](size_t _i) const { return m_data[_i]; }

		// public so that it can be init using default ctors
		T m_data[N];
	private:
		static constexpr size_t m_size = N;
	};
}

#include "static_array.hpp"