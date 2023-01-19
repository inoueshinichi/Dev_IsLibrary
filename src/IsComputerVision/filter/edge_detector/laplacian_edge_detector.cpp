#include <IsComputerVision/filter/utils/utils_filter.hpp>
#include <IsComputerVision/filter/edge_detector/laplacian_edge_detector.hpp>


namespace is
{
    namespace cv
    {
        using namespace nbla;

        NdArrayPtr laplacian_edge_detector(NdArrayPtr src, bool is_elem_8) 
        {
            IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(src);
            IS_DEBUG_CHECK_NDARRAY_STATE(__func__, IS_DEBUG_FLAG, src);

            const auto &ctx =
                SingletonManager::get<GlobalContext>()->get_current_context();
            auto sh = src->shape();
            auto st = src->strides();

            // Laplacianカーネル
            int ksize_x = 3;
            int ksize_y = 3;
            int kernel_size = ksize_x * ksize_y;
            double *p_kernel = new double[kernel_size];
            IS_ZERO_MEMORY(p_kernel, sizeof(double) * kernel_size);
            int hlf_ksx = (int)(ksize_x / 2);
            int hlf_ksy = (int)(ksize_y / 2);

            if (is_elem_8) 
            {
              p_kernel[0] = 1; p_kernel[1] = 1;  p_kernel[2] = 1;
              p_kernel[3] = 1; p_kernel[4] = -8; p_kernel[5] = 1;
              p_kernel[6] = 1; p_kernel[7] = 1;  p_kernel[8] = 1;
            }
            else 
            {
              p_kernel[0] = 0; p_kernel[1] = 1;  p_kernel[2] = 0;
              p_kernel[3] = 1; p_kernel[4] = -4; p_kernel[5] = 1;
              p_kernel[6] = 0; p_kernel[7] = 1;  p_kernel[8] = 0;
            }

            // パディング
            auto extend = padding<double>(src, hlf_ksx, hlf_ksy, IS_PADDING_MEAN);

            // 演算結果
            auto dst = zeros<double>(sh);

            // 畳み込み演算
            convolution<double>(dst, extend, Size(ksize_x, ksize_y), p_kernel);

            delete[] p_kernel;

            return dst;
        }
    }
}