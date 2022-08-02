#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Elementwise pow scalar
         * y_i = x_i ^ v
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
        NBLA_DEFINE_TRANSFORM_UNARY_1_INPLACE(PowScalar, a0 == 0.5f ? std::sqrt(x) : std::pow(x, (T)a0), double)
    }
}