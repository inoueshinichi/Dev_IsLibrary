/**
 * @file test_nnabla_func_mean.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla mean-func
 * @version 0.1
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// googletest
#include <gtest/gtest.h>

// nnabla
#include <test_nnabla.hpp>

// test utils
#include <test_utils.hpp>

namespace 
{
    // Mean by axis 0
    TEST(nnabla_func, mean_axis_0)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 5});
        show_ndarray_contents<float>(ndarray_in);

        // sum with axis=0
        auto ndarray_out1 = mean<float>(ndarray_in, 0);
        auto shape_out1 = ndarray_out1->shape();
        printf("meaned: size:%ld (%ld, %ld)\n", shape_out1.size(), shape_out1[0], shape_out1[1]);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Mean by axis 1
    TEST(nnabla_func, mean_axis_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 5});
        show_ndarray_contents<float>(ndarray_in);

        // mean with axis=1
        auto ndarray_out2 = mean<float>(ndarray_in, 1);
        auto shape_out2 = ndarray_out2->shape();
        printf("meaned: size:%ld (%ld, %ld)\n", shape_out2.size(), shape_out2[0], shape_out2[1]);
        show_ndarray_contents<float>(ndarray_out2);
    }

    // Mean by axis 2
    TEST(nnabla_func, mean_axis_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 5});
        show_ndarray_contents<float>(ndarray_in);

        // mean with axis=2
        auto ndarray_out3 = mean<float>(ndarray_in, 2);
        auto shape_out3 = ndarray_out3->shape();
        printf("meaned: size:%ld (%ld, %ld)\n", shape_out3.size(), shape_out3[0], shape_out3[1]);
        show_ndarray_contents<float>(ndarray_out3);
    }

    // Mean by axis 2 with keep_dims
    TEST(nnabla_func, mean_axis_2_keep_dims)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 5});
        show_ndarray_contents<float>(ndarray_in);

        // mean with axis=2 and keep_dims = true
        auto ndarray_out4 = mean<float>(ndarray_in, 2, true);
        auto shape_out4 = ndarray_out4->shape();
        std::printf("meaned: size:%ld (%ld, %ld, %ld)\n", shape_out4.size(), shape_out4[0], shape_out4[1], shape_out4[2]);
        show_ndarray_contents<float>(ndarray_out4);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}