/**
 * @file rtti.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <typeinfo>
#include <iostream>
#include <string>

#if defined(_MSC_VER)
    #if !defined(_DEBUG) || defined(NDEBUG)
        #define IS_SINGLETON_LOG(MESSAGE, SINGLETON)
    #else
        #define IS_SINGLETON_LOG(MESSAGE, SINGLETON)                                      \
            std::cout << "[" << #MESSAGE << "] a singleton `" << typeid(SINGLETON).name() \
                    << "`" << std::endl;
    #endif
#else
    #if defined(NDEBUG)
        #define IS_SINGLETON_LOG(MESSAGE, SINGLETON)
    #else
        // UNIX系だとlibstdc++に含まれる.
        // Windowsは未調査.
        // Shinichi Inoue 21/2/5
        #if __has_include(<cxxabi.h>)
            #include <cxxabi.h>
            #define IS_SINGLETON_LOG(MESSAGE, SINGLETON)                                \
            std::string singleton_name;                                                 \
            const std::type_info &type_id = typeid(SINGLETON); /* RTTI */               \
            int32 stat{-1};                                                             \
            char *name = abi::__cxa_demangle(type_id.name(), 0, 0, &stat);              \
            if (name != nullptr)                                                        \
            {                                                                           \
                if (stat == 0) /* success: stat == 0 */                                 \
                {                                                                       \
                    singleton_name = name;                                              \
                    std::cout << "[" << #MESSAGE << "] a singleton `" << singleton_name \
                            << "`" << std::endl;                                        \
                    ::free(name);                                                       \
                }                                                                       \
            }                                                                           \
            if (stat != 0)                                                              \
            {                                                                           \
                std::cout << "[" << #MESSAGE << "] a singleton `"                       \
                        << typeid(SINGLETON).name() << "`" << std::endl;                \
            }
        #else
            #define IS_SINGLETON_LOG(MESSAGE, SINGLETON)                \
                std::cout << "[" << #MESSAGE << "] a singleton `"       \
                        << typeid(SINGLETON).name() << "`" << std::endl;
        #endif
    #endif
#endif