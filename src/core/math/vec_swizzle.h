#ifndef CORE_MATH_VEC_SWIZZLE_H_INCLUDED
#define CORE_MATH_VEC_SWIZZLE_H_INCLUDED

#define VEER_VEC_DEFINE_GETTER( _name, _i0 )						\
	[[nodiscard]] TYPE _name() const { return m_data[_i0]; }    \
	TYPE& _name() { return m_data[_i0]; }                    

// TODO : implement swizzle getters that return a ref to modifiy values

#define VEER_VEC_DEFINE_GETTER_2( _name, _i0, _i1 )													\
	[[nodiscard]] vec<TYPE, 2u> _name() const { return vec<TYPE, 2u>( m_data[_i0], m_data[_i1] ); }	\

#define VEER_VEC_DEFINE_GETTER_3( _name, _i0, _i1, _i2 )																\
	[[nodiscard]] vec<TYPE, 3u> _name() const { return vec<TYPE, 3u>( m_data[_i0], m_data[_i1], m_data[_i2] ); }	\

#define VEER_VEC_DEFINE_GETTER_4( _name, _i0, _i1, _i2, _i3 )																	\
	[[nodiscard]] vec<TYPE, 4u> _name() const { return vec<TYPE, 4u>( m_data[_i0], m_data[_i1], m_data[_i2], m_data[_i3] ); }	\


#define VEER_VEC_SWIZZLE_1					\
VEER_VEC_DEFINE_GETTER( x, 0 )				\
VEER_VEC_DEFINE_GETTER_2( xx, 0, 0 )			\
VEER_VEC_DEFINE_GETTER_3( xxx, 0, 0, 0 )		\
VEER_VEC_DEFINE_GETTER_4( xxxx, 0, 0, 0, 0 )	\

