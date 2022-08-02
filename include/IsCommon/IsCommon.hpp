/**
 * @file defs.hpp
 * @author your name (you@domain.com)
 * @brief ライブラリ用定義
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#if defined(_MSC_VER)
// __func__の定義
#define __func__ __FUNCTION__
#endif

// dll/lib
#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(IsCommon_EXPORTS) || defined(IsCommon_dbg_EXPORTS)
        #define IS_COMMON_API __declspec(dllexport)
    #else
        #define IS_COMMON_API __declspec(dllimport)
    #endif
#else
    #define IS_COMMON_API
#endif

// debug log
#if 1
    #include <cstdio>
    #define IS_DEBUG_STREAM(format, ...) std::printf(format, ##__VA_ARGS__)
#else
    #define IS_DEBUG_STREAM(format, ...)
#endif

// コピーコンストラクタとコピー代入演算子の外部公開を禁止する
#define IS_DISABLE_COPY_AND_ASSIGN(class_name) \
private:                                       \
    class_name(const class_name &);            \
    class_name &operator=(const class_name &);



#if (__cplusplus < 201704L)
#define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::result_of<Func(Args...)>::type
#else
#define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::invoke_result<Func, Args...>::type
#endif