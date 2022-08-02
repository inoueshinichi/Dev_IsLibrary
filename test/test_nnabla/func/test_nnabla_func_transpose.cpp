/**
 * @file test_nnabla_func_transpose.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla transpose-func
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
    // Transpose
    TEST(nnabla_func, transpose)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // ones
        auto ndarray_ones = ones<float>(Shape_t{3, 5, 5});
        auto shape = ndarray_ones->shape();
        auto strides = ndarray_ones->strides();
        float *data = ndarray_ones->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape[0]; ++c)
        {
            for (int y = 0; y < shape[1]; ++y)
            {
                for (int x = 0; x < shape[2]; ++x)
                {
                    data[c * strides[0] + y * strides[1] + x * strides[2]] = c;
                }
            }
        }
        printf("in-shape: (%ld, %ld, %ld)\n", shape[0], shape[1], shape[2]);
        show_ndarray_contents<float>(ndarray_ones);

        auto out_ndarray = transpose<float>(ndarray_ones, {1, 2, 0});
        auto out_shape = out_ndarray->shape();
        show_ndarray_contents<float>(out_ndarray);

        cout << "transpose" << endl;
        printf("out-shape: (%ld, %ld, %ld)\n", out_shape[0], out_shape[1], out_shape[2]);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}