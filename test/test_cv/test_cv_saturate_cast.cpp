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
using namespace is::cv;

namespace
{
    // SaturateClamp
    TEST(cv_func, saturate_clamp_float2byte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        using ubyte = unsigned char;
        auto ndarray = zeros<float>(Shape_t{3, 3, 3});
        show_ndarray_property(ndarray);
        show_ndarray_contents<float>(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        float *data = ndarray->cast_data_and_get_pointer<float>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 255;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 256;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 257;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 128;

        std::cout << "non zero" << std::endl;

        show_ndarray_contents<float>(ndarray);

        std::cout << "saturate clamp: float -> byte" << std::endl;
        ndarray = saturate_clamp<char>(ndarray);
        show_ndarray_contents<float>(ndarray);
    }

    // NdArrayCast
    TEST(cv_func, ndarray_cast_float2byte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        using ubyte = unsigned char;
        auto ndarray = zeros<float>(Shape_t{3, 3, 3});
        show_ndarray_property(ndarray);
        show_ndarray_contents<float>(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        float *data = ndarray->cast_data_and_get_pointer<float>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 255;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 256;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 257;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 128;

        std::cout << "non zero" << std::endl;

        show_ndarray_contents<float>(ndarray);

        std::cout << "cast_data_and_get_pointer: float -> byte" << std::endl;
        ndarray->cast(dtypes::BYTE, ctx);
        show_ndarray_contents<float>(ndarray);
    }
}


int main(int, char**)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}