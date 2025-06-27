#ifndef CORE_CONTAINERS_STATIC_ARRAY_H_INCLUDED
#define CORE_CONTAINERS_STATIC_ARRAY_H_INCLUDED

namespace veer
{
	template<typename T, size_t N >
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
		// this ctor does not support non default constructible T :[ (i think I can live with this for now)
		// But I can thus use default T() param without worrying too much :]
		constexpr static_array( const_reference _val = T() );

		constexpr static_array( std::initializer_list<T> _list );

		static_array(const static_array& _other);
		static_array& operator=(const static_array& _other);
		static_array(static_array&& _other);
		static_array& operator=(static_array&& _other);

		pointer data() { return m_data; }
		const_pointer data() const { return m_data; }

		iterator begin() { return m_data; }
		const_iterator cbegin() const { return m_data; }

		iterator end() { return m_data + m_size; }
		const_iterator cend() const { return m_data + m_size; }

		constexpr size_t size() const { return m_size; }

		reference operator[](size_t _i) { return m_data[_i]; }
		const_reference operator[](size_t _i) const { return m_data[_i]; }

	private:
		T m_data[N];
		static constexpr size_t m_size = N;
	};
}

#include "static_array.hpp"

#endif // CORE_CONTAINERS_STATIC_ARRAY_H_INCLUDED
