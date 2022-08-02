#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_UNARY(Log1p, std::log1p(x))
    }
}