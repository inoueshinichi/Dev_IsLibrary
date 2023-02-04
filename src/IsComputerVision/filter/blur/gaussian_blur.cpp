#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/blur/gaussian_blur.hpp>

namespace is
{
    namespace cv
    {
        using uchar = unsigned char;

        /**
         * @brief ガウシアンフィルタ
         * 
         * @param src 
         * @param ksize 
         * @param sigma 
         * @return NdArrayPtr 
         */
        NdArrayPtr gaussian_blur(NdArrayPtr src, int ksize, double sigma)
        {
            IS_DEBUG_NDARRAY_SHAPE_AS_IMAGE(src)

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();

            if (ksize % 2 == 0)
                ksize += 1;

            int hlf_ks = (int)(ksize / 2);

            // ガウシアンカーネル(2次の正規分布)

            // http://opencv.jp/opencv-2.1/cpp/image_filtering.html#getGaussianKernel
            sigma = (sigma != 0) ? sigma : 0.3 * (ksize / 2 - 1) + 0.8;
            IS_DEBUG_LOG("sigma: %.3f\n", sigma);

            int kernel_size = ksize * ksize;
            double *p_kernel = new double[kernel_size];
            int dx, dy;
            double e, val;
            double sum = 0;

            sum = 0;
            for (int j = 0; j < ksize; ++j) {
              for (int i = 0; i < ksize; ++i) {
                dy = j - hlf_ks;
                dx = i - hlf_ks;
                e = -1 * (dy * dy + dx * dx) / (2 * sigma * sigma);
                val = std::exp(e);
                p_kernel[j * ksize + i] = val;
                sum += val;
              }
            }
            for (int k = 0; k < kernel_size; ++k) {
                p_kernel[k] /= sum;
            }

            // パディング
            auto extend = padding<ubyte>(src, hlf_ks, hlf_ks, IS_PADDING_MEAN);

            // 演算結果
            auto dst = zeros<ubyte>(sh);

            auto start = std::chrono::high_resolution_clock::now();

            // 畳み込み演算
            convolution<ubyte>(dst, extend, Size(ksize, ksize), p_kernel);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "Convolution: " << duration << "[ms]" << std::endl;
            delete[] p_kernel; p_kernel = nullptr;
            return dst;
        }
    }
}