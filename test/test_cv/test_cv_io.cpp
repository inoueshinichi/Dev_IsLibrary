
// GoogleTest
#include <gtest/gtest.h>

// Test Target
#include <IsComputerVision/io/image_io.hpp>

// Test Utility
#include <test_utils.hpp>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

using namespace is::cv;
using namespace is::nbla;
using namespace is::common;

namespace
{
    TEST(cv_io, bmp)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        std::string test_filename = TEST_INPUT_MONO_256_DIR "Tree.bmp";

        // Bmp
        ImageIo<BmpFile> io_bmp;
        auto ndarray = io_bmp.load(test_filename, true);
        
        auto st = ndarray->strides();
        auto sh = ndarray->shape();
        int st_H = st.at(0);
        int st_W = st.at(1);
        int st_C = st.at(2);
        int sh_H = sh.at(0);
        int sh_W = sh.at(1);
        int sh_C = sh.at(2);
        ubyte * data = ndarray->cast_data_and_get_pointer<ubyte>(ctx_cpu);
        std::printf("Image size(%d, %d, %d)\n", sh_H, sh_W, sh_C);

        for (int y = 0; y < sh_H; y++)
        {
            for (int x = 0; x < sh_W; x++)
            {
                for (int c = 0; c < sh_C; c++)
                {
                    if (y == x)
                        data[y * st_H + x * st_W + c * st_C] = 0;
                }
            }
        }

        // Matplotlib
        // plt::imshow(data, sh_H, sh_W, 1); // BMPファイルの表示
        // plt::show();

        std::string out_filename = TEST_OUTPUT_DIR "Tree_out.bmp";
        io_bmp.save(out_filename, ndarray, true);
    }

}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}