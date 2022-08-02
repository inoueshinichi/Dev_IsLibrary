#pragma once

#include <IsNdArray/function/utils/base_transform_binary.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_BINARY(NotEqual, x0 != x1)
    }
}