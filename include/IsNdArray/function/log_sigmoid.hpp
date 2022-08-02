#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        // log sigmoid 関数
        NBLA_DEFINE_TRANSFORM_UNARY(LogSigmoid, x > (T)0 ? -std::log(std::exp(-x) + (T)1) : x - std::log(std::exp(x) + (T)1))
    }
}