#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsCommon/tm.hpp>

// imgproc
#include <IsComputerVision/io/img_io.hpp>

// nnabla
#include <IsNdArray/nd_array.hpp>
#include <IsNdArray/nd_array_extra.hpp>

#include <string>
#include <cstdio>

// utility
#include <test_utils.hpp>

namespace
{
    TEST(imgproc_io, bmp)
    {
        using namespace std;
        using namespace is::imgproc;
        using namespace is::imgproc::format;
        using namespace is::nbla;
        using namespace is::common;
        using byte = unsigned char;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        string dummy_filename = TEST_INPUT_MONO_256_DIR "Tree.bmp";

        // Bmp
        ImageIo<BmpFile> io_bmp;
        auto t_ndarray = io_bmp.load(dummy_filename, true);
        if (!t_ndarray)
        {
            std::cerr << "Error loaded ndarray is nullptr" << std::endl;
        }

        auto t_strides = t_ndarray->strides();
        auto t_shape = t_ndarray->shape();
        byte * data = t_ndarray->cast_data_and_get_pointer<byte>(ctx_cpu);
        std::printf("Image size(%ld, %ld, %ld)\n", t_shape[0], t_shape[1], t_shape[2]);

        // for (int c = 0; c < t_shape[0]; ++c)
        // {
        //     for (int y = 0; y < t_shape[1]; ++y)
        //     {
        //         for (int x = 0; x < t_shape[2]; ++x)
        //         {
        //             if (y == x)
        //                 data[c * t_strides[0] + y * t_strides[1] + x * t_strides[2]] = 0;
        //         }
        //     }
        // }
        for (int y = 0; y < t_shape[0]; y++)
        {
            for (int x = 0; x < t_shape[1]; x++)
            {
                for (int c = 0; c < t_shape[2]; c++)
                {
                    if (y == x)
                        data[y * t_strides[0] + x * t_strides[1] + c * t_strides[2]] = 0;
                }
            }
        }

        // Matplotlib
        int height = t_shape[0];
        int width = t_shape[1];
        // plt::imshow(data, height, width, 1); // BMPファイルの表示
        // plt::show();

        dummy_filename = TEST_OUTPUT_DIR "Tree_out.bmp";
        io_bmp.save(dummy_filename, t_ndarray, true);
    }


    
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}