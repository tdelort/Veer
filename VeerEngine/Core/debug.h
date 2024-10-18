#ifndef CORE_DEBUG_H_INCLUDED
#define CORE_DEBUG_H_INCLUDED

#include "core.h"

namespace veer
{
	class debug
	{
		class assert_error : public std::runtime_error
		{
		public:
			assert_error(std::string const & _message);
		};

	public:
		enum class log_level : uint8_t
		{
			debug,
			log,
			warn,
			error
		};
		static void assert_if( bool _condition, std::string_view const _message , const char* _file, const char* _function, int _line );
		static void print( log_level _level, std::string_view const _message , const char* _file, const char* _function, int _line );
	};
} // namespace veer


#define VEER_BREAKPOINT __debugbreak()
#define VEER_PRETTY_FUNCTION __FUNCTION__


#define VEER_ASSERT_INTERNAL( _test, _msg )		\
{												\
	std::stringstream VEER_ASSERT_INTERNAL__ss;	\
	VEER_ASSERT_INTERNAL__ss << _msg;				\
	debug::assert_if( ( _test ), VEER_ASSERT_INTERNAL__ss.str(), __FILE__, VEER_PRETTY_FUNCTION, __LINE__ ); \
}

#define VEER_ASSERT(_test, _msg) VEER_ASSERT_INTERNAL( _test, _msg );

#define VEER_LOG_INTERNAL( _level, _msg )	\
{											\
	std::stringstream VEER_LOG_INTERNAL__ss;	\
	VEER_LOG_INTERNAL__ss << _msg;			\
	debug::print( _level, VEER_LOG_INTERNAL__ss.str(), __FILE__, VEER_PRETTY_FUNCTION, __LINE__ ); \
}

#define VEER_LOG_ERROR(_msg) VEER_LOG_INTERNAL(debug::log_level::error, _msg );
#define VEER_LOG_WARNING(_msg) VEER_LOG_INTERNAL(debug::log_level::warn, _msg );
#define VEER_LOG(_msg) VEER_LOG_INTERNAL(debug::log_level::log, _msg );
#define VEER_LOG_DEBUG(_msg) VEER_LOG_INTERNAL(debug::log_level::debug, _msg );

#endif // CORE_DEBUG_H_INCLUDED
