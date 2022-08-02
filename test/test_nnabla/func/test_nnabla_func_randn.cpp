/**
 * @file test_nnabla_func_rand.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla rand-func
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
    // Randn double
    TEST(nnabla_func, randn_double)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        float mu = 0.0;
        float sigma = 1.0;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};
        auto ndarray_out_d = randn<double>(shape, mu, sigma, seed);
        cout << "double" << endl;
        show_ndarray_contents<double>(ndarray_out_d);
        cout << ndarray_out_d->get_data_pointer<double>(ctx_cpu)[0] << endl;
    }

    // Randn float
    TEST(nnabla_func, randn_float)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        float mu = 0.0;
        float sigma = 1.0;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};
        
        auto ndarray_out_f = randn<float>(shape, mu, sigma, seed);
        cout << "float" << endl;
        show_ndarray_contents<float>(ndarray_out_f);
        cout << ndarray_out_f->get_data_pointer<float>(ctx_cpu)[0] << endl;
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
