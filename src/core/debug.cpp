#include "debug.h"

namespace veer
{
    void debug::assert_if(bool _condition, const char* _message , const char* _file, const char* _function, int _line)
    {
        if(!_condition)
        {
            print(log_level::error, _message, _file, _function, _line );
        }
    }

	void debug::print(log_level _level, const char* _message , const char* _file, const char* _function, int _line)
    {
        std::ostream& os = _level == log_level::error ? std::cerr : std::cout;
        
        const char* s_prefix_table[] =
        {
            "\033[0;37m[DEBUG] ",   // log_level::debug,
            "\033[0;36m[LOG] ",     // log_level::log
            "\033[0;33m[WARN] ",    // log_level::warn,
            "\033[0;31m[ERROR] ",   // log_level::error,
        };

        os << s_prefix_table[static_cast<size_t>(_level)] /*<< _file*/ << ":" << _line << " in " << _function << " : " << _message << "\033[0m" << std::endl;
    }
} // namespace veer