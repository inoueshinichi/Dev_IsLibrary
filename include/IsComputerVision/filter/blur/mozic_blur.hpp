/**
 * @file mozic_blur.hpp
 * @author your name (you@domain.com)
 * @brief モザイクフィルタ
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
         * @brief モザイクフィルタ
         *
         * @param src
         * @param block
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr mozic_blur(NdArrayPtr src, int block);
    }
}