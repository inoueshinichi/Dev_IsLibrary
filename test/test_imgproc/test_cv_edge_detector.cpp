#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;

#include <IsCommon/tm.hpp>

// imgproc
#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/io/img_io.hpp>
#include <IsComputerVision/saturate_clamp.hpp>

// filter
#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/edge_detector/sobel_edge_detector.hpp>
#include <IsComputerVision/filter/edge_detector/laplacian_edge_detector.hpp>
#include <IsComputerVision/filter/edge_detector/log_edge_detector.hpp>


// utility
#include <test_utils.hpp>

#include <string>
#include <cstdio>

using uchar = unsigned char;
using namespace is::nbla;
using namespace is::common;
using namespace is::cv;

namespace
{
    TEST(cv_func, sobel_edge_detector) {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto sh = src->shape();
        auto st = src->strides();
        // show_ndarray_property(src);

        int direction = 0;
        auto filtered = invoke_tm_chrono_ms_ret(sobel_edge_detector, src, direction);
        double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

        // 基準輝度を128にする
        for (int c = 0; c < sh[0]; ++c)
        {
            for (int j = 0; j < sh[1]; ++j)
            {
                for (int i = 0; i < sh[2]; ++i)
                {
                    double v = dst_data[st[0] * c + st[1] * j + st[2] * i];
                    v += 128;
                    dst_data[st[0] * c + st[1] * j + st[2] * i] =
                        saturate_clamp<uchar>(v); // Clamp
                }
            }
        }

        // double -> uchar
        filtered->cast_data_and_get_pointer<uchar>(ctx);

        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "sobel_barbara512_dir%d.bmp",
                                       direction);
        io_bmp.save(dummy_filename, filtered, false);
    }

    TEST(cv_func, laplacian_edge_detector) {
      const auto &ctx =
          SingletonManager::get<GlobalContext>()->get_current_context();
      std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

      std::string dummy_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

      // io
      ImageIo<format::BmpFile> io_bmp;
      auto src = io_bmp.load(dummy_filename, false);

      // src
      auto sh = src->shape();
      auto st = src->strides();
      // show_ndarray_property(src);

      int direction = 0;
      auto filtered = invoke_tm_chrono_ms_ret(laplacian_edge_detector, src, true);
      double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

      // 基準輝度を128にする
      for (int c = 0; c < sh[0]; ++c) {
        for (int j = 0; j < sh[1]; ++j) {
          for (int i = 0; i < sh[2]; ++i) {
            double v = dst_data[st[0] * c + st[1] * j + st[2] * i];
            v += 128;
            dst_data[st[0] * c + st[1] * j + st[2] * i] =
                saturate_clamp<uchar>(v);  // Clamp
          }
        }
      }

      // double -> uchar
      filtered->cast_data_and_get_pointer<uchar>(ctx);

      // show_ndarray_property(filtered);

      dummy_filename = format_string(TEST_OUTPUT_DIR "laplacian_barbara512_dir%d.bmp", direction);
      io_bmp.save(dummy_filename, filtered, false);
    }

    TEST(cv_func, log_edge_detector) {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        std::string dummy_filename = TEST_INPUT_MONO_512_DIR "barbara512.bmp";

        // io
        ImageIo<format::BmpFile> io_bmp;
        auto src = io_bmp.load(dummy_filename, false);

        // src
        auto sh = src->shape();
        auto st = src->strides();
        // show_ndarray_property(src);

        auto filtered = invoke_tm_chrono_ms_ret(log_edge_detector, src, 0);
        double *dst_data = filtered->cast_data_and_get_pointer<double>(ctx);

        // 基準輝度を128にする
        for (int c = 0; c < sh[0]; ++c) {
            for (int j = 0; j < sh[1]; ++j) {
            for (int i = 0; i < sh[2]; ++i) {
                double v = dst_data[st[0] * c + st[1] * j + st[2] * i];
                v += 128;
                dst_data[st[0] * c + st[1] * j + st[2] * i] =
                    saturate_clamp<uchar>(v);  // Clamp
            }
            }
        }

        // double -> uchar
        filtered->cast_data_and_get_pointer<uchar>(ctx);

        // show_ndarray_property(filtered);

        dummy_filename = format_string(TEST_OUTPUT_DIR "log_barbara512.bmp");
        io_bmp.save(dummy_filename, filtered, false);
    }
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}