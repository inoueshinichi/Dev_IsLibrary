#pragma once

#include <IsNdArray/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_UNARY(Sign, (x > (T)0) ? (T)1 : ((x < (T)0) ? (T)-1 : (T)0))
    }
}
