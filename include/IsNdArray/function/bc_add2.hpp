#pragma once

#include <IsNdArray/function/utils/base_transform_binary.hpp>

namespace is
{
    namespace nbla
    {
       /**
        * @brief BcAdd2
        * Broadcastable Add2 operation.
        * This shouldn't be used by users. This is used in Add2.
        * Other elementwise binary operations are not implementing broadcastable
        * because the original implementations are already broadcastable.
        * Add2 is a special case in which in-place computation is allowed.
        * We need this for an implementation for broadcastable Add2.
        * If Add2's inputs require broadcasting, it's fallback into BcAdd2 operation.
        * See setup_impl of add2.cpp.
        */
       NBLA_DEFINE_TRANSFORM_BINARY_INPLACE(BcAdd2, x0 + x1, true)
    }
}