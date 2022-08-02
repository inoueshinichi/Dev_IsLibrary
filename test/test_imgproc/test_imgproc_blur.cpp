#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsCommon/tm.hpp>

// nbla
#include <IsNdArray/nd_array_extra.hpp>

// imgproc
#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/io/img_io.hpp>

// filter/blur
#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/average_blur.hpp>
#include <IsComputerVision/filter/blur/gaussian_blur.hpp>
#include <IsComputerVision/filter/blur/median_blur.hpp>
#include <IsComputerVision/filter/blur/mozic_blur.hpp>

// utility
#include <test_utils.hpp>

#include <string>
#include <cstdio>

using uchar = unsigned char;
using namespace is::nbla;
using namespace is::common;
using namespace is::imgproc;



namespace 
{
    

    // AverageBlur
    TEST(imgproc_func, average_blur)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        // show_ndarray_property(src);

        int ksize_x = 15;
        int ksize_y = 15;
        auto filtered = invoke_tm_chrono_ms_ret(average_blur, src, Size(ksize_x, ksize_y));
        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "average_Lenna_%dx%d.bmp",
                                       ksize_x, ksize_y);
        io_bmp.save(dummy_filename, filtered, false);
    }

    // GaussianBlur
    TEST(imgproc_func, gaussian_blur)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        // show_ndarray_property(src);

        int ksize = 9;
        auto filtered = invoke_tm_chrono_ms_ret(gaussian_blur, src, ksize, 0);
        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "gaussian_Lenna_size%dx%d.bmp",
                                        ksize, ksize);
        io_bmp.save(dummy_filename, filtered, false);
    }

    // MedianBlur
    TEST(imgproc_func, median_blur)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        // show_ndarray_property(src);

        int ksize = 9;
        auto filtered = invoke_tm_chrono_ms_ret(median_blur, src, ksize);
        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "median_Lenna_%dx%d.bmp", 
                                        ksize, ksize);
        io_bmp.save(dummy_filename, filtered, false);
    }

    // MozicBlur
    TEST(imgproc_func, mozic_blur)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_COLOR_DIR "Lenna.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto in_shape = src->shape();
        auto in_strides = src->strides();
        // show_ndarray_property(src);

        int block = 50;
        auto filtered = invoke_tm_chrono_ms_ret(mozic_blur, src, block);
        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "mozic_Lenna_%dx%d.bmp", 
                                        block, block);
        io_bmp.save(dummy_filename, filtered, false);
    }
}       

int main(int, char**)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}