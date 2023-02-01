/**
 * @file defs.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(IsComputerVision_EXPORTS) || defined(IsComputerVision_dbg_EXPORTS)
        #define IS_CV_API __declspec(dllexport)
    #else
        #define IS_CV_API __declspec(dllimport)
    #endif
#else
    #define IS_CV_API
#endif




