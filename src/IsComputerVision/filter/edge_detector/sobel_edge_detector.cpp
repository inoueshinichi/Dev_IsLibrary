#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/edge_detector/sobel_edge_detector.hpp>

namespace is
{
    namespace imgproc
    {
        using namespace nbla;

        NdArrayPtr sobel_edge_detector(NdArrayPtr src, int direction)
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);
            IS_DEBUG_CHECK_NDARRAY_STATE(__func__, IS_DEBUG_FLAG, src);

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();

            // 微分カーネル
            int ksize_x = 3;
            int ksize_y = 3;
            int kernel_size = ksize_x * ksize_y;
            double *p_kernel = new double[kernel_size];
            IS_ZERO_MEMORY(p_kernel, sizeof(double) * kernel_size);
            int hlf_ksx = (int)(ksize_x / 2);
            int hlf_ksy = (int)(ksize_y / 2);

            // パディング
            auto extend = padding<double>(src, hlf_ksx, hlf_ksy, IS_PADDING_MEAN);

            // 演算結果
            auto dst = zeros<double>(sh);

            // 微分方向 8つ
            direction = direction % 7;

            // →
            if (direction == 0)
            {
                p_kernel[0] = -1;  p_kernel[1] = 0;  p_kernel[2] = 1;
                p_kernel[3] = -2;  p_kernel[4] = 0;  p_kernel[5] = 2;
                p_kernel[6] = -1;  p_kernel[7] = 0;  p_kernel[8] = 1;
            }
            // → ↓
            else if (direction == 1)
            {
                p_kernel[0] = -2;  p_kernel[1] = -1; p_kernel[2] = 0;
                p_kernel[3] = -1;  p_kernel[4] = 0;  p_kernel[5] = 1;
                p_kernel[6] = 0;   p_kernel[7] = 1;  p_kernel[8] = 2;
            }
            // ↓
            else if (direction == 2)
            {
                p_kernel[0] = -1; p_kernel[1] = -2; p_kernel[2] = -1;
                p_kernel[3] = 0;  p_kernel[4] = 0;  p_kernel[5] = 0;
                p_kernel[6] = 1;  p_kernel[7] = 2;  p_kernel[8] = 1;
            }
            // ← ↓
            else if (direction == 3)
            {
                p_kernel[0] = 0;  p_kernel[1] = -1; p_kernel[2] = -2;
                p_kernel[3] = 1;  p_kernel[4] = 0;  p_kernel[5] = -1;
                p_kernel[6] = 2;  p_kernel[7] = 1;  p_kernel[8] = 0;
            }
            // ←
            else if (direction == 4)
            {
                p_kernel[0] = 1; p_kernel[1] = 0; p_kernel[2] = -1;
                p_kernel[3] = 2; p_kernel[4] = 0; p_kernel[5] = -2;
                p_kernel[6] = 1; p_kernel[7] = 0; p_kernel[8] = -1;
            }
            // ← ↑
            else if (direction == 5)
            {
                p_kernel[0] = 2; p_kernel[1] = 1;  p_kernel[2] = 0;
                p_kernel[3] = 1; p_kernel[4] = 0;  p_kernel[5] = -1;
                p_kernel[6] = 0; p_kernel[7] = -1; p_kernel[8] = -2;
            }
            // ↑
            else if (direction == 6)
            {
                p_kernel[0] = 1;  p_kernel[1] = 2;  p_kernel[2] = 1;
                p_kernel[3] = 0;  p_kernel[4] = 0;  p_kernel[5] = 0;
                p_kernel[6] = -1; p_kernel[7] = -2; p_kernel[8] = -1;
            }
            // → ↑
            else 
            { // 7
                p_kernel[0] = 0;  p_kernel[1] = 1;  p_kernel[2] = 2;
                p_kernel[3] = -1; p_kernel[4] = 0;  p_kernel[5] = 1;
                p_kernel[6] = -2; p_kernel[7] = -1; p_kernel[8] = 0;
            }

            // 畳み込み演算
            convolution<double>(dst, extend, Size(ksize_x, ksize_y), p_kernel);

            delete[] p_kernel;

            return dst;
        }
    }
}