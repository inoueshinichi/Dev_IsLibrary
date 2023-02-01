/**
 * @file exception.hpp
 * @author your name (you@domain.com)
 * @brief 自作例外
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <IsCommon/IsCommon.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace is {
    namespace common {
        using std::string;
        using std::vector;

    /**
     * @brief このマクロを通して例外をthrowする.
     * エラーコードは, common::error_codeのenum classで定義されている.
     * i.e : IS_ERROR(error_code::cuda_error, "Error size %d", size);
     */
#define IS_ERROR(code, msg, ...)                                                  \
    throw Exception(code, format_string(msg, ##__VA_ARGS__),                      \
                    __func__, __FILE__, __LINE__)

    /**
     * @brief 特定の条件を満たすかどうかをチェックする.
     * エラーが発生したら，メッセージを投げる.
     * i.e : IS_CHECK(size == 2, error_code::cuda_error, "Error size %d", size);
     */
#define IS_CHECK(condition, code, msg, ...)                                       \
    if (!(condition)) {                                                           \
        IS_ERROR(code, string("Failed `" #condition "`: ") + msg, ##__VA_ARGS__)  \
    }


    /**
     * @brief 強制終了
     * 
     */
#define IS_FORCE_ASSERT(condition, msg, ...)                                      \
    if (!(condition)) {                                                           \
        std::cerr << "Aborting: " << format_string(msg, ##__VA_ARGS__) << " at "  \
                  << __func__ << " in " << __FILE__ << ":" << __LINE__            \
                  << std::endl;                                                   \
        ::abort();                                                                \
    }

        /**
         * @brief Isにおけるエラーコード.
         * 開発者はsrc/nbla/exception.cppの中にあるget_error_string()関数に
         * `CASE_ERROR_STRING({code name});`を追加しなけｒばならない.
         */
        enum class error_code {
            // 独自例外コード
            unclassified = 0,
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
         * @brief Isにおける例外クラス.
         * // https://github.com/Itseez/opencv/blob/c3ad8af42a85a3d03c6dd5727c8b5f4f7585d1d2/modules/core/src/system.cpp
         * // https://github.com/Itseez/opencv/blob/9aeb8c8d5a35bf7ed5208459d46fdb6822c5692c/modules/core/include/opencv2/core/base.hpp
         * // https://github.com/Itseez/opencv/blob/b2d44663fdd90e4c50d4a06435492b5cb0f1021d/modules/core/include/opencv2/core.hpp
         * エラーコードは，上記の列挙型クラスerror_codeに列挙されている.
         * 開発者/ユーザーがこの例外を直接投げることは想定されていない.
         * 代わりにIs_ERRORマクロを使用すること.
         */
        class IS_COMMON_API Exception : public std::exception {
        protected:
            error_code code_;
            string full_msg_; // 表示されるFullメッセージ
            string msg_;      // エラーメッセージ
            string func_;     // エラーが発生した関数名
            string file_;     // エラーが発生したファイル名
            uint32 line_;     // エラーが発生した行番号

        public:
            Exception(error_code code, const string &msg, const string &func, const string &file, uint32 line);
            virtual ~Exception() throw();
            virtual const char *what() const throw();
        };
    }
}
