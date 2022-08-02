#pragma once

#include <IsNdArray/function/utils/base_transform_binary.hpp>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise division
         * y_i = x0_i / x1_i
         * 
         * Inputs:
         * - N-D array.
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         */
        NBLA_DEFINE_TRANSFORM_BINARY_INPLACE(Div2, x0 / x1, true)
    }
}