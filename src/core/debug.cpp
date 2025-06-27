#include "debug.h"

namespace veer
{
    debug::assert_error::assert_error(std::string const & _message)
        : std::runtime_error("veer::utils::assert_error(" + _message + ")")
    { }

    void debug::assert_if(bool _condition, std::string_view const _message , const char* _file, const char* _function, int _line)
    {
        if(!_condition)
        {
            print(log_level::error, _message, _file, _function, _line );
            throw assert_error(_message.data());
        }
    }

	void debug::print(log_level _level, std::string_view const _message , const char* _file, const char* _function, int _line)
    {
        std::ostream& os = _level == log_level::error ? std::cerr : std::cout;
        
        // set the color of the text
        std::string prefix;
        switch(_level)
        {
            case log_level::debug:
                prefix = "\033[0;37m[DEBUG] ";
                break;
            case log_level::warn:
                prefix = "\033[0;33m[WARN] ";
                break;
            case log_level::error:
                prefix = "\033[0;31m[ERROR] ";
                break;
            default:
                prefix = "\033[0;36m[LOG] ";
        }

        os << prefix << _file << ":" << _line << " in " << _function << " : " << _message << "\033[0m" << std::endl;
    }
} // namespace veer