#pragma once

#include "vec.h"
#include <core/debug.h>

namespace veer::math
{
	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE,ELEM_COUNT>::vec() 
		: m_data{ 0 }
	{}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE,ELEM_COUNT>::vec( TYPE _e ) 
		: m_data{ _e }
	{}

	template<arithmetic TYPE, size_t ELEM_COUNT>
    vec<TYPE,ELEM_COUNT>::~vec() { }


	template<arithmetic TYPE, size_t ELEM_COUNT>
	[[nodiscard]] TYPE vec<TYPE,ELEM_COUNT>::operator[](size_t _index) const
	{
		VEER_ASSERT( _index < ELEM_COUNT, "Index out of range" );
		return m_data[_index];
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	TYPE& vec<TYPE, ELEM_COUNT>::operator[](size_t _index)
	{
		VEER_ASSERT( _index < ELEM_COUNT, "Index out of range" );
		return m_data[_index];
	}


	//----------------------------------------------------------------------------
	//							COUMPOUND OPERATORS
	//----------------------------------------------------------------------------

	// += 
	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(OTHER_TYPE _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] += casted_other;
		return *this;
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator+=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] += static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// -= 
	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(OTHER_TYPE _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] -= casted_other;
		return *this;
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator-=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] -= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// *= 
	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(OTHER_TYPE _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] *= casted_other;
		return *this;
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator*=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] *= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}

	// /= 
	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(OTHER_TYPE _other)
	{
		const TYPE casted_other = static_cast<TYPE>(_other);
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] /= casted_other;
		return *this;
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	template<arithmetic OTHER_TYPE>
	[[nodiscard]] vec<TYPE, ELEM_COUNT>& vec<TYPE, ELEM_COUNT>::operator/=(const vec<OTHER_TYPE, ELEM_COUNT>& _other)
	{
		for ( size_t i = 0; i < ELEM_COUNT; ++i)
			m_data[i] /= static_cast<TYPE>(_other.m_data[i]);
		return *this;
	}


    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _vec)
	{
		return vec<TYPE, ELEM_COUNT>( -_vec.m_data[0], -_vec.m_data[1], -_vec.m_data[2], -_vec.m_data[3] );
	}


    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator+(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) += _rhs;
	}

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator-(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) -= _rhs;
	}

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator/(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) /= _rhs;
	}

    template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> operator*(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		return vec<TYPE, ELEM_COUNT>(_lhs) *= _rhs;
	}



	//----------------------------------------------------------------------------
	//						COMMON GEOMETRIC FUNCTIONS
	//----------------------------------------------------------------------------

	template<arithmetic TYPE, size_t ELEM_COUNT>
	TYPE dot(const vec<TYPE, ELEM_COUNT>& _lhs, const vec<TYPE, ELEM_COUNT>& _rhs)
	{
		TYPE accum = 0.f;
		for (size_t i = 0; i < ELEM_COUNT; ++i)
			accum += _lhs[i] * _rhs[i];
		return accum;
	}

	template<arithmetic TYPE>
	vec<TYPE, 3u> cross(const vec<TYPE, 3u>& _lhs, const vec<TYPE, 3u>& _rhs)
	{
		return vec<TYPE, 3u>(
				_lhs[1] * _rhs[2] - _lhs[2] * _rhs[1],
				_lhs[2] * _rhs[0] - _lhs[0] * _rhs[2],
				_lhs[0] * _rhs[1] - _lhs[1] * _rhs[0]
			);
	}

	// sq_length 

	template<arithmetic TYPE, size_t ELEM_COUNT>
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

	template<arithmetic TYPE, size_t ELEM_COUNT>
	float length(const vec<TYPE, ELEM_COUNT>& _v)
	{
		return std::sqrt(sq_length(_v));
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize(const vec<TYPE, ELEM_COUNT>& _v)
	{
		const float len = length(_v);
		return _v / len;
	}

	template<arithmetic TYPE, size_t ELEM_COUNT>
	vec<TYPE, ELEM_COUNT> normalize_safe(const vec<TYPE, ELEM_COUNT>& _v, float _epsilon)
	{
		const float len = length(_v);
		if ( len < _epsilon )
			return 0.f;
		return _v / len;
	}

}
