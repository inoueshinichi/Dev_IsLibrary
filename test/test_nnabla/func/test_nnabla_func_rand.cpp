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
    // Rand
    TEST(nnabla_func, rand)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        float low = 0.0;
        float high = 100;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};

        auto rand_ndarray = rand<float>(shape, low, high, seed);
        show_ndarray_contents<float>(rand_ndarray);
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
