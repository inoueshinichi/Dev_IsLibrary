/**
 * @file test_nnabla_func_greater_scalar.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-03
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
    TEST(nnabla_func, greater_scalar_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        float* data1 = in_narr1->cast_data_and_get_pointer<float>(ctx_cpu);

        data1[0] = 0; data1[1] = 0; data1[2] = 1;
        data1[3] = 1; data1[4] = 5; data1[5] = 0;
        data1[6] = std::numeric_limits<float>::quiet_NaN(); data1[7] = std::numeric_limits<float>::quiet_NaN(); data1[8] = std::numeric_limits<float>::infinity(); 
 
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        // auto in_narr2 = ones<float>(Shape_t{3, 3});
        // auto st2 = in_narr2->strides();
        // auto sh2 = in_narr2->shape();
        // float* data2 = in_narr2->cast_data_and_get_pointer<float>(ctx_cpu);

        // data2[0] = 0; data2[1] = 1; data2[2] = 0;
        // data2[3] = 1; data2[4] = 5; data2[5] = std::numeric_limits<float>::infinity();
        // data2[6] = 0; data2[7] = std::numeric_limits<float>::quiet_NaN(); data2[8] = std::numeric_limits<float>::infinity(); 

        // cout << "--- in_arr2" << endl;
        // show_ndarray_contents<float>(in_narr2);

        auto output = is::nbla::greater_scalar<float>(in_narr1, 4);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}