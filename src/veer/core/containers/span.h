#pragma once

namespace veer::containers
{
	template<typename T>
	class span
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = T*;
		using const_iterator = const T*;

	public:
		constexpr span();
		constexpr span(T& _data);
		constexpr span(T* _data, size_t _count);

		template<typename COLLECTION_TYPE>
		constexpr span(COLLECTION_TYPE& _collection);

		T* data() { return m_data; }
		const T* data() const { return m_data; }

		T* begin() const { return m_data; }
		const T* cbegin() const { return m_data; }

		T* end() const { return m_data + m_size; }
		const T* cend() const { return m_data + m_size; }

		size_t size() const { return m_size; }
		bool empty() const { return m_size == 0u; }

		T& operator[](size_t _i) { return m_data[_i]; }
		const T& operator[](size_t _i) const { return m_data[_i]; }

	private:
		T* m_data;
		size_t m_size;
	};
}

#include "span.hpp"