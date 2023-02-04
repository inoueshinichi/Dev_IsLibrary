/**
 * @file padding.hpp
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

        /*Padding style*/
        enum
        {
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
        template <typename T>
        NdArrayPtr padding(NdArrayPtr src, int ex, int ey, int style)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();
            int st_H = st.at(0);
            int st_W = st.at(1);
            int st_C = st.at(2);
            const int H = sh.at(0);
            const int W = sh.at(1);
            const int C = sh.at(2);
            T *data = src->template cast_data_and_get_pointer<T>(ctx);

            int idx_ys = ey;
            int idx_xs = ex;
            int idx_ye = idx_ys + H;
            int idx_xe = idx_xs + W;

            auto ex_src = zeros<T>(Shape_t{H+2*ey, W+2*ex, C});
            auto ex_st = ex_src->strides();
            const auto ex_H = ex_st.at(0);
            const auto ex_W = ex_st.at(1);
            const auto ex_C = ex_st.at(2);
            T *data_ex = ex_src->template cast_data_and_get_pointer<T>(ctx);

            int tx, ty;
            for (int y = 0; y < H; ++y)
            {
                for (int x = 0; x < W; ++x)
                {
                    ty = y + ey;
                    tx = x + ex;
                    for (int c = 0; c < C; ++c)
                    {
                        data_ex[ty * ex_H + tx * ex_W + c * ex_C] =
                            data[y * st_H + x * st_W + c * st_C];
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
                            mean_left += data_ex[y * ex_H + (x + idx_xs) * ex_W + c * ex_C];
                            mean_right += data_ex[y * ex_H + (idx_xe - 1 - x) * ex_W + c * ex_C];
                        }
                        mean_left /= ex;
                        mean_right /= ex;

                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[y * ex_H + x * ex_W + c * ex_C] = saturate_clamp<T>(mean_left);
                            data_ex[y * ex_H + (idx_xe + x) * ex_W + c * ex_C] = saturate_clamp<T>(mean_right);
                        }
                    }

                    // 上端, 下端
                    float mean_top, mean_bottom;
                    for (int x = idx_xs; x < idx_xe; ++x)
                    {
                        mean_top = mean_bottom = 0.0f;

                        for (int y = 0; y < ey; ++y)
                        {
                            mean_top += data_ex[(idx_ys + y) * ex_H + x * ex_W + c * ex_C];
                            mean_bottom += data_ex[(idx_ye - 1 - y) * ex_H + x * ex_W + c * ex_C];
                        }
                        mean_top /= ey;
                        mean_bottom /= ey;

                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex[y * ex_H + x * ex_W + c * ex_C] = saturate_clamp<T>(mean_top);
                            data_ex[(idx_ye + y) * ex_H + x * ex_W + c * ex_C] = saturate_clamp<T>(mean_bottom);
                        }
                    }

                    // 四隅
                    float mean_tl, mean_tr, mean_bl, mean_br;
                    mean_tl = mean_tr = mean_bl = mean_br = 0.0f;

                    for (int y = 0; y < ey; ++y)
                    {
                        // 左上
                        mean_tl += data_ex[y * ex_H + idx_xs * ex_W + c * ex_C];
                        // 右上
                        mean_tr += data_ex[y * ex_H + (idx_xe - 1) * ex_W + c * ex_C];
                        // 左下
                        mean_bl += data_ex[(idx_ye + y) * ex_H + idx_xs * ex_W + c * ex_C];
                        // 右下
                        mean_br += data_ex[(idx_ye + y) * ex_H + (idx_xe - 1) * ex_W + c * ex_C];
                    }

                    for (int x = 0; x < ex; ++x)
                    {
                        // 左上
                        mean_tl += data_ex[idx_ys * ex_H + x * ex_W + c * ex_C];
                        // 右上
                        mean_tr += data_ex[idx_ys * ex_H + (idx_xe + x) * ex_W + c * ex_C];
                        // 左下
                        mean_bl += data_ex[(idx_ye - 1) * ex_H + x * ex_W + c * ex_C];
                        // 右下
                        mean_br += data_ex[(idx_ye - 1) * ex_H + (idx_xe + x) * ex_W + c * ex_C];
                    }

                    mean_tl /= (ex + ey);
                    mean_tr /= (ex + ey);
                    mean_bl /= (ex + ey);
                    mean_br /= (ex + ey);

                    for (int y = 0; y < ey; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[y * ex_H + x * ex_W + c * ex_C] = saturate_clamp<T>(mean_tl);
                            data_ex[y * ex_H + (x + idx_xe) * ex_W + c * ex_C] = saturate_clamp<T>(mean_tr);
                            data_ex[(y + idx_ye) * ex_H + x * ex_W + c * ex_C] = saturate_clamp<T>(mean_bl);
                            data_ex[(y + idx_ye) * ex_H + (x + idx_xe) * ex_W + c * ex_C] = saturate_clamp<T>(mean_br);
                        }
                    }
                }
            }
            else if (style == IS_PADDING_REFLECT)
            {
                // reflect
                // cba|abcdefg|gfe
                IS_ERROR(error_code::not_implemented, "No implement padding-reflect.");
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
                            data_ex[y * ex_H + x * ex_W + c * ex_C] =
                                data_ex[y * ex_H + idx_xs * ex_W + c * ex_C];
                            data_ex[y * ex_H + (idx_xe + x) * ex_W + c * ex_C] =
                                data_ex[y * ex_H + (idx_xe - 1) * ex_W + c * ex_C];
                        }
                    }

                    // 上端, 下端
                    for (int x = idx_xs; x < idx_xe; ++x)
                    {
                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex[y * ex_H + x * ex_W + c * ex_C] =
                                data_ex[idx_ys * ex_H + x * ex_W + c * ex_C];
                            data_ex[(idx_ye + y) * ex_H + x * ex_W + c * ex_C] =
                                data_ex[(idx_ye - 1) * ex_H + x * ex_W + c * ex_C];
                        }
                    }

                    // 四隅
                    T mean_tl = saturate_clamp<T>(
                        (
                            data_ex[(idx_ys - 1) * ex_H + idx_xs * ex_W + c * ex_C] +
                            data_ex[idx_ys * ex_H + (idx_xs - 1) * ex_W + c * ex_C]) /
                        2);

                    T mean_tr = saturate_clamp<T>(
                        (
                            data_ex[(idx_ys - 1) * ex_H + (idx_xe - 1) * ex_W + c * ex_C] +
                            data_ex[idx_ys * ex_H + idx_xe * ex_W + c * ex_C]) /
                        2);

                    T mean_bl = saturate_clamp<T>(
                        (
                            data_ex[(idx_ye - 1) * ex_H + (idx_xs - 1) * ex_W + c * ex_C] +
                            data_ex[idx_ye * ex_H * idx_xs * ex_W + c * ex_C]) /
                        2);
                    T mean_br = saturate_clamp<T>(
                        (
                            data_ex[(idx_ye - 1) * ex_H + idx_xe * ex_W + c * ex_C] +
                            data_ex[idx_ye * ex_H + (idx_xe - 1) * ex_W + c * ex_C]) /
                        2);

                    for (int y = 0; y < ey; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex[y * ex_H + x * ex_W + c * ex_C] = mean_tl;
                            data_ex[y * ex_H + (x + idx_xe) * ex_W + c * ex_C] = mean_tr;
                            data_ex[(y + idx_ye) * ex_H + x * ex_W + c * ex_C] = mean_bl;
                            data_ex[(y + idx_ye) * ex_H + (x + idx_xe) * ex_W + c * ex_C] = mean_br;
                        }
                    }
                }
            }
            else if (style == IS_PADDING_WARP)
            {
                // warp
                // efg|abcdefg|abc
                IS_ERROR(error_code::not_implemented, "No implement padding-warp.");
            }
            else
            {
                IS_ERROR(error_code::not_implemented, 
                    "Must be zero[0], mean[1], reflect[2], replicate[3] or warp[4]. Given is %d", style);
            }

            return ex_src;
        }
    }
}