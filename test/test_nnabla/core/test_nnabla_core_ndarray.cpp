/**
 * @file test_nnabla_core_ndarray.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla NdArray
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
    // NdArrary
    TEST(nnabla_core, ndarray)
    {
        using namespace std;
        using byte = unsigned char;
        string device_id{"cpu"};
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarr_1 = NdArray::create(Shape_t{3, 16, 16});
        ndarr_1->fill(128);
        ndarr_1->cast(dtypes::DOUBLE, ctx_cpu);
        show_ndarray_contents<float>(ndarr_1);

        cout << " test zeros ---------------" << endl;

        auto ndarray_zeros = zeros<byte>(Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ndarray_zeros);

        cout << " test ones ---------------" << endl;

        auto ndarray_ones = ones<byte>(Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ndarray_ones);

        cout << " test eye ---------------" << endl;

        auto ndarray_eye = eye<byte>(3, 5);
        show_ndarray_contents<float>(ndarray_eye);

        auto ndarray_eye2 = eye<byte>(5, 3);
        show_ndarray_contents<float>(ndarray_eye2);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
