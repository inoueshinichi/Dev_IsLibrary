/**
 * @file average_blur.hpp
 * @author your name (you@domain.com)
 * @brief 平均値フィルタ
 * @version 0.1
 * @date 2022-02-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

namespace is
{
    namespace imgproc
    {
        using namespace nbla;

        /**
         * @brief 平均値フィルタ
         *
         * @param src
         * @param ksize_x
         * @param ksize_y
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr average_blur(NdArrayPtr src, const Size &ksize);

    }
}
