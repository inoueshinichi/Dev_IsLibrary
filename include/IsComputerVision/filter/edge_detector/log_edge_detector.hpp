/**
 * @file log_filter.hpp
 * @author your name (you@domain.com)
 * @brief LOGフィルタ
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
         * @brief LoG (Laplacian of Gaussian)
         * ノイズ除去 + 輪郭線抽出
         * @param src
         * @param sigma
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr log_edge_detector(NdArrayPtr src, double sigma = 0);

    } // imgproc
}