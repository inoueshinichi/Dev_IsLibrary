#include <IsNdArray/exception.hpp>

#include <sstream>

namespace is
{
    namespace nbla
    {
        // enum class error_codeをエラー文字列として取得
        string get_error_string(error_code code)
		{
			switch (code)
			{
#define CASE_ERROR_STRING(code_name)    \
	case error_code::code_name:         \
		return #code_name

				CASE_ERROR_STRING(unclassified);
				CASE_ERROR_STRING(not_implemented);
				CASE_ERROR_STRING(value);
				CASE_ERROR_STRING(type);
				CASE_ERROR_STRING(memory);
				CASE_ERROR_STRING(io);
				CASE_ERROR_STRING(os);
				CASE_ERROR_STRING(target_specific);
				CASE_ERROR_STRING(target_specific_async);
				CASE_ERROR_STRING(runtime);
#undef CASE_ERROR_STRING
			}

			return std::string();
		}


        /* =============== Exception =============== */             
        Exception::Exception(error_code code, const string& msg, 
                             const string& func, const string& file, int line)
			: code_(code), msg_(msg), func_(func), file_(file), line_(line)
		{

			std::ostringstream ss;
			ss << get_error_string(code_) << " error in "
				<< func_ << std::endl << file_ << ":" << line_ << std::endl
				<< msg_ << std::endl;
			full_msg_ = ss.str();
		}


        Exception::~Exception() throw() {}


        const char* Exception::what() const throw()
		{ 
			return full_msg_.c_str(); 
		}
    }
}