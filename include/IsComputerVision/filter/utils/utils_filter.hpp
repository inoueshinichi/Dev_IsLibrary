#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

#include <cstdio>

#if defined(_MSC_VER)
// __func__の定義
#define __func__ __FUNCTION__
#endif

#ifdef _OPENMP
    #include <omp.h>
#endif

/**
 * @todo
 * [OK] Padding
 * [] Border
 * [OK] Convolution
 * [] im2col_2d
 * [] col2im_2d
 * [] im2col_3d
 * [] col2im_3d
 */

namespace is
{
    namespace cv
    {
        using namespace nbla;

        /*Padding style*/
        enum {
            IS_PADDING_ZERO = 0,
            IS_PADDING_MEAN = 1,
            IS_PADDING_REFLECT = 2,
            IS_PADDING_REPLICATE = 3,
            IS_PADDING_WARP = 4
        };

        /**
         * @brief reflectとwarpが未実装
         * 
         * @tparam T 
         * @param src 
         * @param ex 
         * @param ey 
         * @param style 
         * 
         * @note 0: zero
         *       1: mean
         *       2: reflect
         *       3: replicate
         *       4: warp
         * 
         * @return NdArrayPtr 
         */
        template < typename T>
        NdArrayPtr padding(NdArrayPtr src, int ex, int ey, int style)
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();
            int st0 = st.at(0);
            int st1 = st.at(1);
            int st2 = st.at(2);
            const int C = sh.at(0);
            const int H = sh.at(1);
            const int W = sh.at(2);
            T *data = src->template cast_data_and_get_pointer<T>(ctx);

            int idx_ys = ey;
            int idx_xs = ex;
            int idx_ye = idx_ys + H;
            int idx_xe = idx_xs + W;

            auto ex_src = zeros<T>(Shape_t{C, H + 2 * ey, W + 2 * ex});
            auto ex_st = ex_src->strides();
            const auto ex_C = ex_st.at(0);
            const auto ex_H = ex_st.at(1);
            const auto ex_W = ex_st.at(2);
            T *data_ex = ex_src->template cast_data_and_get_pointer<T>(ctx);

            int tx, ty;
            for (int c = 0; c < C; ++c)
            {
                for (int y = 0; y < H; ++y) 
                {
                    for (int x = 0; x < W; ++x) 
                    {
                        ty = y + ey;
                        tx = x + ex;
                        data_ex[c * ex_C + ty * ex_H + tx * ex_W] = 
                            data[c * st0 + y * st1 + x * st2];
                    }
                }
            }

            if (style == IS_PADDING_ZERO)
            {
                // zero
                // 000|abcdefg|000
                return ex_src;
            }
            else if (style == IS_PADDING_MEAN)
            {
                // mean
                for (int c = 0; c < C; ++c)
                {
                    // 左端, 右端
                    float mean_left, mean_right;
                    for (int y = idx_ys; y < idx_ye; ++y)
                    {
                        mean_left = mean_right = 0.0f;

                        for (int x = 0; x < ex; ++x)
                        {
                            mean_left += data_ex[c * ex_C + y * ex_H + (x + idx_xs) * ex_W];
                            mean_right += data_ex[c * ex_C + y * ex_H + (idx_xe - 1 - x) * ex_W];
                        }
                        mean_left /= ex;
                        mean_right /= ex;

                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = 
                                saturate_clamp<T>(mean_left);
                            data_ex[c * ex_C + y * ex_H + (idx_xe + x) * ex_W] = 
                                saturate_clamp<T>(mean_right);
                        }
                    }

                    // 上端, 下端
                    float mean_top, mean_bottom;
                    for (int x = idx_xs; x < idx_xe; ++x)
                    {
                        mean_top = mean_bottom = 0.0f;

                        for (int y = 0; y < ey; ++y)
                        {
                            mean_top += data_ex[c * ex_C + (idx_ys + y) * ex_H + x * ex_W];
                            mean_bottom += data_ex[c * ex_C + (idx_ye - 1 - y) * ex_H + x * ex_W];
                        }
                        mean_top /= ey;
                        mean_bottom /= ey;

                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = saturate_clamp<T>(mean_top);
                            data_ex[c * ex_C + (idx_ye + y) * ex_H + x * ex_W] = 
                                saturate_clamp<T>(mean_bottom);
                        }
                    }

                    // 四隅
                    float mean_tl, mean_tr, mean_bl, mean_br;
                    mean_tl = mean_tr = mean_bl = mean_br = 0.0f;

                    for (int y = 0; y < ey; ++y)
                    {
                        // 左上
                        mean_tl += data_ex[c * ex_C + y * ex_H + idx_xs * ex_W];
                        // 右上
                        mean_tr += data_ex[c * ex_C + y * ex_H + (idx_xe - 1) * ex_W];
                        // 左下
                        mean_bl += data_ex[c * ex_C + (idx_ye + y) * ex_H + idx_xs * ex_W];
                        // 右下
                        mean_br += data_ex[c * ex_C + (idx_ye + y) * ex_H + (idx_xe - 1) * ex_W];
                    }

                    for (int x = 0; x < ex; ++x)
                    {
                        // 左上
                        mean_tl += data_ex[c * ex_C + idx_ys * ex_H + x * ex_W];
                        // 右上
                        mean_tr += data_ex[c * ex_C + idx_ys * ex_H + (idx_xe + x) * ex_W];
                        // 左下
                        mean_bl += data_ex[c * ex_C + (idx_ye - 1) * ex_H + x * ex_W];
                        // 右下
                        mean_br += data_ex[c * ex_C + (idx_ye - 1) * ex_H + (idx_xe + x) * ex_W];
                    }

                    mean_tl /= (ex + ey);
                    mean_tr /= (ex + ey);
                    mean_bl /= (ex + ey);
                    mean_br /= (ex + ey);

