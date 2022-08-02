/**
 * @file test_nnabla_func_log10.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla log10-func
 * @version 0.1
 * @date 2022-01-05
 * 
 * @copyright Copyright (c) 2022
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
    TEST(nnabla_func, log10_1)
    {
        using namespace std;
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 3});
        auto st = ndarray_zeros->strides();
        auto sh = ndarray_zeros->shape();
        float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);

        data[0 * st[0] + 0 * st[1]] = 1.0;
        data[1 * st[0] + 1 * st[1]] = -1.5;
        data[2 * st[0] + 2 * st[1]] = 0.0;

        show_ndarray_contents<float>(ndarray_zeros);

        cout << "---" <<endl;

        auto out_ndarray = is::common::invoke_tm_chrono_ms_ret(log10<float>, ndarray_zeros);

        show_ndarray_contents<float>(out_ndarray);
    }

    TEST(nnabla_func, log_2)
    {
        using namespace std;
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 3});
        auto st = ndarray_zeros->strides();
        auto sh = ndarray_zeros->shape();
        float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);

        data[0 * st[0] + 0 * st[1]] = 0;
        data[0 * st[0] + 1 * st[1]] = 1;
        data[0 * st[0] + 2 * st[1]] = 10;
        data[1 * st[0] + 0 * st[1]] = 100;
        data[1 * st[0] + 1 * st[1]] = 1000;
        data[1 * st[0] + 2 * st[1]] = 10000;
        data[2 * st[0] + 0 * st[1]] = 100000;
        data[2 * st[0] + 1 * st[1]] = 1000000;
        data[2 * st[0] + 2 * st[1]] = 10000000;

        show_ndarray_contents<float>(ndarray_zeros);

        cout << "---" <<endl;

        for (int y = 0; y < sh[0]; ++y)
        {
            for (int x = 0; x < sh[1]; ++x)
            {
                auto& v = data[y * st[0] + x * st[1]];
                v = log10(v);
            }
        }

        show_ndarray_contents<float>(ndarray_zeros);
    }

    TEST(nnabla_func, log10_3)
    {
        using namespace std;
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 3});
        auto st = ndarray_zeros->strides();
        auto sh = ndarray_zeros->shape();
        float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);

        data[0 * st[0] + 0 * st[1]] = 0;
        data[0 * st[0] + 1 * st[1]] = 5;
        data[0 * st[0] + 2 * st[1]] = 15;
        data[1 * st[0] + 0 * st[1]] = 103;
        data[1 * st[0] + 1 * st[1]] = 1024;
        data[1 * st[0] + 2 * st[1]] = 10563;
        data[2 * st[0] + 0 * st[1]] = 100234;
        data[2 * st[0] + 1 * st[1]] = 1000111;
        data[2 * st[0] + 2 * st[1]] = 10000222;

        show_ndarray_contents<float>(ndarray_zeros);

        cout << "---" <<endl;

        for (int y = 0; y < sh[0]; ++y)
        {
            for (int x = 0; x < sh[1]; ++x)
            {
                auto& v = data[y * st[0] + x * st[1]];
                if (v == 0) v = 1;
                else {
                    if (v < 0) v *= -1;
                    v = (int)log10(v) + 1; // 小数点切り捨て ・・・数値の桁数を簡単に計算できるかも
                }
            }
        }

        show_ndarray_contents<float>(ndarray_zeros);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}