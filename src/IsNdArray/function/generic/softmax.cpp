#include <IsNdArray/array.hpp>
#include <IsNdArray/function/softmax.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>
#include <cmath>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Softmax, int)

        template <typename T>
        void Softmax<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Shape_t in_shape = inputs[0]->shape();
            if (axis_ < 0) 
            {
                axis_ += in_shape.size();
                NBLA_CHECK(axis_ >= 0, error_code::value,
                            "Absolute value of axis must be less than that of input ndim. "
                            "axes[%d]: %d >= ndim of input: %d.",
                            abs(axis_ - static_cast<int>(in_shape.size())), 
                            in_shape.size());
            }
            NBLA_CHECK(static_cast<unsigned>(axis_) < in_shape.size(), error_code::value,
                        "axis must be less than ndim of inputs[0]. "
                        "axis: %d >= ndim of inputs[0]: %d.",
                        axis_, in_shape.size());
            outputs[0]->reshape(in_shape, true);
            Size_t size = inputs[0]->size();
            Size_t size_axis = inputs[0]->size(axis_);
            size0_ = size / size_axis;          // Batch size.
            size1_ = inputs[0]->shape()[axis_]; // Size of specified axis.
            size2_ = size / size0_ / size1_;    // Size of rest.
            NBLA_CHECK(size0_ * size1_ * size2_ == size, error_code::unclassified,
                        "An error occurred during setup Softmax function.");
        }

        template <class T>
        void Softmax<T>::execute_impl(const NdArrays& inputs,
                                     const NdArrays& outputs) 
        {
            typedef typename force_float<T>::type AccumType;

            // Setting up ndarrays
            const T *x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T *y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int i0 = 0; i0 < size0_; ++i0) 
            {
                for (int i2 = 0; i2 < size2_; ++i2) 
                {
                    const int j = i0 * size1_ * size2_ + i2;

                    // compute maximum
                    T max_x = x[j];
                    for (int i1 = 0; i1 < size1_; ++i1) 
                    {
                        const int k = i1 * size2_ + j;
                        max_x = (max_x >= x[k]) ? max_x : x[k];
                    }

                    // compute exponential and sum
                    AccumType exp_sum = 0;
                    for (int i1 = 0; i1 < size1_; ++i1) 
                    {
                        const int k = i1 * size2_ + j;
                        const T tmp = std::exp(x[k] - max_x);
                        y[k] = tmp;
                        exp_sum += tmp;
                    }

                    // compute softmax
                    for (int i1 = 0; i1 < size1_; ++i1) 
                    {
                        const int k = i1 * size2_ + j;
                        y[k] = y[k] / exp_sum;
                    }
                }
            }
        }


        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        // NBLA_INSTANTIATE_CLASS(Softmax, bool)
        // NBLA_INSTANTIATE_CLASS(Softmax, char)
        // NBLA_INSTANTIATE_CLASS(Softmax, unsigned char)
        // NBLA_INSTANTIATE_CLASS(Softmax, short)
        // NBLA_INSTANTIATE_CLASS(Softmax, unsigned short)
        // NBLA_INSTANTIATE_CLASS(Softmax, int)
        // NBLA_INSTANTIATE_CLASS(Softmax, unsigned int)
        // NBLA_INSTANTIATE_CLASS(Softmax, long)
        // NBLA_INSTANTIATE_CLASS(Softmax, unsigned long)
        // NBLA_INSTANTIATE_CLASS(Softmax, long long)
        // NBLA_INSTANTIATE_CLASS(Softmax, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Softmax, float)
        NBLA_INSTANTIATE_CLASS(Softmax, double)
        NBLA_INSTANTIATE_CLASS(Softmax, long double)
    }
}