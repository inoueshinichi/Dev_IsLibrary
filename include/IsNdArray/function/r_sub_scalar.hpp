#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise r sub scalar
         * y_i = v - x_i
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
        NBLA_DEFINE_TRANSFORM_UNARY_1_INPLACE(RSubScalar, (T)a0 - x, double)
    }
}