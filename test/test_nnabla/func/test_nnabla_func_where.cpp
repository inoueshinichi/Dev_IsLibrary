/**
 * @file test_nnabla_func_where.cpp
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
    TEST(nnabla_func, where_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = NdArray::create(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        bool* data1 = in_narr1->cast_data_and_get_pointer<bool>(ctx_cpu);
        cout << "--- in_arr1" << endl;
        // show_ndarray_contents<bool>(in_narr1);

        data1[0] = true; data1[4] = true; data1[8] = true;
        data1[9] = true; data1[13] = true; data1[17] = true;
        data1[18] = true; data1[22] = true; data1[26] = true;
        show_ndarray_contents<bool>(in_narr1);


        auto in_narr2 = ones<float>(Shape_t{3, 3, 3});
        in_narr2 = mul_scalar<float>(in_narr2, 5);
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto in_narr3 = ones<float>(Shape_t{3, 3, 3});
        in_narr3 = mul_scalar<float>(in_narr3, 9);
        cout << "--- in_arr3" << endl;
        show_ndarray_contents<float>(in_narr3);


        auto output = where<float>(in_narr1, in_narr2, in_narr3);
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
