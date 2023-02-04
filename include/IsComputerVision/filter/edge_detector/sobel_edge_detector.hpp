/**
 * @file sobel_filter.hpp
 * @author your name (you@domain.com)
 * @brief ソベルフィルタ
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
         * @brief Sobel
         *
         * @param src
         * @param direction
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr sobel_edge_detector(NdArrayPtr src, int direction);
    }
}