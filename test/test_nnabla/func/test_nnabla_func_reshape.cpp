/**
 * @file test_nnabla_func_reshape.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla reshape-func
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
    // Reshape
    TEST(nnabla_func, reshape)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // ones
        auto ndarray_ones = ones<float>(Shape_t{3, 5, 5});
        float *data = ndarray_ones->cast_data_and_get_pointer<float>(ctx_cpu);
        auto shape = ndarray_ones->shape();
        auto strides = ndarray_ones->strides();
        data[0] = 0.0;
        data[1 * strides[0] + 1 * strides[1] + 1 * strides[2]] = 0.0;
        data[2 * strides[0] + 2 * strides[1] + 2 * strides[2]] = 0.0;
        show_ndarray_contents<float>(ndarray_ones);

        auto out_ndarray = reshape<float>(ndarray_ones, Shape_t{3, 25});
        auto out_shape = out_ndarray->shape();
        show_ndarray_contents<float>(out_ndarray);
        printf("out: size:%ld (%ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1]);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}