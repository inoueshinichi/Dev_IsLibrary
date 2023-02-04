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

#if (!IS_DEBUG_FLAG)
    #define IS_DEBUG_NDARRAY_STATE(ndarray, type, ndim, shape, strides)
#else
    #define IS_DEBUG_NDARRAY_STATE(ndarray, type, ndim, shape, strides) \
        std::ostringstream oss;                                         \
        oss << "[" << #ndarray << "]: ";                                \
        oss << "type " << type << ", ";                                 \
        oss << "ndim " << ndim << ", ";                                 \
        oss << "shape (";                                               \
        for (int d = 0; d < ndim; ++d)                                  \
        {                                                               \
            oss << shape[d];                                            \
            if (d != ndim - 1)                                          \
            {                                                           \
                oss << ",";                                             \
            }                                                           \
            else                                                        \
            {                                                           \
                oss << "), ";                                           \
            }                                                           \
        }                                                               \
                                                                        \
        oss << "strides (";                                             \
        for (int32 d = 0; d < ndim; ++d)                                \
        {                                                               \
            oss << strides[d];                                          \
            if (d != ndim - 1)                                          \
            {                                                           \
                oss << ",";                                             \
            }                                                           \
            else                                                        \
            {                                                           \
                oss << ")";                                             \
            }                                                           \
        }                                                               \
        oss << " at " << __func__ << " in ";                            \
        oss << __FILE__ << ":" << __LINE__;                             \
        std::cout << oss.str() << std::endl;
#endif

#define IS_DEBUG_NDARRAY_SHAPE_AS_IMAGE(ndarray)                                       \
    {                                                                                  \
        auto ndim = ndarray->ndim();                                                   \
        auto shape = ndarray->shape();                                                 \
        auto strides = ndarray->strides();                                             \
        auto dtype = ndarray->dtype();                                                 \
        auto type = nbla::dtype_to_string(dtype);                                      \
        IS_DEBUG_NDARRAY_STATE(ndarray, type, ndim, shape, strides)                    \
        if (ndim != 3)                                                                 \
        {                                                                              \
            throw IS_ERROR(error_code::value, "%s is not 3 dimension. Given is %d.",   \
                           #ndarray, ndim);                                            \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            /*(H,W,C)*/                                                                \
            int channels = shape[2];                                                   \
            if (channels > 3 || channels == 2)                                         \
            {                                                                          \
                throw IS_ERROR(error_code::value,                                      \
                               "Channels(shape[2]) of %s must be 1 or 3. Given is %d", \
                               #ndarray, channels));                                   \
            }                                                                          \
        }                                                                              \
    }
