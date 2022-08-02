/**
 * @file test_nnabla_core_synced_array.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla SyncedArray
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
    // SyncedArray
    TEST(nnabla_core, synced_array)
    {
        using namespace std;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        auto sync_array_1 = make_shared<SyncedArray>(array_size);
        sync_array_1->zero();
        sync_array_1->cast(dtypes::UBYTE, ctx_cpu);
        cout << "sync_array_1: head_array_class -> " << sync_array_1->head_array_class() << endl;
        cout << "sync_array_1: get_num_arrays -> " << sync_array_1->get_num_arrays() << endl;
        cout << "sync_array_1: size -> " << sync_array_1->size() << endl;
        cout << "sync_array_1: modification_count -> " << sync_array_1->modification_count() << endl;
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
