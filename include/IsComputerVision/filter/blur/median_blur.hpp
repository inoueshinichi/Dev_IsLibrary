/**
 * @file median_blur.hpp
 * @author your name (you@domain.com)
 * @brief メディアンフィルタ
 * @version 0.1
 * @date 2022-02-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <IsComputerVision/common.hpp>

namespace is
{
    namespace cv
    {
        using namespace nbla;

        /**
         * @brief メディアンフィルタ
         *
         * @param src
         * @param ksize_x
         * @param ksize_y
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr median_blur(NdArrayPtr src, int ksize);
    }
}