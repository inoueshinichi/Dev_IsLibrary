/**
 * @file gaussian_blur.hpp
 * @author your name (you@domain.com)
 * @brief ガウシアンフィルタ
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
    namespace cv
    {
        using namespace nbla;

        /**
         * @brief ガウシアンフィルタ
         *
         * @param src
         * @param ksize_x
         * @param ksize_y
         * @param sigma_x
         * @param sigma_y
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr gaussian_blur(NdArrayPtr src, int ksize, double sigma = 0);
    }
}