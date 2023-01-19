#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/edge_detector/log_edge_detector.hpp>


namespace is
{
    namespace cv
    {
        using namespace nbla;

        NdArrayPtr log_edge_detector(NdArrayPtr src, double sigma) 
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);
            IS_DEBUG_CHECK_NDARRAY_STATE(__func__, IS_DEBUG_FLAG, src);

            const auto &ctx =
                SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();

            // LoGカーネル
            int ksize = 3;
            int kernel_size = ksize * ksize;
            double *p_kernel = new double[kernel_size];
            IS_ZERO_MEMORY(p_kernel, sizeof(double) * kernel_size);
            int hlf_ks = (int)(ksize / 2);

            // http://opencv.jp/opencv-2.1/cpp/image_filtering.html#getGaussianKernel
            sigma = (sigma != 0) ? sigma : 0.3 * (ksize / 2 - 1) + 0.8;
            IS_DEBUG_STREAM("sigma: %.3f\n", sigma);

            int dx, dy;
            double norm, sig2, coef, dist, val;
            double sum = 0;
            sig2 = sigma * sigma;
            
            for (int j = 0; j < ksize; ++j) 
            {
                for (int i = 0; i < ksize; ++i) 
                {
                    dy = j - hlf_ks;
                    dx = i - hlf_ks;
                    norm = dx *dx + dy *dy;
                    coef = norm - sig2;
                    dist = -0.5 * norm / (2 * sig2);
                    val = coef * std::exp(dist);
                    p_kernel[j * ksize + i] = val;
                    sum += val;
                }
            }
            
            // 正規化
            for (int k = 0; k < kernel_size; ++k) 
            {
                p_kernel[k] /= sum;
            }

            // パディング
            auto extend = padding<double>(src, hlf_ks, hlf_ks, IS_PADDING_MEAN);

            // 演算結果
            auto dst = zeros<double>(sh);

            // 畳み込み演算
            convolution<double>(dst, extend, Size(ksize, ksize), p_kernel);

            delete[] p_kernel;

            return dst;
        }
    } // imgproc
}