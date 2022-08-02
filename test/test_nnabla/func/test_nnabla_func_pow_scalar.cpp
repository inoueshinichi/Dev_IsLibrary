/**
 * @file test_nnabla_func_pow_scalar.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla pow_scalar-func
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
    TEST(nnabla_func, pow_scalar_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
        auto ndarray_adds = add_scalar<float>(ndarray_zeros, 2);
        show_ndarray_contents<float>(ndarray_adds);

        auto out_ndarray = pow_scalar<float>(ndarray_adds, 2);

        show_ndarray_contents<float>(out_ndarray);
    }

    TEST(nnabla_func, pow_scalar_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
        auto ndarray_adds = add_scalar<float>(ndarray_zeros, 2);
        show_ndarray_contents<float>(ndarray_adds);

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
                    v = pow(v, 2);
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