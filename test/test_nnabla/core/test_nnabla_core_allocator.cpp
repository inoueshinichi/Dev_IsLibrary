/**
 * @file test_nnabla_core_allocator.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla Allocator
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
    // Allocator
    TEST(nnabla_core, allocator)
    {
        using namespace std;
        using memory_type = CpuMemory;
        string device_id{ "cpu" };
        size_t bytes{ 64 };

        AllocatorMemory alloc_mem;
        shared_ptr<NaiveAllocator<memory_type>> 
            allocator(new NaiveAllocator<memory_type>());
        
        alloc_mem = allocator->alloc(bytes, device_id);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
