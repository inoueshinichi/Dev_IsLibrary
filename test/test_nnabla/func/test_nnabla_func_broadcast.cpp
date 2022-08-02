/**
 * @file test_nnabla_func_broadcast.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla broadcast-func *バグあり
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
    // Broadcast
    TEST(nnabla_func, broadcast_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{1, 5, 5});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape_in[0]; ++c)
        {
            for (int y = 0; y < shape_in[1]; ++y)
            {
                for (int x = 0; x < shape_in[2]; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = c;
                }
            }
        }
        printf("in-shape: (%ld, %ld, %ld)\n", shape_in[0], shape_in[1], shape_in[2]);
        show_ndarray_contents<float>(ndarray_in);

        // broadcasetはshapeで与えたTensorサイズに拡張する.
        auto out_ndarray = broadcast<float>(ndarray_in, {3, 5, 5});
        auto out_shape = out_ndarray->shape();
        printf("out: size:%ld (%ld, %ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1], out_shape[2]);
        show_ndarray_contents<float>(out_ndarray);

    }

    // Broadcast2
    TEST(nnabla_func, broadcast_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = zeros<float>(Shape_t{3, 1, 5});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape_in[0]; ++c)
        {
            for (int y = 0; y < shape_in[1]; ++y)
            {
                for (int x = 0; x < shape_in[2]; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = c;
                }
            }
        }
        printf("in-shape: (%ld, %ld, %ld)\n", shape_in[0], shape_in[1], shape_in[2]);
        show_ndarray_contents<float>(ndarray_in);

        // broadcasetはshapeで与えたTensorサイズに拡張する.
        auto out_ndarray = broadcast<float>(ndarray_in, {3, 5, 5});
        auto out_shape = out_ndarray->shape();
        printf("out: size:%ld (%ld, %ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1], out_shape[2]);
        show_ndarray_contents<float>(out_ndarray);
    }

    // Broadcast3
    TEST(nnabla_func, broadcast_3)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 1});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape_in[0]; ++c)
        {
            for (int y = 0; y < shape_in[1]; ++y)
            {
                for (int x = 0; x < shape_in[2]; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = c;
                }
            }
        }
        printf("in-shape: (%ld, %ld, %ld)\n", shape_in[0], shape_in[1], shape_in[2]);
        show_ndarray_contents<float>(ndarray_in);

        // broadcasetはshapeで与えたTensorサイズに拡張する.
        auto out_ndarray = broadcast<float>(ndarray_in, {3, 5, 5});
        auto out_shape = out_ndarray->shape();
        printf("out: size:%ld (%ld, %ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1], out_shape[2]);
        show_ndarray_contents<float>(out_ndarray);
    }


    // Broadcast4
    TEST(nnabla_func, broadcast_4)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 1});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape_in[0]; ++c)
        {
            for (int y = 0; y < shape_in[1]; ++y)
            {
                for (int x = 0; x < shape_in[2]; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = c;
                }
            }
        }
        printf("in-shape: (%ld, %ld, %ld)\n", shape_in[0], shape_in[1], shape_in[2]);
        show_ndarray_contents<float>(ndarray_in);

        // broadcasetはshapeで与えたTensorサイズに拡張する.
        auto out_ndarray = broadcast<float>(ndarray_in, {2, 3, 5, 5});
        auto out_shape = out_ndarray->shape();
        printf("out: size:%ld (%ld, %ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1], out_shape[2]);
        show_ndarray_contents<float>(out_ndarray);
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}