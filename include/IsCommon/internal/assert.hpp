/**
 * @file assert.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <IsCommon/internal/defs.hpp>
#include <IsCommon/internal/format_string.hpp>

////////////////
// Custom Assert
////////////////
#if defined(_DEBUG)
    #define IS_CUSTOM_ASSERT
#endif


#if defined(IS_CUSTOM_ASSERT)
    #define IS_DEBUG_ASSERT(condition, msg, ...)                                  \
    if (!(condition) &&                                                           \
        is::common::custom_assert(is::common::format_string(msg, ##__VA_ARGS__)   \
                                  __func__,                                       \
                                  __FILE__,                                       \
                                  __LINE__))                                      \
    {                                                                             \
        _asm int 3; /*デバッガの起動*/                                              \
    }
        
#else
    #define IS_DEBUG_ASSERT(condition, msg, ...)
#endif


namespace is
{
    namespace common
    {
        using std::string;
        bool custom_assert(const string& msg, const string& func, const string& file, int line)
        {
            return true;
        }
    }
}