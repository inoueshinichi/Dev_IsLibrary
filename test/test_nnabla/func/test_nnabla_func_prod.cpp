/**
 * @file test_nnabla_func_prod.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla proc-func
 * @version 0.1
 * @date 2021-12-17
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

#include <random>

namespace 
{
    // Prod by axis 0
    TEST(nnabla_func, prod_axis_0)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_contents<float>(ndarray_in);

        // sort with axis=0
        Shape_t axes = {0};
        auto ndarray_out1 = prod<float>(ndarray_in, axes);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Prod by axis 1
    TEST(nnabla_func, prod_axis_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }
        
        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=1
        Shape_t axes = {1};
        auto ndarray_out1 = prod<float>(ndarray_in, axes);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Prot by axis 2
    TEST(nnabla_func, prod_axis_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // sort with axis=2
        Shape_t axes = {2};
        auto ndarray_out1 = prod<float>(ndarray_in, axes);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Prot by axis 2 with keep_dims
    TEST(nnabla_func, prod_axis_2_with_keep_dims)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // sort with axis=2
        Shape_t axes = {2};
        auto ndarray_out1 = prod<float>(ndarray_in, axes, true);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Prot by axis 1 -> 2
    TEST(nnabla_func, prod_axis_1_to_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // sort with axis 1 -> 2
        Shape_t axes = {1, 2};
        auto ndarray_out1 = prod<float>(ndarray_in, axes);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }

    // Prot by axis 1 -> 2 with keep_dims
    TEST(nnabla_func, prod_axis_1_to_2_with_keep_dims)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 1990;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[c] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // sort with axis 1 -> 2
        Shape_t axes = {1, 2};
        auto ndarray_out1 = prod<float>(ndarray_in, axes, true);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

