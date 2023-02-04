// GoogleTest
#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsComputerVision/io/image_io.hpp>

// Test Target
#include <IsComputerVision/filter/utils/padding.hpp>

// Test Utility
#include <test_utils.hpp>

using namespace is::nbla;
using namespace is::common;
using namespace is::cv;

#define PADDING_ZERO 0
#define PADDING_MEAN 1
#define PADDING_REFLECT 2
#define PADDING_REPLICATE 3
#define PADDING_WARP 4

namespace
{
    // Padding
    TEST(cv_func, padding)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::string test_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<BmpFile> io_bmp;
        auto src = io_bmp.load(test_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        show_ndarray_property(src);

        // padding
        int pad_x = 5;
        int pad_y = 5;
        auto pad = padding<ubyte>(src, pad_x, pad_x, PADDING_REPLICATE);
        show_ndarray_property(pad);

        std::string out_filename = format_string(TEST_OUTPUT_DIR "paddingLenna_%dx%d.bmp", pad_x, pad_y);
        io_bmp.save(out_filename, pad, false);
    }
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}