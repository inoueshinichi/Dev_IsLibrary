#pragma once
#if defined(_MSC_VER)
// __func__の定義
#	define __func__ __FUNCTION__
#endif

#include <IsNdArray/IsNdArray.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::string;
		using std::vector;

        /**
         * @brief Nnablaでは, このマクロを通して例外をthrowする.
         * エラーコードはnbla::error_codeのenum classで定義されている.
         * i.e : NBLA_ERROR(error_code::cuda_error, "Error size %d", size);
         */
#define NBLA_ERROR(code, msg, ...)                                                          \
	throw Exception(code, format_string(msg, ##__VA_ARGS__), __func__, __FILE__, __LINE__);

        /**
         * @brief 特定の条件を満たすかどうかをチェックする.
         * エラーが発生したら，メッセージを投げる.
         * i.e : NBLA_CHECK(size == 2, error_code::cuda_error, "Error size %d", size);
         */
#define NBLA_CHECK(condition, code, msg, ...)                                                \
	if (!(condition))                                                                        \
	{                                                                                        \
		NBLA_ERROR(code, string("Failed `" #condition "`: ") + msg, ##__VA_ARGS__)           \
	}


#define NBLA_FORCE_ASSERT(condition, msg, ...)                                               \
	if (!(condition))																		 \
	{																						 \
		std::cerr << "Aborting: " << format_string(msg, ##__VA_ARGS__) << " at "             \
				  << __func__ << " in " << __FILE__ << ":" << __LINE__                       \
				  << std::endl;                                                              \
		::abort();                                                                           \
	}


        /**
         * @brief Nnablaにおけるエラーコード.
         * 開発者はsrc/nbla/exception.cppの中にあるget_error_string()関数に
         * `CASE_ERROR_STRING({code name});`を追加しなけｒばならない.
         */
        enum class error_code
        {
            unclassified,
            not_implemented,
            value,
            type,
            memory,
            io,
            os,
            target_specific,
            target_specific_async,
            runtime
        };


        /**
         * @brief Get the error string object
         * 例外エラーを文字列で取得する関数
         * @param code 
         * @return string 
         */
        string get_error_string(error_code code);


        /**
         * @brief Nnablaにおける例外クラス.
         * // https://github.com/Itseez/opencv/blob/c3ad8af42a85a3d03c6dd5727c8b5f4f7585d1d2/modules/core/src/system.cpp
         * // https://github.com/Itseez/opencv/blob/9aeb8c8d5a35bf7ed5208459d46fdb6822c5692c/modules/core/include/opencv2/core/base.hpp
         * // https://github.com/Itseez/opencv/blob/b2d44663fdd90e4c50d4a06435492b5cb0f1021d/modules/core/include/opencv2/core.hpp
         * エラーコードは，上記の列挙型クラスerror_codeに列挙されている.
         * 開発者/ユーザーがこの例外を直接投げることは想定されていない.
         * 代わりにNBLA_ERRORマクロを使用すること.
         */
        class NBLA_API Exception : public std::exception
		{
		protected:
			error_code code_;
			string full_msg_; // 表示されるFullメッセージ
			string msg_;      // エラーメッセージ
			string func_;     // エラーが発生した関数名
			string file_;     // エラーが発生したファイル名
			int    line_;     // エラーが発生した行番号

		public:
			Exception(error_code code, const string& msg, const string& func, const string& file, int line);
			virtual ~Exception() throw();
			virtual const char* what() const throw();
		};


        /**
         * @brief string formatter関数
         * 
         * @tparam T 
         * @tparam Args 
         * @param format 
         * @param first 
         * @param rest 
         * @return string 
         */
        template <typename T, typename... Args>
		string format_string(const string& format, T first, Args... rest)
		{
			int size = std::snprintf(nullptr, 0, format.c_str(), first, rest...);
			if (size < 0)
			{
				std::printf("fatal error in format_string function: snprintf failed\n");
				std::abort();
			}
			vector<char> buffer(size + 1);
			snprintf(buffer.data(), size + 1, format.c_str(), first, rest...);
			return string(buffer.data(), buffer.data() + size);
		}


        /**
         * @brief formatなしでのstring_formatter関数
         * 
         * @param format 
         * @return string 
         */
        inline string format_string(const string& format)
		{
			for (auto iter = format.begin(); iter != format.end(); iter++)
			{
				if (*iter == '%')
				{
					if (*(iter + 1) == '%')
					{
						iter++;
					}
					else
					{
						NBLA_ERROR(error_code::unclassified, "Invalid format string %s", format.c_str());
					}
				}
			}
			return format;
		}

    }
}