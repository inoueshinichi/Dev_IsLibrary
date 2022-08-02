#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/function/cumsum.hpp>
#include <IsNdArray/nd_array.hpp>



namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(CumSum, int, bool, bool)
        // axis
        // exclusive
        // reverse

        template <typename T>
        void CumSum<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs) 
        {
            Shape_t in_shape = inputs[0]->shape();
            Size_t size = inputs[0]->size();

            if (axis_ < 0) 
            {
                axis_ += in_shape.size();

                NBLA_CHECK(axis_ >= 0, 
                           error_code::value,
                           "Absolute value of axis must be less than that of input ndim. "
                           "axes[%d]: %d >= ndim of input: %d.",
                           abs(axis_ - static_cast<int>(in_shape.size())), 
                           in_shape.size());
            }

            NBLA_CHECK(static_cast<unsigned>(axis_) < in_shape.size(), 
                       error_code::value,
                       "axis must be less than ndim of inputs[0]. "
                       "axis: %d >= ndim of inputs[0]: %d.",
                       axis_, 
                       in_shape.size());
            
            Size_t size_axis = inputs[0]->size(axis_);

            size_ = inputs[0]->size();       // Total size
            size0_ = size / size_axis;       // Batch size.
            size1_ = in_shape[axis_];        // Size of specified axis.
            size2_ = size / size0_ / size1_; // Size of rest.

             outputs[0]->reshape(in_shape, true);
        }


        template <typename T>
        void CumSum<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs) 
        {
            using AccumType = typename force_float<T>::type;

            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            AccumType* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);

            for (int i0 = 0; i0 < size0_; ++i0) 
            {
                for (int i2 = 0; i2 < size2_; ++i2) 
                {
                    const int j = i0 * size1_ * size2_ + i2;
                    for (int idx = 0; idx < size1_; ++idx) 
                    {
                        const int i1 = reverse_ ? size1_ - idx - 1 : idx;
                        const int y_k = i1 * size2_ + j;
                        if (idx == 0) 
                        {
                            // To prevent accessing out-of-bounds.
                            y[y_k] = exclusive_ ? 0 : x[y_k];
                            continue;
                        }
                        const int d = reverse_ ? -1 : 1;
                        const int y_k_prev = y_k - d * size2_;
                        const int x_k = exclusive_ ? y_k_prev : y_k;

                        y[y_k] = y[y_k_prev] + x[x_k];
                    }
                }
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(CumSum, bool)
        NBLA_INSTANTIATE_CLASS(CumSum, char)
        NBLA_INSTANTIATE_CLASS(CumSum, unsigned char)
        NBLA_INSTANTIATE_CLASS(CumSum, short)
        NBLA_INSTANTIATE_CLASS(CumSum, unsigned short)
        NBLA_INSTANTIATE_CLASS(CumSum, int)
        NBLA_INSTANTIATE_CLASS(CumSum, unsigned int)
        NBLA_INSTANTIATE_CLASS(CumSum, long)
        NBLA_INSTANTIATE_CLASS(CumSum, unsigned long)
        NBLA_INSTANTIATE_CLASS(CumSum, long long)
        NBLA_INSTANTIATE_CLASS(CumSum, unsigned long long)
        NBLA_INSTANTIATE_CLASS(CumSum, float)
        NBLA_INSTANTIATE_CLASS(CumSum, double)
        NBLA_INSTANTIATE_CLASS(CumSum, long double)
    }
}