#define VEER_VEC_SWIZZLE_2					\
VEER_VEC_SWIZZLE_1							\
VEER_VEC_DEFINE_GETTER( y, 1 )				\
VEER_VEC_DEFINE_GETTER_2( xy, 0, 1 )			\
VEER_VEC_DEFINE_GETTER_2( yx, 1, 0 )			\
VEER_VEC_DEFINE_GETTER_2( yy, 1, 1 )			\
VEER_VEC_DEFINE_GETTER_3( xxy, 0, 0, 1 )		\
VEER_VEC_DEFINE_GETTER_3( xyx, 0, 1, 0 )		\
VEER_VEC_DEFINE_GETTER_3( xyy, 0, 1, 1 )		\
VEER_VEC_DEFINE_GETTER_3( yxx, 1, 0, 0 )		\
VEER_VEC_DEFINE_GETTER_3( yxy, 1, 0, 1 )		\
VEER_VEC_DEFINE_GETTER_3( yyx, 1, 1, 0 )		\
VEER_VEC_DEFINE_GETTER_3( yyy, 1, 1, 1 )		\
VEER_VEC_DEFINE_GETTER_4( xxxy, 0, 0, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_4( xxyx, 0, 0, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_4( xxyy, 0, 0, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_4( xyxx, 0, 1, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_4( xyxy, 0, 1, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_4( xyyx, 0, 1, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_4( xyyy, 0, 1, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_4( yxxx, 1, 0, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_4( yxxy, 1, 0, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_4( yxyx, 1, 0, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_4( yxyy, 1, 0, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_4( yyxx, 1, 1, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_4( yyxy, 1, 1, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_4( yyyx, 1, 1, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_4( yyyy, 1, 1, 1, 1 )	\

#define VEER_VEC_SWIZZLE_3					\
VEER_VEC_SWIZZLE_2							\
VEER_VEC_DEFINE_GETTER( z, 2 )				\
VEER_VEC_DEFINE_GETTER_2( xz, 0, 2 )			\
VEER_VEC_DEFINE_GETTER_2( yz, 1, 2 )			\
VEER_VEC_DEFINE_GETTER_2( zx, 2, 0 )			\
VEER_VEC_DEFINE_GETTER_2( zy, 2, 1 )			\
VEER_VEC_DEFINE_GETTER_2( zz, 2, 2 )			\
VEER_VEC_DEFINE_GETTER_3( xxz, 0, 0, 2 )		\
VEER_VEC_DEFINE_GETTER_3( xyz, 0, 1, 2 )		\
VEER_VEC_DEFINE_GETTER_3( xzx, 0, 2, 0 )		\
VEER_VEC_DEFINE_GETTER_3( xzy, 0, 2, 1 )		\
VEER_VEC_DEFINE_GETTER_3( xzz, 0, 2, 2 )		\
VEER_VEC_DEFINE_GETTER_3( yxz, 1, 0, 2 )		\
VEER_VEC_DEFINE_GETTER_3( yyz, 1, 1, 2 )		\
VEER_VEC_DEFINE_GETTER_3( yzx, 1, 2, 0 )		\
VEER_VEC_DEFINE_GETTER_3( yzy, 1, 2, 1 )		\
VEER_VEC_DEFINE_GETTER_3( yzz, 1, 2, 2 )		\
VEER_VEC_DEFINE_GETTER_3( zxx, 2, 0, 0 )		\
VEER_VEC_DEFINE_GETTER_3( zxy, 2, 0, 1 )		\
VEER_VEC_DEFINE_GETTER_3( zxz, 2, 0, 2 )		\
VEER_VEC_DEFINE_GETTER_3( zyx, 2, 1, 0 )		\
VEER_VEC_DEFINE_GETTER_3( zyy, 2, 1, 1 )		\
VEER_VEC_DEFINE_GETTER_3( zyz, 2, 1, 2 )		\
VEER_VEC_DEFINE_GETTER_3( zzx, 2, 2, 0 )		\
VEER_VEC_DEFINE_GETTER_3( zzy, 2, 2, 1 )		\
VEER_VEC_DEFINE_GETTER_3( zzz, 2, 2, 2 )		\
VEER_VEC_DEFINE_GETTER_3( xxxz, 0, 0, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xxyz, 0, 0, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xxzx, 0, 0, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( xxzy, 0, 0, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( xxzz, 0, 0, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xyxz, 0, 1, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xyyz, 0, 1, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xyzx, 0, 1, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( xyzy, 0, 1, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( xyzz, 0, 1, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xzxx, 0, 2, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_3( xzxy, 0, 2, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_3( xzxz, 0, 2, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xzyx, 0, 2, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_3( xzyy, 0, 2, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_3( xzyz, 0, 2, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( xzzx, 0, 2, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( xzzy, 0, 2, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( xzzz, 0, 2, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yxxz, 1, 0, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yxyz, 1, 0, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yxzx, 1, 0, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( yxzy, 1, 0, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( yxzz, 1, 0, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yyxz, 1, 1, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yyyz, 1, 1, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yyzx, 1, 1, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( yyzy, 1, 1, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( yyzz, 1, 1, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yzxx, 1, 2, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_3( yzxy, 1, 2, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_3( yzxz, 1, 2, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yzyx, 1, 2, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_3( yzyy, 1, 2, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_3( yzyz, 1, 2, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( yzzx, 1, 2, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( yzzy, 1, 2, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( yzzz, 1, 2, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zxxx, 2, 0, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zxxy, 2, 0, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zxxz, 2, 0, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zxyx, 2, 0, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zxyy, 2, 0, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zxyz, 2, 0, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zxzx, 2, 0, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zxzy, 2, 0, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zxzz, 2, 0, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zyxx, 2, 1, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zyxy, 2, 1, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zyxz, 2, 1, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zyyx, 2, 1, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zyyy, 2, 1, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zyyz, 2, 1, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zyzx, 2, 1, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zyzy, 2, 1, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zyzz, 2, 1, 2, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zzxx, 2, 2, 0, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zzxy, 2, 2, 0, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zzxz, 2, 2, 0, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zzyx, 2, 2, 1, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zzyy, 2, 2, 1, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zzyz, 2, 2, 1, 2 )	\
VEER_VEC_DEFINE_GETTER_3( zzzx, 2, 2, 2, 0 )	\
VEER_VEC_DEFINE_GETTER_3( zzzy, 2, 2, 2, 1 )	\
VEER_VEC_DEFINE_GETTER_3( zzzz, 2, 2, 2, 2 )	\

#endif // CORE_MATH_VEC_SWIZZLE_H_INCLUDED
