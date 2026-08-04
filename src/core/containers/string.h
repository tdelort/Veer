#pragma once

#include <core/core.h>
#include <core/concepts.h>
#include <core/basic_allocator.h>

namespace veer::containers
{
	// not a specialization (using private inheritance) of resizable_array since I plan to implement short string optimisation)
    template<typename T, allocator ALLOCATOR = veer::basic_allocator>
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
		base_string();
		~base_string();

		base_string(const base_string& _other);
		base_string& operator=(const base_string& _other);
		base_string(base_string&& _other);
		base_string& operator=(base_string&& _other);

		// getters / setters
		char* str() { return m_data; }
		const char* c_str() const { return m_data; }

		T* begin() { return m_data; }
		const T* cbegin() const { return m_data; }

		T* end() { return m_data + size(); }
		const T* cend() const { return m_data + size(); }

		size_t size() const { return m_size; }
		bool empty() const { return size() == 0u; }
		size_t capacity() const { return m_capacity; }

		T& operator[](size_t _i) { return m_data[_i]; }
		const T& operator[](size_t _i) const { return m_data[_i]; }

		template<typename CHAR_TYPE>
		base_string& operator=(CHAR_TYPE* _other);

		template<typename CHAR_TYPE>
		base_string& operator+=(CHAR_TYPE* _other);

		T& back();
		const T& back() const;

		// change size
		void push_back(const T& _val);
		void push_back(T&& _val);
		void pop_back();

		void insert(iterator _pos, const T& _elem);

		template<typename INPUT_ITERATOR> 
		void insert(iterator _pos, INPUT_ITERATOR _first, INPUT_ITERATOR _last);

		template<typename CHAR_TYPE>
		void append(CHAR_TYPE* _str);

		void clear();
		void destroy();

	private:
		void grow();
		void grow(size_t _min_capacity_needed);
		void alloc(size_t _new_capacity);

	private:
		size_t m_size{0u};
		size_t m_capacity{0u};
		T* m_data{nullptr};

		ALLOCATOR m_allocator;
	};

	using string = base_string<char>;
	using wstring = base_string<wchar_t>;
}


#include "string.hpp"
