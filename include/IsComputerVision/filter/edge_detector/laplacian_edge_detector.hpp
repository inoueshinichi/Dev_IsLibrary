/**
 * @file lablacian_filter.hpp
 * @author your name (you@domain.com)
 * @brief ラプラシアンフィルタ
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
         * @brief Laplacian
         *
         * @param src
         * @param is_elem_8
         * @return NdArrayPtr
         */
        IS_CV_API NdArrayPtr laplacian_edge_detector(NdArrayPtr src, bool is_elem_8 = true);
    }
}