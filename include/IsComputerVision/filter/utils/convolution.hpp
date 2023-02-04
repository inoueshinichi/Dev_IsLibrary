/**
 * @file convolution.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-02
 * 
 * @copyright Copyright (c) 2023
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
         * @brief 畳み込み演算
         * [Note] Very slow
         *
         * @tparam T
         * @param dst
         * @param extend
         * @param ksize
         * @param kernel
         */
        template <typename T>
        void convolution(NdArrayPtr dst, NdArrayPtr extend,
                         const Size &ksize, double *kernel)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();

            // dst
            auto dst_sh = dst->shape();
            int dst_H = dst_sh.at(0);
            int dst_W = dst_sh.at(1);
            int dst_C = dst_sh.at(2);
            auto dst_st = dst->strides();
            size_t dst_stH = dst_st.at(0);
            size_t dst_stW = dst_st.at(1);
            size_t dst_stC = dst_st.at(2);
            T *data_dst = dst->cast_data_and_get_pointer<T>(ctx);

            // extend
            auto ex_st = extend->strides();
            size_t ex_stH = ex_st.at(0);
            size_t ex_stW = ex_st.at(1);
            size_t ex_stC = ex_st.at(2);
            T *data_ex = extend->cast_data_and_get_pointer<T>(ctx);

            int hlf_ksy = int(ksize.height() / 2);
            int hlf_ksx = int(ksize.width() / 2);

            double answer;

            for (int c = 0; c < dst_C; ++c)
            {
                for (int y = 0; y < dst_H; ++y)
                {
                    for (int x = 0; x < dst_W; ++x)
                    {
                        answer = 0.0f;

                        // kernel
                        for (int j = -hlf_ksy, kj = 0; j <= hlf_ksy; ++j, ++kj)
                        {
                            for (int i = -hlf_ksy, ki = 0; i <= hlf_ksx; ++i, ++ki)
                            {
                                int ty = (y + hlf_ksy) + j;
                                int tx = (x + hlf_ksx) + i;
                                answer += kernel[kj * ksize.width() + ki] *
                                          data_ex[ty * ex_stH + tx * ex_stW + c * ex_stC];
                            }
                        }

                        // 演算結果
                        data_dst[y * dst_stH + x * dst_stW + c * dst_stC] = saturate_clamp<T>(answer);
                    }
                }
            }
        }
    }
}