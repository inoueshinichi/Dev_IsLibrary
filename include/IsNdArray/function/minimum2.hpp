#pragma once

#include <IsNdArray/function/utils/base_transform_binary.hpp>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise minimum
         * y_i = min(x0_i, x1_i)
         * 
         * Inputs:
         * - N-D array.
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         */
        NBLA_DEFINE_TRANSFORM_BINARY(Minimum2, (x0 < x1) ? x0 : x1)
    }
}