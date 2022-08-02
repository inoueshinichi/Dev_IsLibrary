#include <IsNdArray/array.hpp>
#include <IsNdArray/function/split.hpp>
#include <IsNdArray/nd_array.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Split, int)


        template <typename T>
        void Split<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const Shape_t in_shape = inputs[0]->shape();
            if (axis_ < 0)
                axis_ += in_shape.size();
            
            NBLA_CHECK(axis_ >= 0,
                       error_code::value,
                       "axis must not be less than zero, got %d", 
                       axis_);

            NBLA_CHECK(static_cast<Shape_t::size_type>(axis_) < in_shape.size(),
                       error_code::value, 
                       "axis must be less than ndim of inputs[0]. "
                       "axis: %d >= ndim of inputs[0]: %d.",
                       axis_, 
                       in_shape.size());

            num_outputs_ = in_shape[axis_];

            NBLA_CHECK(static_cast<Shape_t::size_type>(num_outputs_) == outputs.size(),
                       error_code::value,
                       "inputs[0].shape[axis] must be the same number as the outputs. "
                       "inputs[0].shape[axis]: %d, outputs: %d.",
                       num_outputs_, 
                       outputs.size());
            
            Shape_t out_shape = in_shape;
            out_shape.erase(out_shape.begin() + axis_);

            for (int i = 0; i < num_outputs_; ++i) 
            {
                outputs[i]->reshape(out_shape, true);
            }
            inner_size_ = outputs[0]->size(axis_);

            NBLA_CHECK(inner_size_ != 0, 
                       error_code::unclassified,
                       "Zero is specified as the input value.");

            outer_size_ = outputs[0]->size() / inner_size_;

            NBLA_CHECK(inner_size_ * num_outputs_ * outer_size_ == inputs[0]->size(),
                       error_code::unclassified,
                       "An error occurred during setup Split function.");
        }


        template <typename T>
        void Split<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            for (int i0 = 0; i0 < num_outputs_; ++i0) 
            {
                T* y = outputs[i0]->cast_data_and_get_pointer<T>(this->ctx_, true);
                for (int i1 = 0; i1 < outer_size_; ++i1) 
                {
                    for (int i2 = 0; i2 < inner_size_; ++i2) 
                    {
                        y[i1 * inner_size_ + i2] = x[i1 * (inner_size_ * num_outputs_) + i0 * inner_size_ + i2];
                    }
                }
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Split, bool)
        NBLA_INSTANTIATE_CLASS(Split, char)
        NBLA_INSTANTIATE_CLASS(Split, unsigned char)
        NBLA_INSTANTIATE_CLASS(Split, short)
        NBLA_INSTANTIATE_CLASS(Split, unsigned short)
        NBLA_INSTANTIATE_CLASS(Split, int)
        NBLA_INSTANTIATE_CLASS(Split, unsigned int)
        NBLA_INSTANTIATE_CLASS(Split, long)
        NBLA_INSTANTIATE_CLASS(Split, unsigned long)
        NBLA_INSTANTIATE_CLASS(Split, long long)
        NBLA_INSTANTIATE_CLASS(Split, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Split, float)
        NBLA_INSTANTIATE_CLASS(Split, double)
        NBLA_INSTANTIATE_CLASS(Split, long double)
    }
}