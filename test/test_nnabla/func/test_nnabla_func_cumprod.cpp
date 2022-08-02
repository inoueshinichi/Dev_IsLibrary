/**
 * @file test_nnabla_func_cumprod.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla cumprod-func
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
    // CumProd by axis 0
    TEST(nnabla_func, cumprod_axis_0)
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

        // cumprod with axis=0
        auto ndarray_out1 = cumprod<float>(ndarray_in, 0);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }


    // CumProd by axis 0 with exclusive
    TEST(nnabla_func, cumprod_axis_0_exclusive)
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

        // cumprod with axis=1
        auto ndarray_out1 = cumprod<float>(ndarray_in, 0, true);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }


    // CumProd by axis 1
    TEST(nnabla_func, cumprod_axis_1)
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

        // cumprod with axis=1
        auto ndarray_out1 = cumprod<float>(ndarray_in, 1);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }


    // CumProd by axis 1 exclusive
    TEST(nnabla_func, cumprod_axis_1_exclusive)
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

        // cumprod with axis=1
        auto ndarray_out1 = cumprod<float>(ndarray_in, 1, true);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }


    // CumProd by axis 2
    TEST(nnabla_func, cumprod_axis_2)
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

        // cumprod with axis=2
        auto ndarray_out1 = cumprod<float>(ndarray_in, 2);
        show_ndarray_property(ndarray_out1);
        show_ndarray_contents<float>(ndarray_out1);
    }


    // CumProd by axis 2 with reverse
    TEST(nnabla_func, cumprod_axis_2_with_reverse)
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

        // cumprod with axis=2 reverse
        auto ndarray_out_vec = cumprod<float>(ndarray_in, 2, false, true);
        show_ndarray_property(ndarray_out_vec);
        show_ndarray_contents<float>(ndarray_out_vec);
    }


    // CumProd by axis 2 with exclusive reverse
    TEST(nnabla_func, cumprod_axis_2_with_exclusive_with_reverse)
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

        // cumprod with axis=2 exclusive reverse
        auto ndarray_out_vec = cumprod<float>(ndarray_in, 2, true, true);
        show_ndarray_property(ndarray_out_vec);
        show_ndarray_contents<float>(ndarray_out_vec);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

