/**
 * @file debug.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <IsCommon/internal/defs.hpp>

// Log flag
#if (1)
    #define IS_DEBUG_FLAG
#endif

// Debug log
#if defined(IS_DEBUG_FLAG)
    #include <cstdio>
    #if __cplusplus <= 201703L // <= C++17
        #define IS_DEBUG_LOG(format, ...) std::printf(format, ##__VA_ARGS__);
    #else // >= C++20
        #ifdef _MSC_VER
            #if _MSC_VER >= 1925 // >= Visual Studio 2019 16.5.1
                #define IS_DEBUG_LOG(format, ...) std::printf(format __VA_OPT__(, ) __VA_ARGS__);
            #else                                               // < Visual Studio 2019 16.5.1
                // _MSVC_TRADITIONAL 従来:1, 準拠:0
                #if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // 従来型プリプロセッサ
                    #define IS_DEBUG_LOG(format, ...) std::printf(format IS_MACRO_VA_EXPAND(__VA_ARGS__));
                #else // 新型プリプロセッサ
                    #define IS_DEBUG_LOG(format, ...) std::printf(format, ##__VA_ARGS__);
                #endif
            #endif
        #else // GCC or Clang (>= C++20)
            #define IS_DEBUG_LOG(format, ...) std::printf(format __VA_OPT__(, ) __VA_ARGS__);
        #endif
    #endif
#else
    #define IS_DEBUG_LOG(format, ...)
#endif


// Class object func ckeck
#if (IS_DEBUG_FLAG)
    #define IS_DEBUG_OBJECT_FUNC(class_this_ptr)                            \
        CV_DEBUG_LOG("[%p] %s: %s at %s, %d:%d\n",                          \
            (void *)class_this_ptr, class_this_ptr->GetClassName().c_str(), \
            __func__, __FILE__, __LINE__);
#else
    #define IS_DEBUG_OBJECT_FUNC(class_this_ptr)
#endif

