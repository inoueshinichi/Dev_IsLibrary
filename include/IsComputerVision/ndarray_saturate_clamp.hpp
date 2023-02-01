/**
 * @file ndarray_saturate_clamp.hpp
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
#include <IsComputerVision/saturate_clamp.hpp>

namespace is
{
    namespace cv
    {
        using namespace nbla;

        /**
         * @brief saturate_clamp for NdArray
         *
         * @tparam T
         * @param src
         * @return NdArrayPtr
         */
        template <typename T>
        NdArrayPtr saturate_clamp(NdArrayPtr src)
        {
            const auto &ctx =
                SingletonManager::get<GlobalContext>()->get_current_context();

#define IS_SATURATE_CLAMP(dtype, type)                          \
    case dtypes::dtype:                                         \
    {                                                           \
        type *data = src->cast_data_and_get_pointer<type>(ctx); \
        for (size_t k = 0; k < src->size(); ++k)                \
        {                                                       \
            data[k] = (type)saturate_clamp<T>(data[k]);         \
        }                                                       \
    }                                                           \
    break

            auto dtype = src->dtype();
            switch (dtype)
            {
                IS_SATURATE_CLAMP(UBYTE, ubyte);
                IS_SATURATE_CLAMP(BYTE, byte);
                IS_SATURATE_CLAMP(USHORT, ushort);
                IS_SATURATE_CLAMP(SHORT, short);
                IS_SATURATE_CLAMP(UINT, uint);
                IS_SATURATE_CLAMP(INT, int);
                IS_SATURATE_CLAMP(ULONG, ulong);
                IS_SATURATE_CLAMP(LONG, long);
                IS_SATURATE_CLAMP(ULONGLONG, ulonglong);
                IS_SATURATE_CLAMP(LONGLONG, longlong);
                IS_SATURATE_CLAMP(FLOAT, float);
                IS_SATURATE_CLAMP(DOUBLE, double);
                IS_SATURATE_CLAMP(BOOL, bool);
                IS_SATURATE_CLAMP(LONGDOUBLE, longdouble);

            case dtypes::HALF:
            {
                Half *data = src->cast_data_and_get_pointer<Half>(ctx);
                for (size_t k = 0; k < src->size(); ++k)
                {
                    data[k] = (Half)saturate_clamp<T>((float)data[k]);
                }
            }
            break;
            }

            return src;
#undef IS_SATURATE_CLAMP
        }
    }
}
