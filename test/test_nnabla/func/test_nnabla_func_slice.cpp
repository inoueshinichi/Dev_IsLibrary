/**
 * @file test_nnabla_func_slice.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla slice-func
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
    // Slice
    TEST(nnabla_func, slice)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray_in = ones<float>(Shape_t{3, 16, 16});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        printf("in: size:%ld (%ld, %ld, %ld)\n", shape_in.size(), shape_in[0], shape_in[1], shape_in[2]);
        for (int c = 0; c < shape_in[0]; ++c)
        {
            for (int y = 5; y < 10; ++y)
            {
                for (int x = 5; x < 10; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = c * 1.1;
                }
            }
        }
        show_ndarray_contents<float>(ndarray_in);

        auto ndarray_out = slice<float>(ndarray_in, {0, 4, 4}, {2, 11, 11}, {1, 1, 1});
        auto shape_out = ndarray_out->shape();
        auto strides_out = ndarray_out->strides();
        float *data_out = ndarray_out->cast_data_and_get_pointer<float>(ctx_cpu);
        printf("slice: size:%ld (%ld, %ld, %ld)\n", shape_out.size(), shape_out[0], shape_out[1], shape_out[2]);
        show_ndarray_contents<float>(ndarray_out);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}