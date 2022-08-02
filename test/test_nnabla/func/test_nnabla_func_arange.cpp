/**
 * @file test_nnabla_func_arange.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla arange-func
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
    // Arange
    TEST(nnabla_func, arange)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray = arange<float>(0.0, 10.0, 1.1);
        show_ndarray_contents<float>(ndarray);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}