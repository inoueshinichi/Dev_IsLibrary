/**
 * @file test_nnabla_func_sort.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla sort-func
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
    // Sort by axis 0
    TEST(nnabla_func, sort_axis_0)
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
        auto ndarray_out1 = sort<float>(ndarray_in, 0);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

    // Sort by axis 1
    TEST(nnabla_func, sort_axis_1)
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
        auto ndarray_out1 = sort<float>(ndarray_in, 1);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

    // Sort by axis 2
    TEST(nnabla_func, sort_axis_2)
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
        auto ndarray_out1 = sort<float>(ndarray_in, 2);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

     // Sort by axis 2, only index
    TEST(nnabla_func, sort_axis_2_only_index)
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

        // sort with axis=2 only_index
        auto ndarray_out1 = sort<float>(ndarray_in, 2, false, false, true);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<ulong>(ndarray_out1[0]);
    }

    // Sort by axis 2 with reverse
    TEST(nnabla_func, sort_axis_2_with_reverse)
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
        auto ndarray_out1 = sort<float>(ndarray_in, 2, true);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

     // Sort by axis 2 with reverse & with_index
    TEST(nnabla_func, sort_axis_2_with_reverse_with_index)
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
        auto ndarray_out_vec = sort<float>(ndarray_in, 2, true, true);
        show_ndarray_property(ndarray_out_vec[0]);
        show_ndarray_contents<float>(ndarray_out_vec[0]);

        // index
        show_ndarray_property(ndarray_out_vec[1]);
        show_ndarray_contents<ulong>(ndarray_out_vec[1]);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

