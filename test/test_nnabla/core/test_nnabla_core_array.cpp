/**
 * @file test_nnabla_core_array.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla Array
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
    // Array
    TEST(nnabla_core, array)
    {
        using namespace std;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        shared_ptr<Array> cpu_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu));

        printf("cpu_array_1: pointer -> %p\n", cpu_array_1->pointer<float>());
        printf("cpu_array_1: const_pointer -> %p\n", cpu_array_1->const_pointer<float>());
        printf("cpu_array_1: dtype -> %d\n", cpu_array_1->dtype());
        printf("cpu_array_1: size -> %ld\n", cpu_array_1->size());
        cout << "cpu_array_1: context -> " << cpu_array_1->context().to_string() << endl;
        cout << "--------------------------------------" << endl;

        Context ctx_cpu_cache({"cpu:float"}, "CpuCachedArray", "0");
        shared_ptr<Array> cpu_cached_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu_cache));

        cout << "cpu_cached_array_1: context -> " << cpu_cached_array_1->context().to_string() << endl;

        cpu_cached_array_1->zero();
    }

}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
