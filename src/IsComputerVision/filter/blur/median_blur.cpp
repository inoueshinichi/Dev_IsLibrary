#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/median_blur.hpp>

namespace is
{
    namespace cv
    {
        NdArrayPtr median_blur(NdArrayPtr src, int ksize)
        {
            IS_DEBUG_NDARRAY_SHAPE_AS_IMAGE(src)

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();
            int sh_H = sh.at(0);
            int sh_W = sh.at(1);
            int sh_C = sh.at(2);
            int st_H = st.at(0);
            int st_W = st.at(1);
            int st_C = st.at(2);

            if (ksize % 2 == 0)
                ksize += 1;

            int hlf_ks = (int)(ksize / 2);

            // パディング
            auto extend = padding<ubyte>(src, hlf_ks, hlf_ks, IS_PADDING_MEAN);
            auto exsh = extend->shape();
            int exsh_H = exsh[0];
            int exsh_W = exsh[1];
            int exsh_C = exsh[2];
            auto exst = extend->strides();
            int exst_H = exst[0];
            int exst_W = exst[1];
            int exst_C = exst[2];
            ubyte *ex_data = extend->cast_data_and_get_pointer<ubyte>(ctx);

            // 演算結果
            auto dst = zeros<ubyte>(sh);
            ubyte *dst_data = dst->cast_data_and_get_pointer<ubyte>(ctx);

            /*チャネル毎のヒストグラム*/
            // 256, 128, 64, 32, 16, 8, 4, 2, (1) 
            const int HIST_NUM = 8;
            int **pp_hists = new int*[HIST_NUM];
            int *p_bins = new int[HIST_NUM];
            for (int k = 0, range = 256; k < HIST_NUM; ++k, range /= 2) 
            {
                pp_hists[k] = new int[range];
                IS_ZERO_MEMORY(pp_hists[k], sizeof(int) * range);
                p_bins[k] = 256 / range;
            }
    

            /*ヒストグラムを用いて中央値を求める*/
            // https://qiita.com/YVT/items/7734b342894c2f8247b3

            int mid = (ksize * ksize / 2) + 1; // 1スタート 3x3=9個なら1,2,3,4,5,6,7,8,9の5番
            int xt;
            for (int c = 0; c < sh_C; ++c) 
            {
              for (int y = 0; y < sh_H; ++y) 
              {
                for (int x = 0; x < sh_W; ++x) 
                {

                    if (y % 2 == 0) xt = x;
                    else xt = sh_W - 1 - x;
                    
                    if (xt == 0 && y == 0) 
                    {
                        // ヒストグラム初期化
                        for (int k = 0, range = 256; k < HIST_NUM; ++k, range /= 2) 
                        {
                            IS_ZERO_MEMORY(pp_hists[k], sizeof(int) * range);
                        }

                        // 階層bin別のヒストグラムを求める
                        for (int j = 0; j < ksize; ++j) 
                        {
                            for (int i = 0; i < ksize; ++i) 
                            {
                                const auto &lum = ex_data[j * exst_H + i * exst_W + c * exst_C];
                                for (int k = 0; k < HIST_NUM; ++k) 
                                {
                                    pp_hists[k][lum / p_bins[k]] += 1;
                                }
                            }
                        }

                    }
                    else if ((xt == 0 && y % 2 == 0) || (xt == sh_W - 1 && y % 2 == 1))
                    { // 両端
                        int t = y - 1;
                        int b = t + ksize;
                        for (int i = 0; i < ksize; ++i) 
                        {
                            const auto &lum_upside = ex_data[t * exst_H + (xt + i) * exst_W + c * exst_C];
                            const auto &lum_downside = ex_data[b * exst_H + (xt + i) * exst_W + c * exst_C];

                            // 各階層のhist
                            for (int k = 0; k < HIST_NUM; ++k) 
                            {
                                pp_hists[k][lum_upside / p_bins[k]] -= 1; // 減らす
                                pp_hists[k][lum_downside / p_bins[k]] += 1; // 増やす
                            }
                        }
                    }
                    else 
                    {
                        int l, r;
                        if (y % 2 == 0) 
                        {
                            l = xt - 1;
                            r = l + ksize;
                        }
                        else 
                        {
                            l = xt;
                            r = l + ksize;
                        }

                        for (int j = 0; j < ksize; ++j) 
                        {
                            const auto &lum_left = ex_data[(y + j) * exst_H + l * exst_W + c * exst_C];
                            const auto &lum_right = ex_data[(y + j) * exst_H + r * exst_W + c * exst_C];

                            // 各階層のhist
                            for (int k = 0; k < HIST_NUM; ++k) 
                            {
                                if (y % 2 == 0) 
                                { // 偶数行
                                    pp_hists[k][lum_left / p_bins[k]] -= 1; 
                                    pp_hists[k][lum_right / p_bins[k]] += 1;
                                }
                                else { // 奇数行
                                    pp_hists[k][lum_left / p_bins[k]] += 1; 
                                    pp_hists[k][lum_right / p_bins[k]] -= 1;
                                }
                            }
                        }
                    }

                    
                    // binの大きなヒストグラムから辿ってメディアンな輝度を求める.
                    ubyte index; // median lum
                    ubyte first, last, sum_hist;
                    int hierarchy_idx = HIST_NUM - 1;
                    first = 0; last = 256 / p_bins[hierarchy_idx];
                    sum_hist = 0; // 累積ヒストグラム
                    while (hierarchy_idx >= 0) 
                    {
                        for (index = first; index < last; ++index) 
                        {
                            int judge_sum_hist = sum_hist + pp_hists[hierarchy_idx][index];
                            if (mid <= judge_sum_hist) 
                            {
                                first = 2 * index;
                                last = 2 * (index + 1);
                                break;
                            }
                            else 
                            {
                                sum_hist += pp_hists[hierarchy_idx][index];
                            }
                            
                        }
                        hierarchy_idx--;
                    }

                    dst_data[y * st_H + xt * st_W + c * st_C] = index;
                }
              }
            }

            // メモリ破棄
            delete[] p_bins; p_bins = nullptr;
            for (int i = 0; i < HIST_NUM; ++i) 
            {
                delete[] pp_hists[i]; pp_hists[i] = nullptr;
            }
            delete[] pp_hists; pp_hists = nullptr;

            return dst;
        }
    }
}