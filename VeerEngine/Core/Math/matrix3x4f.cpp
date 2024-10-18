#include "matrix3x4f.h"

#include "../debug.h"

namespace veer
{
	matrix3x4f::matrix3x4f()
		: matrix3x4f( 0.0f )
	{
	}

	/*
	*  Diagonal :
	*  v 0 0 0 (0*4 + 0 = 0)
	*  0 v 0 0 (1*4 + 1 = 5)
	*  0 0 v 0 (2*4 + 2 = 10)
	*/
	matrix3x4f::matrix3x4f( float _value )
	{
		m_data[0] = _value;
		m_data[5] = _value;
		m_data[10] = _value;
	}

	matrix3x4f::matrix3x4f( const float( &_list )[12] )
	{
		for( int i = 0; i < 12; ++i )
		{
			m_data[i] = _list[i];
		}
	}

	matrix3x4f::matrix3x4f( const float( &_list )[3][4] )
	{
		for( int i = 0; i < 3; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				m_data[i * 4 + j] = _list[i][j];
			}
		}
	}

	float matrix3x4f::at( const size_t _row, const size_t _column ) const
	{
		VEER_ASSERT( _row < 3 && _column < 4, "Index out of range" );
		return m_data[_row * 4 + _column];
	}

	float& matrix3x4f::at( const size_t _row, const size_t _column )
	{
		VEER_ASSERT( _row < 3 && _column < 4, "Index out of range" );
		return m_data[_row * 4 + _column];
	}

	std::ostream& operator<<( std::ostream& _os, const matrix3x4f& _matrix )
	{
		_os << "[";
		_os << "(" << _matrix.m_data[0] << ", " << _matrix.m_data[1] << ", " << _matrix.m_data[2] << ", " << _matrix.m_data[3] << ")";
		_os << "(" << _matrix.m_data[4] << ", " << _matrix.m_data[5] << ", " << _matrix.m_data[6] << ", " << _matrix.m_data[7] << ")";
		_os << "(" << _matrix.m_data[8] << ", " << _matrix.m_data[9] << ", " << _matrix.m_data[10] << ", " << _matrix.m_data[11] << ")";
		_os << "]";
		return _os;
	}
} // namespace veer
