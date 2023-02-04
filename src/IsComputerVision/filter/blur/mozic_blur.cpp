#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/mozic_blur.hpp>

namespace is
{
    namespace cv
    {
        using uchar = unsigned char;

        NdArrayPtr mozic_blur(NdArrayPtr src, int block) 
        {
            IS_DEBUG_NDARRAY_SHAPE_AS_IMAGE(src)
            
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            int sh_H = sh.at(0);
            int sh_W = sh.at(1);
            int sh_C = sh.at(2);
            auto st = src->strides();
            int st_H = st.at(0);
            int st_W = st.at(1);
            int st_C = st.at(2);

            int block_num_y = (int)(sh_H / block);
            int block_num_x = (int)(sh_W / block);
            int block_mod_y = sh_H % block;
            int block_mod_x = sh_W % block;

            ubyte* src_data = src->cast_data_and_get_pointer<ubyte>(ctx);

            // 演算結果
            auto dst = zeros<ubyte>(sh);
            ubyte *dst_data = dst->cast_data_and_get_pointer<ubyte>(ctx);

            double mean = 0.0;

            for (int c = 0; c < sh_C; ++c)
            {
                // モザイク計算
                for (int jy = 0, ys = 0, ye = block; jy < block_num_y; ++jy, ys += block, ye += block) 
                {
                    for (int ix = 0, xs = 0, xe = block; ix < block_num_x; ++ix, xs += block, xe += block) 
                    {
                        mean = 0.0;

                        // mean
                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                mean += src_data[y * st_H + x * st_W + c * st_C];
                            }
                        }

                        mean /= (block * block);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[y * st_H + x * st_W + c * st_C] = saturate_clamp<ubyte>(mean);
                            }
                        }
                    }

                    // 右端数の平均
                    if (block_mod_x > 0)
                    {
                        int xs = block_num_x * block;
                        int xe = xs + block_mod_x;
                        mean = 0.0;

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                mean += src_data[y * st_H + x * st_W + c * st_C];
                            }
                        }

                        mean /= (block * block_mod_x);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[y * st_H + x * st_W + c * st_C] = saturate_clamp<ubyte>(mean);
                            }
                        }
                    }
                }

                // 下端数の平均
                if (block_mod_y > 0)
                {
                    int ys = block_num_y * block;
                    int ye = ys + block_mod_y;
                    
                    for (int ix = 0, xs = 0, xe = block; ix < block_num_x; ++ix, xs += block, xe += block)
                    {
                        mean = 0.0;

                        for (int x = xs; x < xe; ++x)
                        {
                            for (int y = ys; y < ye; ++y)
                            {
                                mean += src_data[y * st_H + x * st_W + c * st_C];
                            }
                        }

                        mean /= (block * block_mod_y);

                        for (int x = xs; x < xe; ++x)
                        {
                            for (int y = ys; y < ye; ++y)
                            {
                                dst_data[y * st_H + x * st_W + c * st_C] = saturate_clamp<ubyte>(mean);
                            }
                        }
                    }
                    
                    // 右端
                    if (block_mod_x > 0)
                    {
                        int xs = block * block_num_x;
                        int xe = xs + block_mod_x;
                        mean = 0.0;

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                mean += src_data[y * st_H + x * st_W + c * st_C];
                            }
                        }

                        mean /= (block_mod_y * block_mod_x);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[y * st_H + x * st_W + c * st_C] = saturate_clamp<ubyte>(mean);
                            }
                        }
                    }
                }
            }

            return dst;
        }
    } // imgproc
}