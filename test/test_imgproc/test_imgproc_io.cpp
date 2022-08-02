#include <gtest/gtest.h>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

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
        auto test_ndarray = io_bmp.load(dummy_filename, true);
        if (!test_ndarray)
        {
            std::cerr << "Error loaded ndarray is nullptr" << std::endl;
        }

        auto test_strides = test_ndarray->strides();
        auto test_shape = test_ndarray->shape();
        byte * data = test_ndarray->cast_data_and_get_pointer<byte>(ctx_cpu);
        std::printf("size(%ld, %ld, %ld)\n", test_shape[0], test_shape[1], test_shape[2]);

        for (int c = 0; c < test_shape[0]; ++c)
        {
            for (int y = 0; y < test_shape[1]; ++y)
            {
                for (int x = 0; x < test_shape[2]; ++x)
                {
                    if (y == x)
                        data[c * test_strides[0] + y * test_strides[1] + x * test_strides[2]] = 0;
                }
            }
        }

        // Matplotlib
        int height = test_shape[0];
        int width = test_shape[1];
        // plt::imshow(data, height, width, 1); // BMPファイルの表示
        // plt::show();

        dummy_filename = TEST_OUTPUT_DIR "Tree_out.bmp";
        io_bmp.save(dummy_filename, test_ndarray, true);
    }


    
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}