// GoogleTest
#include <gtest/gtest.h>

// #include "matplotlibcpp.h"
// namespace plt = matplotlibcpp;


// Test Target
#include <IsComputerVision/common.hpp>

// Test Utility
#include <test_utils.hpp>

using namespace is::common;
using namespace is::nbla;
using namespace is::cv;

namespace
{
    // SaturateClamp
    TEST(cv_func, saturate_clamp_float2ubyte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray = zeros<float>(Shape_t{2, 3, 3});
        show_ndarray_property(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        float *data = ndarray->cast_data_and_get_pointer<float>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 255;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 256;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 257;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 128;
        show_ndarray_contents<float>(ndarray);

        std::cout << "saturate clamp: float -> ubyte" << std::endl;
        ndarray = saturate_clamp<ubyte>(ndarray);
        show_ndarray_contents<ubyte>(ndarray);
    }

    // NdArrayCast
    TEST(cv_func, ndarray_cast_float2ubyte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray = zeros<float>(Shape_t{2, 3, 3});
        show_ndarray_property(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        float *data = ndarray->cast_data_and_get_pointer<float>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 255;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 256;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 257;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 128;
        show_ndarray_contents<float>(ndarray);

        std::cout << "cast_data_and_get_pointer: float -> ubyte" << std::endl;
        ndarray->cast(dtypes::UBYTE, ctx);
        show_ndarray_contents<ubyte>(ndarray);
    }

    // SaturateClamp
    TEST(cv_func, saturate_clamp_byte2ubyte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray = zeros<byte>(Shape_t{2, 3, 3});
        show_ndarray_property(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        byte *data = ndarray->cast_data_and_get_pointer<byte>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 128;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = -128;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = -127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 1;
        show_ndarray_contents<byte>(ndarray);

        std::cout << "saturate clamp: byte -> ubyte" << std::endl;
        ndarray = saturate_clamp<ubyte>(ndarray);
        show_ndarray_contents<ubyte>(ndarray);
    }

    // NdArrayCast
    TEST(cv_func, ndarray_cast_byte2ubyte)
    {
        const auto &ctx =
            SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray = zeros<byte>(Shape_t{2, 3, 3});
        show_ndarray_property(ndarray);
        auto sh = ndarray->shape();
        auto st = ndarray->strides();

        byte *data = ndarray->cast_data_and_get_pointer<byte>(ctx);
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = -1;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 127;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 128;
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = -128;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = -127;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 1;
        show_ndarray_contents<byte>(ndarray);

        std::cout << "cast_data_and_get_pointer: byte -> ubyte" << std::endl;
        ndarray->cast(dtypes::UBYTE, ctx);
        show_ndarray_contents<ubyte>(ndarray);
    }

    // Test Native Cast
    TEST(cv_func, native_cast)
    {
        byte b1 = -1;
        byte b2 = 127;
        byte b3 = 128;
        byte b4 = -128;
        byte b5 = -127;
        byte b6 = 1;

        std::printf("BYTE's content: [0, 1, ..., 127, -128, -127, ..., -1\n");

        std::cout << "b1: " << +b1 << std::endl;
        std::cout << "b2: " << +b2 << std::endl;
        std::cout << "b3: " << +b3 << std::endl;
        std::cout << "b4: " << +b4 << std::endl;
        std::cout << "b5: " << +b5 << std::endl;
        std::cout << "b6: " << +b6 << std::endl;

        std::cout << "ub1: " << +(ubyte)b1 << std::endl;
        std::cout << "ub2: " << +(ubyte)b2 << std::endl;
        std::cout << "ub3: " << +(ubyte)b3 << std::endl;
        std::cout << "ub4: " << +(ubyte)b4 << std::endl;
        std::cout << "ub5: " << +(ubyte)b5 << std::endl;
        std::cout << "ub6: " << +(ubyte)b6 << std::endl;
    }
}


int main(int, char**)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}