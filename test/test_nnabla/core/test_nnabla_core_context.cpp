/**
 * @file test_nnabla_core_context.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla Context
 * @version 0.1
 * @date 2021-12-11
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
    // Context
    TEST(nnabla_core, context)
    {
        using namespace std;
        Context ctx_cpu;
        cout << ctx_cpu.array_class << endl;
        cout << ctx_cpu.device_id << endl;
        cout << ctx_cpu.to_string() << endl;
        cout << get_array_key_from_context(ctx_cpu) << endl;
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}