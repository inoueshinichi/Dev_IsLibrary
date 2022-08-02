#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        // Sigmoid関数
        NBLA_DEFINE_TRANSFORM_UNARY(Sigmoid, ((T)1 / ((T)1 + std::exp(-x))))
    }
}