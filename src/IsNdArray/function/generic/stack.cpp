#include <IsNdArray/array.hpp>
#include <IsNdArray/function/stack.hpp>
#include <IsNdArray/nd_array.hpp>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Stack, int)
        // axis

        template <typename T>
        void Stack<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const Shape_t in_shape = inputs[0]->shape();
            if (axis_ < 0)
                axis_ += in_shape.size() + 1;

            NBLA_CHECK(axis_ >= 0,
                        error_code::value,
                        "axis must not be less than zero, got %d",
                        axis_);

            NBLA_CHECK(static_cast<Shape_t::size_type>(axis_) <= in_shape.size(),
                        error_code::value,
                        "axis must be less than or equal to ndim of input. "
                        "axis: %d > ndim of inputs[0]: %d.",
                        axis_, 
                        in_shape.size());

            num_inputs_ = inputs.size();

            for (int i = 1; i < num_inputs_; ++i) 
            {
                NBLA_CHECK(inputs[i]->shape() == in_shape, 
                           error_code::value,
                           "All inputs must be the same size. "
                           "inputs[%d] shape: (%s) != inputs[0] shape: (%s).",
                           i, 
                           string_join(inputs[i]->shape(), string(", ")).c_str(),
                           string_join(in_shape, string(", ")).c_str());
            }
            Shape_t out_shape = inputs[0]->shape();
            out_shape.insert(out_shape.begin() + axis_, num_inputs_);
            outputs[0]->reshape(out_shape, true);
            inner_size_ = inputs[0]->size(axis_);
            outer_size_ = inputs[0]->size() / inner_size_;
        }


        template <typename T>
        void Stack<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int i0 = 0; i0 < num_inputs_; ++i0) 
            {
                const T* x = inputs[i0]->get_data_pointer<T>(this->ctx_);
                for (int i1 = 0; i1 < outer_size_; ++i1) 
                {
                    for (int i2 = 0; i2 < inner_size_; ++i2) 
                    {
                        y[i1 * (inner_size_ * num_inputs_) + i0 * inner_size_ + i2] = x[i1 * inner_size_ + i2];
                    }
                }
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Stack, bool)
        NBLA_INSTANTIATE_CLASS(Stack, char)
        NBLA_INSTANTIATE_CLASS(Stack, unsigned char)
        NBLA_INSTANTIATE_CLASS(Stack, short)
        NBLA_INSTANTIATE_CLASS(Stack, unsigned short)
        NBLA_INSTANTIATE_CLASS(Stack, int)
        NBLA_INSTANTIATE_CLASS(Stack, unsigned int)
        NBLA_INSTANTIATE_CLASS(Stack, long)
        NBLA_INSTANTIATE_CLASS(Stack, unsigned long)
        NBLA_INSTANTIATE_CLASS(Stack, long long)
        NBLA_INSTANTIATE_CLASS(Stack, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Stack, float)
        NBLA_INSTANTIATE_CLASS(Stack, double)
        NBLA_INSTANTIATE_CLASS(Stack, long double)
    }
}

