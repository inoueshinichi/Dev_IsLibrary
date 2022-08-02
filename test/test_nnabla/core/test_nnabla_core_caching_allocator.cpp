/**
 * @file test_nnabla_core_caching_allocator.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla CachingAllocator
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
    // CachingAllocator
    TEST(nnabla_core, caching_allocator)
    {
        using namespace std;
        using memory_type = CpuMemory;
        string device_id{ "cpu" };

        shared_ptr<CachingAllocatorWithBuckets<memory_type>> 
            allocator = make_shared<CachingAllocatorWithBuckets<memory_type>>();

        // 初めてのメモリ生成(小)では, 内部で1MBのメモリプールが生成されて、512B単位で必要な容量だけ切り崩している
        // 初めてのメモリ生成(大)では, 内部で生成したメモリ数だけ確保する
        AllocatorMemory alloc_mem_small_1 = allocator->alloc(64, device_id); // 512B
        AllocatorMemory alloc_mem_small_2 = allocator->alloc(530, device_id); // 1024B
        AllocatorMemory alloc_mem_large_1 = allocator->alloc((1 << 20) + 100, device_id);

        // キャッシュされているメモリを表示
        cout << "1) Device_Memory_Used_In_Bytes: " 
                << allocator->device_memory_used_in_bytes(device_id)
                << endl;
        cout << endl;

        // キャシュされている不要なメモリを削除
        allocator->free_unused_device_caches(device_id);

        // キャッシュされているメモリを表示
        cout << "2) Device_Memory_Used_In_Bytes: "
                << allocator->device_memory_used_in_bytes(device_id)
                << endl;
        cout << endl;

        // alloc_mem_small_2に対応するMemoryを破棄
        allocator->free(alloc_mem_small_2.memory());

        // キャッシュされているメモリを表示
        cout << "3) Device_Memory_Used_In_Bytes: "
                << allocator->device_memory_used_in_bytes(device_id)
                << endl;
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}