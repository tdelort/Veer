#pragma once

#include <core/core.h>

namespace veer::containers
{
	template<typename T>
	class resizable_array
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
		resizable_array();

		resizable_array(const resizable_array& _other);
		resizable_array& operator=(const resizable_array& _other);
		resizable_array(resizable_array&& _other);
		resizable_array& operator=(resizable_array&& _other);

		// getters / setters
		T* data() { return m_data; }
		const T* data() const { return m_data; }

		T* begin() { return m_data; }
		const T* cbegin() const { return m_data; }

		T* end() { return m_data + m_size; }
		const T* cend() const { return m_data + m_size; }

		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		bool empty() const { return size() == 0u; }

		T& operator[](size_t _i) { return m_data[_i]; }
		const T& operator[](size_t _i) const { return m_data[_i]; }

		T& back();
		const T& back() const;

		// change size
		void push_back(const T& _val);
		void push_back(T&& _val);

		void pop_back();

		template<typename... ARGS>
		reference emplace_back(ARGS&&... _args);

		iterator erase(iterator _it);

		void reserve(size_t _new_capacity);
		void resize(size_t _new_size, const_reference _new_values = T());

		void clear();
		void destroy();

	private:
		void grow();
		void alloc( size_t _new_capacity );

	private:
		T* m_data{nullptr};
		size_t m_size{0u};
		size_t m_capacity{0u};
	};
}

#include "resizable_array.hpp"