/**
 * @file test_nnabla_func_sub2.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla sub2-func
 * @version 0.1
 * @date 2022-7-31
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
    TEST(nnabla_func, sub2_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = sub2<float>(in_narr1, in_narr2);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, sub2_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3, 3});
        in_narr2 = mul_scalar<float>(in_narr2, 1.5); // * 2
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = sub2<float>(in_narr1, in_narr2);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
        cout << "--- in_narr1" << endl;
        show_ndarray_contents<float>(in_narr1);
        cout << "--- in_narr2" << endl;
        show_ndarray_contents<float>(in_narr2);
    }

    TEST(nnabla_func, sub2_3)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3, 1});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = sub2<float>(in_narr1, in_narr2);
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

