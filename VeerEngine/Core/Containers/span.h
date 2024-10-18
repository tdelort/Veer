#ifndef CORE_CONTAINERS_SPAN_H_INCLUDED
#define CORE_CONTAINERS_SPAN_H_INCLUDED

namespace veer
{
	template<typename T>
	class span
	{
	public:
		span();
		span(T* _data);
		span(T* _data, size_t _count);

		T* data() { return m_data; }
		const T* data() const { return m_data; }

		T* begin() { return m_data; }
		const T* cbegin() const { return m_data; }

		T* end() { return m_data + m_count; }
		const T* cend() const { return m_data + m_count; }

		size_t size() const { return m_count; }

		T& operator[](size_t _i) { return m_data[_i]; }
		const T& operator[](size_t _i) const { return m_data[_i]; }

	private:
		T* m_data;
		size_t m_count;
	};
}

#include "span.hpp"

#endif // CORE_CONTAINERS_SPAN_H_INCLUDED
