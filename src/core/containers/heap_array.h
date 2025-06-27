#ifndef CORE_CONTAINERS_HEAP_ARRAY_H_INCLUDED
#define CORE_CONTAINERS_HEAP_ARRAY_H_INCLUDED

namespace veer
{
	template<typename T>
	class heap_array
	{
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = T*;
		using const_iterator = const T*;
	public:
		heap_array();
		heap_array( const T& _val, size_t _size );
		heap_array( std::initializer_list<T> _list );

		heap_array(const heap_array& _other);
		heap_array& operator=(const heap_array& _other);
		heap_array(heap_array&& _other);
		heap_array& operator=(heap_array&& _other);

		T* data() { return m_data; }
		const T* data() const { return m_data; }

		T* begin() { return m_data; }
		const T* cbegin() const { return m_data; }

		T* end() { return m_data + m_size; }
		const T* cend() const { return m_data + m_size; }

		size_t size() const { return m_size; }

		T& operator[](size_t _i) { return m_data[_i]; }
		const T& operator[](size_t _i) const { return m_data[_i]; }

	private:
		T* m_data;
		size_t m_size = N;
	};
}

#include "span.hpp"

#endif // CORE_CONTAINERS_HEAP_ARRAY_H_INCLUDED
