#pragma once

#include "resizable_array.h"
#include <core/core.h>

namespace veer::containers
{
	// not a specialization (using private inheritance) of resizable_array since I plan to implement short string optimisation)
	template<typename T>
	class base_string
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

		// getters / setters
		char* str() { m_long_data.data(); }

		T* begin() { return m_long_data.begin(); }
		const T* cbegin() const { return m_long_data.begin(); }

		T* end() { return m_long_data.end(); }
		const T* cend() const { return m_long_data.end(); }

		size_t size() const { return m_long_data.size(); }
		bool empty() const { return m_long_data.empty(); }

		T& operator[](size_t _i) { return m_long_data[_i]; }
		const T& operator[](size_t _i) const { return m_long_data[_i]; }

		T& back() { return m_long_data.back(); }
		const T& back() const { return m_long_data.back(); }

		// change size
		void push_back(const T& _val) { m_long_data.push_back( _val ); }
		void push_back(T&& _val) { m_long_data.push_back( std::forward<T&&>( _val ) ); }
		void pop_back() { m_long_data.push_back(); }

		void clear() { m_long_data.clear(); }
		void destroy() { m_long_data.destroy(); }

	private:
		// TODO : implement short string optimization (for now, it's just a dumb passthrough)
		resizable_array<T> m_long_data;
	};

	using string = base_string<char>;
}


#include "string.hpp"
