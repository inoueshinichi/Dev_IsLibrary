#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise maximum scalar
         * y_i = max(x_i, v)
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param val Value of the scalar.
         */
        NBLA_DEFINE_TRANSFORM_UNARY_1(MaximumScalar, (x > (T)a0) ? x : (T)a0, double)
    }
}