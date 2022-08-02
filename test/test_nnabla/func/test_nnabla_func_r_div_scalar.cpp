/**
 * @file test_nnabla_func_r_div_scalar.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla r_div_scalar-func
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
    TEST(nnabla_func, r_div_scalar_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
        auto ndarray_adds = add_scalar<float>(ndarray_zeros, 3);
        show_ndarray_contents<float>(ndarray_adds);

        cout << "-----" << endl;

        auto out_ndarray = r_div_scalar<float>(ndarray_adds, 1);

        show_ndarray_contents<float>(out_ndarray);
    }

    TEST(nnabla_func, r_div_scalar_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
        auto ndarray_adds = add_scalar<float>(ndarray_zeros, 3);
        show_ndarray_contents<float>(ndarray_adds);

        cout << "-----" << endl;

        auto shape = ndarray_zeros->shape();
        auto strides = ndarray_zeros->strides();
        float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape[0]; ++c)
        {
            for (int y = 0; y < shape[1]; ++y)
            {
                for (int x = 0; x < shape[2]; ++x)
                {
                    auto& v = data[c * strides[0] + y * strides[1] + x * strides[2]];
                    v = 1 / v;
                }
            }
        }

        show_ndarray_contents<float>(ndarray_zeros);
    }


    TEST(nnabla_func, r_div_scalar_error)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
        auto ndarray_adds = add_scalar<float>(ndarray_zeros, 0);
        show_ndarray_contents<float>(ndarray_adds);

        cout << "-----" << endl;

        auto out_ndarray = r_div_scalar<float>(ndarray_adds, 1);

        show_ndarray_contents<float>(out_ndarray);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}