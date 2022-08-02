/**
 * @file test_nnabla_func_mul2.cpp
 * @author your name (you@domain.com)
 * @brief [Test] Nnabla mul2-func
 * @version 0.1
 * @date 2022-7-31
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
    TEST(nnabla_func, mul2_1)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3, 3});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = mul2<float>(in_narr1, in_narr2);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, mul2_2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 256, 256});
        in_narr1 = mul_scalar<float>(in_narr1, 2); // * 2
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        //show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 256, 256});
        in_narr2 = mul_scalar<float>(in_narr2, 1.5); // * 1.5
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        //show_ndarray_contents<float>(in_narr2);

        auto output = mul2<float>(in_narr1, in_narr2);
        cout << "--- output" << endl;
        //show_ndarray_contents<float>(output);
        cout << "--- in_narr1" << endl;
        //show_ndarray_contents<float>(in_narr1);
        cout << "--- in_narr2" << endl;
        //show_ndarray_contents<float>(in_narr2);
    }

    TEST(nnabla_func, mul2_3)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 3, 3});
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        cout << "--- in_arr1" << endl;
        show_ndarray_contents<float>(in_narr1);

        auto in_narr2 = ones<float>(Shape_t{3, 3, 1});
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        cout << "--- in_arr2" << endl;
        show_ndarray_contents<float>(in_narr2);

        auto output = mul2<float>(in_narr1, in_narr2);
        cout << "--- output" << endl;
        show_ndarray_contents<float>(output);
    }

    TEST(nnabla_func, native_mul2)
    {
        using namespace std;
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto start = std::chrono::high_resolution_clock::now();

        // input
        auto in_narr1 = ones<float>(Shape_t{3, 256, 256});
        in_narr1 = mul_scalar<float>(in_narr1, 2); // * 2
        auto st1 = in_narr1->strides();
        auto sh1 = in_narr1->shape();
        const float* data1 = in_narr1->get_data_pointer<float>(ctx_cpu);
        cout << "--- in_arr1" << endl;
        //show_ndarray_contents<float>(in_narr1);

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = end - start;
        double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << "gen in_narr1:" << duration_ms << " [ms]" << std::endl;

        start = std::chrono::high_resolution_clock::now();

        auto in_narr2 = ones<float>(Shape_t{3, 256, 256});
        in_narr2 = mul_scalar<float>(in_narr2, 1.5); // * 1.5
        auto st2 = in_narr2->strides();
        auto sh2 = in_narr2->shape();
        const float* data2 = in_narr2->get_data_pointer<float>(ctx_cpu);
        cout << "--- in_arr2" << endl;
        //show_ndarray_contents<float>(in_narr2);

        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << "gen in_narr2:" << duration_ms << " [ms]" << std::endl;

        start = std::chrono::high_resolution_clock::now();

        // auto output = mul2<float>(in_narr1, in_narr2);
        auto output = zeros<float>(Shape_t{3, 256, 256});
        float* out = output->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < sh2[0]; ++c)
            for (int y = 0; y < sh2[1]; ++y)
                for (int x = 0; x < sh2[2]; ++x)
                {
                    out[c * st2[0] + y * st2[1] + x * st2[2]] = 
                        data1[c * st2[0] + y * st2[1] + x * st2[2]] * 
                        data2[c * st2[0] + y * st2[1] + x * st2[2]];
                }

        end = std::chrono::high_resolution_clock::now();

        duration = end - start;
        duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << "calc in_narr1 * in_narr2:" << duration_ms << " [ms]" << std::endl;


        cout << "--- output" << endl;
        //show_ndarray_contents<float>(output);
        cout << "--- in_narr1" << endl;
        //show_ndarray_contents<float>(in_narr1);
        cout << "--- in_narr2" << endl;
        //show_ndarray_contents<float>(in_narr2);
    }

    TEST(nnabla_func, lowcode_mul2)
    {
        using namespace std;
        int rows = 256;
        int cols = 256;
        int channels = 3;

        float* data1 = new float[channels * rows * cols];
        float* data2 = new float[channels * rows * cols];
        float* out = new float[channels * rows * cols];

        std::memset((void*)data1, 2, sizeof(float) * rows * cols * channels);
        std::memset((void*)data2, 3, sizeof(float) * rows * cols * channels);
        std::memset((void*)out, 0, sizeof(float) * rows * cols * channels);

        auto start = std::chrono::high_resolution_clock::now();

        for (int c = 0; c < channels; ++c)
            for (int y = 0; y < rows; ++y)
                for (int x = 0; x < cols; ++x)
                {
                    out[c * channels + y * rows + x * cols] = 
                        data1[c * channels + y * rows + x * cols] *
                        data2[c * channels + y * rows + x * cols];
                }

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = end - start;
        double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << "result:" << duration_ms << " [ms]" << std::endl;

        delete[] data1;
        delete[] data2;
        delete[] out;
    }    
}

int main(int, char**)
{
    std::cout << __FILE__ << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

