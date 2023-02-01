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

#include <IsComputerVision/IsComputerVision.hpp>


#define IS_DEBUG_CHECK_NDARRAY_STATE(func, flag, ndarray) \
    if (flag > 0)                                         \
    {                                                     \
        auto ndim = ndarray->ndim();                      \
        auto shape = ndarray->shape();                    \
        auto strides = ndarray->strides();                \
        auto dtype = ndarray->dtype();                    \
        auto type = nbla::dtype_to_string(dtype);         \
        std::ostringstream oss;                           \
        oss << #func << ", ";                             \
        oss << "Type: " << type << ", ";                  \
        oss << "ndim: " << ndim << ", ";                  \
        oss << "shape: (";                                \
        for (int32 d = 0; d < ndim; ++d)                  \
        {                                                 \
            oss << shape[d];                              \
            if (d != ndim - 1)                            \
            {                                             \
                oss << ",";                               \
            }                                             \
            else                                          \
            {                                             \
                oss << "), ";                             \
            }                                             \
        }                                                 \
                                                          \
        oss << "strides: (";                              \
        for (int32 d = 0; d < ndim; ++d)                  \
        {                                                 \
            oss << strides[d];                            \
            if (d != ndim - 1)                            \
            {                                             \
                oss << ",";                               \
            }                                             \
            else                                          \
            {                                             \
                oss << ")";                               \
            }                                             \
        }                                                 \
        std::cout << oss.str() << std::endl;              \
    }


#define IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(ndarray)                                       \
    {                                                                                  \
        if (!ndarray.get())                                                            \
        {                                                                              \
            throw std::runtime_error("NdArray is nullptr.");                           \
        }                                                                              \
        auto ndim = ndarray->ndim();                                                   \
        auto shape = ndarray->shape();                                                 \
        auto strides = ndarray->strides();                                             \
                                                                                       \
        if (ndim != 3)                                                                 \
        {                                                                              \
            throw std::runtime_error(                                                  \
                is::common::format_string("Miss matching 3 ndim. Given is %d", ndim)); \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            /*(C, H, W)*/                                                              \
            int32 channels = shape[0];                                                 \
            if (channels > 3 || channels == 2)                                         \
            {                                                                          \
                throw std::runtime_error(is::common::format_string(                    \
                    "Miss matching channels of top shape (<=3 or ==2). Given is %d",   \
                    channels));                                                        \
            }                                                                          \
        }                                                                              \
    }



