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
    #if __cplusplus <= 201703L // <= C++17
        #define IS_DEBUG_STREAM(format, ...) std::printf(format, ##__VA_ARGS__)
    #else // >= C++20
        #ifdef _MSC_VER
            #if _MSC_VER >= 1925 // >= Visual Studio 2019 16.5.1
                #define IS_DEBUG_STREAM(format, ...) std::printf(format __VA_OPT__(,) __VA_ARGS__)
            #else // < Visual Studio 2019 16.5.1
                // _MSVC_TRADITIONAL 従来:1, 準拠:0
                #if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // 従来型プリプロセッサ
                    #define IS_EXPAND(...) __VA_ARGS__
                    #define IS_DEBUG_STREAM(format, ...) std::printf(format IS_EXPAND(__VA_ARGS__))
                    #undef IS_EXPAND
                #else // 新型プリプロセッサ
                    #define IS_DEBUG_STREAM(format, ...) std::printf(format, ##__VA_ARGS__)
                #endif
            #endif
        #else // GCC or Clang (>= C++20)
            #define IS_DEBUG_STREAM(format, ...) std::printf(format __VA_OPT__(,) __VA_ARGS__)
        #endif
    #endif
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