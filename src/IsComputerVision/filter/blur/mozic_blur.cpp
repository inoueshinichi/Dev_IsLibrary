#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/mozic_blur.hpp>

namespace is
{
    namespace cv
    {
        using uchar = unsigned char;

        NdArrayPtr mozic_blur(NdArrayPtr src, int block) 
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);
            IS_DEBUG_CHECK_NDARRAY_STATE(__func__, IS_DEBUG_FLAG, src);
            using ubyte = uchar;

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();
            int channels = sh.at(0);
            int height = sh.at(1);
            int width = sh.at(2);

            int block_num_y = (int)(height / block);
            int block_num_x = (int)(width / block);
            int block_mod_y = height % block;
            int block_mod_x = width % block;

            ubyte* src_data = src->cast_data_and_get_pointer<ubyte>(ctx);

            // 演算結果
            auto dst = zeros<ubyte>(sh);
            ubyte *dst_data = dst->cast_data_and_get_pointer<ubyte>(ctx);

            double mean = 0.0;

            for (int c = 0; c < channels; ++c)
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
                                mean += src_data[st[0] * c + st[1] * y + st[2] * x];
                            }
                        }

                        mean /= (block * block);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[st[0] * c + st[1] * y + st[2] * x] = saturate_clamp<ubyte>(mean);
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
                                mean += src_data[st[0] * c + st[1] * y + st[2] * x];
                            }
                        }

                        mean /= (block * block_mod_x);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[st[0] * c + st[1] * y + st[2] * x] = saturate_clamp<ubyte>(mean);
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
                                mean += src_data[st[0] * c + st[1] * y + st[2] * x];
                            }
                        }

                        mean /= (block * block_mod_y);

                        for (int x = xs; x < xe; ++x)
                        {
                            for (int y = ys; y < ye; ++y)
                            {
                                dst_data[st[0] * c + st[1] * y + st[2] * x] = saturate_clamp<ubyte>(mean);
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
                                mean += src_data[st[0] * c + st[1] * y + st[2] * x];
                            }
                        }

                        mean /= (block_mod_y * block_mod_x);

                        for (int y = ys; y < ye; ++y)
                        {
                            for (int x = xs; x < xe; ++x)
                            {
                                dst_data[st[0] * c + st[1] * y + st[2] * x] = saturate_clamp<ubyte>(mean);
                            }
                        }
                    }
                }
            }

            return dst;
        }
    } // imgproc
}