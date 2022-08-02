/**
 * @file test_nnabla_core_memory.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla Memory
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
    // Memory
    TEST(nnabla_core, memory)
    {
        using namespace std;
        string device_id {"cpu"};
        size_t bytes {64};
        size_t second_start_offset = 40;

        // 空メモリ
        CpuMemory mem = CpuMemory(bytes, device_id);
        show_status_memory(&mem);
        
        // メモリ確保
        mem.alloc();
        show_status_memory(&mem);

        // メモリ分割
        auto sub_mem = mem.divide(second_start_offset);
        printf("Memory ->\n");
        show_status_memory(&mem);
        printf("SubMemory ->\n");
        show_status_memory(sub_mem.get());

        // メモリ統合
        mem.try_merge(sub_mem.get());
        printf("SubMemory ->\n");
        show_status_memory(sub_mem.get());
        printf("Memory ->\n");
        show_status_memory(&mem);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}