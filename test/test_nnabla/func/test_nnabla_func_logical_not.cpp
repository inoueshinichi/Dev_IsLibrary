/**
 * @file test_nnabla_func_logical_not.cpp
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
    TEST(nnabla_func, logical_not_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        float* data1 = in_narr1->cast_data_and_get_pointer<float>(ctx_cpu);
        data1[0] = 0; data1[3] = std::numeric_limits<float>::infinity(); data1[6] = std::numeric_limits<float>::quiet_NaN();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto output = logical_not<float>(in_narr1);
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