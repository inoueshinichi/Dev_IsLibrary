#include <IsNdArray/array.hpp>
#include <IsNdArray/function/concatenate.hpp>
#include <IsNdArray/nd_array.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Concatenate, int)


        template <typename T>
        void Concatenate<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Shape_t shape = inputs[0]->shape();
            if (axis_ < 0)
                axis_ += shape.size(); // 最下位の次元で連結
            
            NBLA_CHECK(axis_ >= 0, 
                       error_code::value,
                       "axis must not be less than zero, got %d", 
                       axis_);

            auto axis = static_cast<Shape_t::size_type>(this->axis_);

            NBLA_CHECK(axis < shape.size(), // axis <= shape.size() -> axis < shape.size() に変更. '22.8.3
                       error_code::value,
                       "axis must be less than or equal to ndim of input. "
                       "axis: %d > ndim of input: %d.",
                       axis_, 
                       shape.size());


            inner_total_size_ = 0;
            for (NdArrays::size_type i = 0; i < inputs.size(); ++i)
            {
                // 連結する多次元配列の次元が有効かチェック
                NBLA_CHECK(inputs[i]->shape().size() != 0, 
                           error_code::value,
                           "input value(inputs[%d]) does not exist. "
                           "inputs[%d]->shape().size(): %d.",
                           i,
                           i, 
                           inputs[i]->shape().size());
            
                const int inner_size = inputs[i]->size(this->axis_);
                inner_total_size_ += inner_size;

                if (i >= 1) 
                {
                    shape[axis_] += inputs[i]->shape()[axis_]; // Accumulate size along axis
                    for (Shape_t::size_type j = 0; j < shape.size(); j++) 
                    {
                        if (j != axis) 
                        {
                            NBLA_CHECK(inputs[i]->shape()[j] == shape[j], 
                                       error_code::value,
                                       "Dimensions of inputs must match. "
                                       "inputs[%d]->shape()[%d]: %d != "
                                       "inputs[0]->shape()[%d]: %d.",
                                       i, 
                                       j, 
                                       inputs[i]->shape()[j], 
                                       j, 
                                       shape[j]);
                        }
                    }
                }
            } // for (NdArrays::size_type i = 0; i < inputs.size(); ++i)

            outputs[0]->reshape(shape, true);
            outer_size_ = inputs[0]->size() / inputs[0]->size(axis_);
        }


        template <typename T>
        void Concatenate<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            int inner_offset = 0;
            for (NdArrays::size_type c = 0; c < inputs.size(); ++c) 
            {
                const T* x = inputs[c]->get_data_pointer<T>(this->ctx_);
                const int inner_size = inputs[c]->size(this->axis_);

                for (int o = 0; o < outer_size_; ++o) 
                {
                    for (int i = 0; i < inner_size; ++i) 
                    {
                        y[o * inner_total_size_ + inner_offset + i] = x[o * inner_size + i];
                    }
                }
                inner_offset += inner_size;
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Concatenate, bool)
        NBLA_INSTANTIATE_CLASS(Concatenate, char)
        NBLA_INSTANTIATE_CLASS(Concatenate, unsigned char)
        NBLA_INSTANTIATE_CLASS(Concatenate, short)
        NBLA_INSTANTIATE_CLASS(Concatenate, unsigned short)
        NBLA_INSTANTIATE_CLASS(Concatenate, int)
        NBLA_INSTANTIATE_CLASS(Concatenate, unsigned int)
        NBLA_INSTANTIATE_CLASS(Concatenate, long)
        NBLA_INSTANTIATE_CLASS(Concatenate, unsigned long)
        NBLA_INSTANTIATE_CLASS(Concatenate, long long)
        NBLA_INSTANTIATE_CLASS(Concatenate, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Concatenate, float)
        NBLA_INSTANTIATE_CLASS(Concatenate, double)
        NBLA_INSTANTIATE_CLASS(Concatenate, long double)
    }
}