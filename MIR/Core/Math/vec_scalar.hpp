#ifndef CORE_MATH_VEC_SCALAR_HPP_INCLUDED
#define CORE_MATH_VEC_SCALAR_HPP_INCLUDED


#include "vec.h"

namespace mir
{
	template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE,ELEM_COUNT>::vec() 
		: m_data{ 0 }
	{}

	template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE,ELEM_COUNT>::vec( TYPE _e ) 
		: m_data{ _e }
	{}

	template<typename TYPE, size_t ELEM_COUNT>
    vec<TYPE,ELEM_COUNT>::~vec() { }


	template<typename TYPE, size_t ELEM_COUNT>
	[[nodiscard]] TYPE vec<TYPE,ELEM_COUNT>::operator[](size_t _index) const
	{
		MIR_ASSERT( _index < ELEM_COUNT, "Index out of range" );
		return m_data[_index];
	}

	template<typename TYPE, size_t ELEM_COUNT>
	TYPE& vec<TYPE, ELEM_COUNT>::operator[](size_t _index)
	{
		MIR_ASSERT( _index < ELEM_COUNT, "Index out of range" );
		return m_data[_index];
	}


	//----------------------------------------------------------------------------
	//							COUMPOUND OPERATORS
	//----------------------------------------------------------------------------

	// += 
	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(E0 _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] += casted_other;
		return *this;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(const vec<E0, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] += static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// -= 
	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(E0 _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] -= casted_other;
		return *this;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(const vec<E0, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] -= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// *= 
	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(E0 _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] *= casted_other;
		return *this;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(const vec<E0, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] *= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// /= 
	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(E0 _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] /= casted_other;
		return *this;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	template<typename E0>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(const vec<E0, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] /= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}


    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _vec)
	{
		return vec<TYPE, ELEM_COUNT>( -_vec.m_data[0], -_vec.m_data[1], -_vec.m_data[2], -_vec.m_data[3] );
	}


    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator+(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) += _rhs;
	}

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) -= _rhs;
	}

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) /= _rhs;
	}

    template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) *= _rhs;
	}



	//----------------------------------------------------------------------------
	//						COMMON GEOMETRIC FUNCTIONS
	//----------------------------------------------------------------------------

	template<typename TYPE, size_t ELEM_COUNT>
	TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		TYPE accum = 0.f;
		for (size_t i = 0; i < ELEM_COUNT; ++i)
			accum += _lhs[i] * _rhs[i];
		return accum;
	}

	template<typename TYPE>
	vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs)
	{
		return vec<TYPE, 3u>(
				_lhs[1] * _rhs[2] - _lhs[2] * _rhs[1],
				_lhs[2] * _rhs[0] - _lhs[0] * _rhs[2],
				_lhs[0] * _rhs[1] - _lhs[1] * _rhs[0]
			);
	}

	// sq_length 

	template<typename TYPE, size_t ELEM_COUNT>
	float sq_length(const vec<TYPE, ELEM_COUNT>& _v)
	{
		float size_accum = 0.f;
		for (size_t i = 0; i < ELEM_COUNT; ++i)
		{
			const float val = static_cast<float>(_v[i]);
			size_accum += val * val;
		}
		return size_accum;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	float length(const vec<TYPE, ELEM_COUNT>& _v)
	{
		return std::sqrt(sq_length(_v));
	}

	template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v)
	{
		const float len = length(_v);
		return _v / len;
	}

	template<typename TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize_safe(const vec<TYPE, ELEM_COUNT>& _v, float _epsilon)
	{
		const float len = length(_v);
		if ( len < _epsilon )
			return 0.f;
		return _v / len;
	}

}

#endif // CORE_MATH_VEC_SCALAR_HPP_INCLUDED
