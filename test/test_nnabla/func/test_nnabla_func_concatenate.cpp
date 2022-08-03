/**
 * @file test_nnabla_func_concatenate.cpp
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
    TEST(nnabla_func, concatenate_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 0);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, concatenate_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 1);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, concatenate_3)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        in_narr1->reshape({1, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        in_narr2->reshape({1, 3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 0);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, concatenate_4)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        in_narr1->reshape({1, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        in_narr2->reshape({1, 3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 1);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, concatenate_5)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        in_narr1->reshape({1, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        in_narr2->reshape({1, 3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 2);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    // わざとエラーを出す.
    TEST(nnabla_func, concatenate_6)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = concatenate<float>({in_narr1, in_narr2}, 2); // axisはndimのサイズより小さくすること. そうしないとエラーを吐く.
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