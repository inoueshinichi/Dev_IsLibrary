/**
 * @file test_nnabla_func_max.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla max-func
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
    // Max by axis 0
    TEST(nnabla_func, max_axis_0)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;
        
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=0
        auto ndarray_out1 = max<float>(ndarray_in, 0);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

    // Max by axis 1
    TEST(nnabla_func, max_axis_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;
        
        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=1
        auto ndarray_out1 = max<float>(ndarray_in, 1);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

    // Max by axis 2
    TEST(nnabla_func, max_axis_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=2
        auto ndarray_out1 = max<float>(ndarray_in, 2);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

     // Max by axis 2, only index
    TEST(nnabla_func, max_axis_2_only_index)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;

        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=2 only_index
        auto ndarray_out1 = max<float>(ndarray_in, 2, false, false, true);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<ulong>(ndarray_out1[0]);
    }

    // Max by axis 2 with keep_dims
    TEST(nnabla_func, max_axis_2_with_keep_dims)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;
        
        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=2
        auto ndarray_out1 = max<float>(ndarray_in, 2, true);
        auto shape_out1 = ndarray_out1[0]->shape();
        show_ndarray_property(ndarray_out1[0]);
        show_ndarray_contents<float>(ndarray_out1[0]);
    }

     // Max by axis 2 with keep_dims & with_index
    TEST(nnabla_func, max_axis_2_with_keep_dims_with_index)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 3, 3});
        auto st = ndarray_in->strides();
        auto sh = ndarray_in->shape();
        float* data = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);

        // ch:0
        data[0 * st[0] + 0 * st[1] + 0 * st[2]] = 1.0;
        data[0 * st[0] + 0 * st[1] + 1 * st[2]] = 2.0;
        data[0 * st[0] + 0 * st[1] + 2 * st[2]] = 3.0;
        data[0 * st[0] + 1 * st[1] + 0 * st[2]] = 4.0;
        data[0 * st[0] + 1 * st[1] + 1 * st[2]] = 5.0;
        data[0 * st[0] + 1 * st[1] + 2 * st[2]] = 6.0;
        data[0 * st[0] + 2 * st[1] + 0 * st[2]] = 7.0;
        data[0 * st[0] + 2 * st[1] + 1 * st[2]] = 8.0;
        data[0 * st[0] + 2 * st[1] + 2 * st[2]] = 9.0;

        // ch:1
        data[1 * st[0] + 0 * st[1] + 0 * st[2]] = 9.0;
        data[1 * st[0] + 0 * st[1] + 1 * st[2]] = 1.0;
        data[1 * st[0] + 0 * st[1] + 2 * st[2]] = 2.0;
        data[1 * st[0] + 1 * st[1] + 0 * st[2]] = 3.0;
        data[1 * st[0] + 1 * st[1] + 1 * st[2]] = 4.0;
        data[1 * st[0] + 1 * st[1] + 2 * st[2]] = 5.0;
        data[1 * st[0] + 2 * st[1] + 0 * st[2]] = 6.0;
        data[1 * st[0] + 2 * st[1] + 1 * st[2]] = 7.0;
        data[1 * st[0] + 2 * st[1] + 2 * st[2]] = 8.0;

        // ch:2
        data[2 * st[0] + 0 * st[1] + 0 * st[2]] = 8.0;
        data[2 * st[0] + 0 * st[1] + 1 * st[2]] = 9.0;
        data[2 * st[0] + 0 * st[1] + 2 * st[2]] = 1.0;
        data[2 * st[0] + 1 * st[1] + 0 * st[2]] = 2.0;
        data[2 * st[0] + 1 * st[1] + 1 * st[2]] = 3.0;
        data[2 * st[0] + 1 * st[1] + 2 * st[2]] = 4.0;
        data[2 * st[0] + 2 * st[1] + 0 * st[2]] = 5.0;
        data[2 * st[0] + 2 * st[1] + 1 * st[2]] = 6.0;
        data[2 * st[0] + 2 * st[1] + 2 * st[2]] = 7.0;
        
        show_ndarray_property(ndarray_in);
        show_ndarray_contents<float>(ndarray_in);

        // max with axis=2
        auto ndarray_out_vec = max<float>(ndarray_in, 2, true, true);
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

