#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise minimum scalar
         * y_i = min(x_i, v)
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
        NBLA_DEFINE_TRANSFORM_UNARY_1(MinimumScalar, (x < (T)a0) ? x : (T)a0, double)
    }
}