#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsCommon/tm.hpp>

// nbla
#include <IsNdArray/nd_array_extra.hpp>

// imgproc
#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/io/img_io.hpp>

// utility
#include <test_utils.hpp>

#include <string>
#include <cstdio>

using uchar = unsigned char;
using namespace is::nbla;
using namespace is::common;
using namespace is::imgproc;

#define PADDING_ZERO 0
#define PADDING_MEAN 1
#define PADDING_REFLECT 2
#define PADDING_REPLICATE 3
#define PADDING_WARP 4

namespace
{
    // Padding
    TEST(imgproc_func, padding)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::string dummy_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        show_ndarray_property(src);

        // padding
        int pad_x = 5;
        int pad_y = 5;
        auto pad = padding<uchar>(src, pad_x, pad_x, PADDING_REPLICATE);
        show_ndarray_property(pad);

        dummy_filename = format_string(TEST_OUTPUT_DIR "paddingLenna_%dx%d.bmp",
                                       pad_x, pad_y);
        io_bmp.save(dummy_filename, pad, false);
    }
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}