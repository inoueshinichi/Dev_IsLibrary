/**
 * @file test_nnabla_func_rand.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla randint-func
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
   // Randint
    TEST(nnabla_func, randint)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        int low = 0;
        int high = 100;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};

        auto randint_ndarray = randint<int>(shape, low, high, seed);

        show_ndarray_contents<int>(randint_ndarray);
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
