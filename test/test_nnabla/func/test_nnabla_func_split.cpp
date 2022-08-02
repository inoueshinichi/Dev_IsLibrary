/**
 * @file test_nnabla_func_split.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla split-func
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

namespace 
{
    TEST(nnabla_func, split_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 2021;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[0] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_contents<float>(ndarray_in);


        auto outputs = split<float>(ndarray_in, 0);
        for (int i = 0; i < outputs.size(); ++i)
        {
            show_ndarray_contents<float>(outputs.at(i));
            cout << "--------------" << endl;
        }
    }

    TEST(nnabla_func, split_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 2021;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[0] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_contents<float>(ndarray_in);


        auto outputs = split<float>(ndarray_in, 1);
        for (int i = 0; i < outputs.size(); ++i)
        {
            show_ndarray_contents<float>(outputs.at(i));
            cout << "--------------" << endl;
        }
    }

    TEST(nnabla_func, split_3)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        unsigned long seed = 2021;
        std::mt19937_64 e(seed);
        std::uniform_int_distribution dist(0, 9);

        for (int c = 0; c < sh[0]; ++c) {
            for (int y = 0; y < sh[1]; ++y) {
                for (int x = 0; x < sh[2]; ++x) {
                    data[c * st[0] + y * st[1] + x * st[2]] = dist(e);
                }
            }
        }

        show_ndarray_contents<float>(ndarray_in);


        auto outputs = split<float>(ndarray_in, 2);
        for (int i = 0; i < outputs.size(); ++i)
        {
            show_ndarray_contents<float>(outputs.at(i));
            cout << "--------------" << endl;
        }
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

