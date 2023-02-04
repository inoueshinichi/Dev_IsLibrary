// GoogleTest
#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsCommon/internal/time.hpp>

#include <IsComputerVision/common.hpp>
#include <IsComputerVision/io/image_io.hpp>

// Test Target
#include <IsComputerVision/filter/edge_detector/sobel_edge_detector.hpp>
#include <IsComputerVision/filter/edge_detector/laplacian_edge_detector.hpp>
#include <IsComputerVision/filter/edge_detector/log_edge_detector.hpp>

// Test Utility
#include <test_utils.hpp>

using namespace is::common;
using namespace is::nbla;
using namespace is::cv;

namespace
{
    TEST(cv_func, sobel_edge_detector)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string test_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

        // io
        ImageIo<BmpFile> io_bmp;
        auto src = io_bmp.load(test_filename, false);

        // src
        auto sh = src->shape();
        auto st = src->strides();
        int sh_H = sh.at(0);
        int sh_W = sh.at(1);
        int sh_C = sh.at(2);
        int st_H = st.at(0);
        int st_W = st.at(1);
        int st_C = st.at(2);
        // show_ndarray_property(src);

        int direction = 0;
        auto filtered = invoke_tm_chrono_ms_ret(sobel_edge_detector, src, direction);
        double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

        // 基準輝度を128にする
        for (int j = 0; j < sh_H; ++j)
        {
            for (int i = 0; i < sh_W; ++i)
            {
                for (int c = 0; c < sh_C; ++c)
                {
                    double v = dst_data[j * st_H + i * st_W + c * st_C];
                    v += 128;
                    dst_data[j * st_H + i * st_W + c * st_C] =
                        saturate_clamp<ubyte>(v); // Clamp
                }
            }
        }

        // double -> uchar
        filtered->cast_data_and_get_pointer<ubyte>(ctx);

        // show_ndarray_property(filtered);

        std::string out_filename = format_string(TEST_OUTPUT_DIR "sobel_barbara512_dir%d.bmp",
                                                 direction);
        io_bmp.save(out_filename, filtered, false);
    }

    TEST(cv_func, laplacian_edge_detector)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string test_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

        // io
        ImageIo<BmpFile> io_bmp;
        auto src = io_bmp.load(test_filename, false);

        // src
        auto sh = src->shape();
        auto st = src->strides();
        int sh_H = sh.at(0);
        int sh_W = sh.at(1);
        int sh_C = sh.at(2);
        int st_H = st.at(0);
        int st_W = st.at(1);
        int st_C = st.at(2);
        // show_ndarray_property(src);

        int direction = 0;
        auto filtered = invoke_tm_chrono_ms_ret(laplacian_edge_detector, src, true);
        double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

        // 基準輝度を128にする
        for (int j = 0; j < sh_H; ++j)
        {
            for (int i = 0; i < sh_W; ++i)
            {
                for (int c = 0; c < sh_C; ++c)
                {
                    double v = dst_data[j * st_H + i * st_W + c * st_C];
                    v += 128;
                    dst_data[j * st_H + i * st_W + c * st_C] =
                        saturate_clamp<ubyte>(v); // Clamp
                }
            }
        }

        // double -> uchar
        filtered->cast_data_and_get_pointer<ubyte>(ctx);

        // show_ndarray_property(filtered);

        std::string out_filename = format_string(TEST_OUTPUT_DIR "laplacian_barbara512_dir%d.bmp", direction);
        io_bmp.save(out_filename, filtered, false);
    }

    TEST(cv_func, log_edge_detector)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string test_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

        // io
        ImageIo<BmpFile> io_bmp;
        auto src = io_bmp.load(test_filename, false);

        // src
        auto sh = src->shape();
        auto st = src->strides();
        int sh_H = sh.at(0);
        int sh_W = sh.at(1);
        int sh_C = sh.at(2);
        int st_H = st.at(0);
        int st_W = st.at(1);
        int st_C = st.at(2);
        // show_ndarray_property(src);

        auto filtered = invoke_tm_chrono_ms_ret(log_edge_detector, src, 0);
        double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

        // 基準輝度を128にする
        for (int j = 0; j < sh_H; ++j)
        {
            for (int i = 0; i < sh_W; ++i)
            {
                for (int c = 0; c < sh_C; ++c)
                {
                    double v = dst_data[j * st_H + i * st_W + c * st_C];
                    v += 128;
                    dst_data[j * st_H + i * st_W + c * st_C] =
                        saturate_clamp<ubyte>(v); // Clamp
                }
            }
        }

        // double -> uchar
        filtered->cast_data_and_get_pointer<ubyte>(ctx);

        // show_ndarray_property(filtered);

        std::string out_filename = format_string(TEST_OUTPUT_DIR "log_barbara512.bmp");
        io_bmp.save(out_filename, filtered, false);
    }
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}