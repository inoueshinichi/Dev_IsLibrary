/**
 * @file test_nnabla_func_isinf.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-03
 * 
 * @copyright Copyright (c) 2022
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
     TEST(nnabla_func, isinf_1)
    {
        using namespace std;
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // zero
        auto ndarray_zeros = zeros<float>(Shape_t{3, 3});
        auto st = ndarray_zeros->strides();
        auto sh = ndarray_zeros->shape();
        float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);

        data[0 * st[0] + 0 * st[1]] = 1.5;
        data[1 * st[0] + 1 * st[1]] = -1.5;
        data[2 * st[0] + 2 * st[1]] = std::numeric_limits<float>::infinity();

        show_ndarray_contents<float>(ndarray_zeros);

        cout << "---" <<endl;

        auto out_ndarray = is::common::invoke_tm_chrono_ms_ret(is::nbla::isinf<float>, ndarray_zeros);

        show_ndarray_contents<bool>(out_ndarray);
    }
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}