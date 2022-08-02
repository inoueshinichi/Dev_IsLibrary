#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/average_blur.hpp>

namespace is
{
    namespace imgproc
    {
        using uchar = unsigned char;

        NdArrayPtr average_blur(NdArrayPtr src, const Size& ksize)
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);
            IS_DEBUG_CHECK_NDARRAY_STATE(__func__, IS_DEBUG_FLAG, src);

            const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();

            int ksize_x = ksize.width();
            int ksize_y = ksize.height();
            if (ksize_x % 2 == 0)
                ksize_x += 1;
            if (ksize_y % 2 == 0)
                ksize_y += 1;

            int hlf_ksx = (int)(ksize_x / 2);
            int hlf_ksy = (int)(ksize_y / 2);

            // 平均値カーネル
            int kernel_size = ksize_x * ksize_y;
            double *p_kernel = new double[kernel_size];
            IS_ZERO_MEMORY(p_kernel, sizeof(double) * kernel_size);
            for (int k = 0; k < kernel_size; ++k)
                p_kernel[k] = 1.0 / kernel_size;

            // パディング
            auto extend = padding<uchar>(src, hlf_ksx, hlf_ksy, IS_PADDING_MEAN);
            
            // 演算結果
            auto dst = zeros<uchar>(sh);

            auto start = std::chrono::high_resolution_clock::now();

            // 畳み込み演算
            convolution<uchar>(dst, extend, Size(ksize_x, ksize_y), p_kernel);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Convolution: " << duration << "[ms]" << std::endl;

            delete[] p_kernel;

            return dst;
        }
    }
}