                    for (int y = 0; y < ey; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = saturate_clamp<T>(mean_tl);
                            data_ex[c * ex_C + y * ex_H + (x + idx_xe) * ex_W] = saturate_clamp<T>(mean_tr);
                            data_ex[c * ex_C + (y + idx_ye) * ex_H + x * ex_W] = saturate_clamp<T>(mean_bl);
                            data_ex[c * ex_C + (y + idx_ye) * ex_H + (x + idx_xe) * ex_W] = 
                                saturate_clamp<T>(mean_br);
                        }
                    }
                }
            }
            else if (style == IS_PADDING_REFLECT)
            {
                // reflect
                // cba|abcdefg|gfe
            }
            else if (style == IS_PADDING_REPLICATE)
            {
                // replicate
                // aaa|abcdefg|ggg

                for (int c = 0; c < C; ++c)
                {
                    // 左端, 右端
                    for (int y = idx_ys; y < idx_ye; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = 
                                data_ex[c * ex_C + y * ex_H + idx_xs * ex_W];
                            data_ex[c * ex_C + y * ex_H + (idx_xe + x) * ex_W] = 
                                data_ex[c * ex_C + y * ex_H + (idx_xe - 1) * ex_W];
                        }
                    }

                    // 上端, 下端
                    for (int x = idx_xs; x < idx_xe; ++x)
                    {
                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = 
                                data_ex[c * ex_C + idx_ys * ex_H + x * ex_W];
                            data_ex[c * ex_C + (idx_ye + y) * ex_H + x * ex_W] =
                                data_ex[c * ex_C + (idx_ye - 1) * ex_H + x * ex_W];
                        }
                    }

                    // 四隅
                    T mean_tl = saturate_clamp<T>(
                        (
                            data_ex[c * ex_C + (idx_ys - 1) * ex_H + idx_xs * ex_W] + 
                            data_ex[c * ex_C + idx_ys * ex_H + (idx_xs - 1) * ex_W]
                        ) / 2
                    );

                    T mean_tr = saturate_clamp<T>(
                        (
                            data_ex[c * ex_C + (idx_ys - 1) * ex_H + (idx_xe - 1) * ex_W] + 
                            data_ex[c * ex_C + idx_ys * ex_H + idx_xe * ex_W]
                        ) / 2
                    );

                    T mean_bl = saturate_clamp<T>(
                        (
                            data_ex[c * ex_C + (idx_ye - 1) * ex_H + (idx_xs - 1) * ex_W] +
                            data_ex[c * ex_C + idx_ye * ex_H * idx_xs * ex_W]
                        ) / 2
                    );
                    T mean_br = saturate_clamp<T>(
                        (
                            data_ex[c * ex_C + (idx_ye - 1) * ex_H + idx_xe * ex_W] +
                            data_ex[c * ex_C + idx_ye * ex_H + (idx_xe - 1) * ex_W]
                        ) / 2
                    );

                    for (int y = 0; y < ey; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[c * ex_C + y * ex_H + x * ex_W] = mean_tl;
                            data_ex[c * ex_C + y * ex_H + (x + idx_xe) * ex_W] = mean_tr;
                            data_ex[c * ex_C + (y + idx_ye) * ex_H + x * ex_W] = mean_bl;
                            data_ex[c * ex_C + (y + idx_ye) * ex_H + (x + idx_xe) * ex_W] =
                                mean_br;
                        }
                    }
                }
            }
            else if (style == IS_PADDING_WARP)
            {
                // warp
                // efg|abcdefg|abc
            }
            else
            {
                std::string msg = is::common::format_string(
                    "No matching `zero[0]`,`mean[1]`,`reflect[2]`,`replicate[3]` or `warp[4]`. Given is %d", 
                    style);
                throw std::runtime_error(msg);
            }

            return ex_src;
        }

        
        
        // NdArrayPtr m2col(NdArrayPyt src, int size)
        // {
        //     /* Matric -> Column */
        //     return std::make_shared<NdArray>();
        // }

        
        // NdArrayPtr col2m(NdArrayPtr src, int size)
        // {
        //     /* Column -> Matrix */
        //     return std::make_shared<NdArray>();
        // }

        /**
         * @brief 畳み込み演算
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
            int dst_C = dst_sh.at(0);
            int dst_H = dst_sh.at(1);
            int dst_W = dst_sh.at(2);
            auto dst_st = dst->strides();
            size_t dst_st0 = dst_st.at(0);
            size_t dst_st1 = dst_st.at(1);
            size_t dst_st2 = dst_st.at(2);
            T *data_dst = dst->cast_data_and_get_pointer<T>(ctx);

            // extend
            auto ex_sh = extend->shape();
            int ex_C = ex_sh.at(0);
            auto ex_st = extend->strides();
            size_t ex_st0 = ex_st.at(0);
            size_t ex_st1 = ex_st.at(1);
            size_t ex_st2 = ex_st.at(2);
            T *data_ex = extend->cast_data_and_get_pointer<T>(ctx);

            if (dst_C != ex_C)
                return;

            int hlf_ksy = int(ksize.height() / 2);
            int hlf_ksx = int(ksize.width() / 2);

            double answer;

            // OpenMPにより畳み込みループタスクを複数スレッドに分割して並列実行

            for (int c = 0; c < dst_C; ++c)
            {
// #ifdef _OPENMP
// #pragma omp parallel for
// #endif
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
                                    data_ex[c * ex_st0 + ty * ex_st1 + tx * ex_st2];
                            }
                        }

                        // 演算結果
                        data_dst[c * dst_st0 + y * dst_st1 + x * dst_st2] = saturate_clamp<T>(answer);
                    }
                }
            }
        }


    


    } // imgproc
} // is