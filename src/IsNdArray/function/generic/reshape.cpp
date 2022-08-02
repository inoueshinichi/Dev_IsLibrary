#include <IsNdArray/array.hpp>
#include <IsNdArray/function/reshape.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>
#include <cstring>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Reshape, const vector<int64_t>&, bool)
        // shape
        // inplace

        template <typename T>
        void Reshape<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            // A: Shape inference for an axis specified with negative size
            int tsize = inputs[0]->size();
            int rest_size = 1;
            int shape_infer_index = -1;
            for (int s = 0; static_cast<Shape_t::size_type>(s) < shape_.size(); ++s)
            {
                if (shape_[s] < 0) 
                {
                    NBLA_CHECK(shape_infer_index < 0, 
                               error_code::value,
                               "The shape option in Reshape function can take negative size "
                               "only in one axis. Given in %d and %d",
                               shape_infer_index, 
                               s);
                    shape_infer_index = s;
                    continue;
                }
                rest_size *= shape_[s];
            }

            if (shape_infer_index >= 0) 
            {
                shape_[shape_infer_index] = tsize / rest_size;
            }


            // B: Check if product of dimensions is total size of input.
            int tsize2 = 1;
            for (auto s : shape_)
                tsize2 *= s;

            NBLA_CHECK(tsize == tsize2, 
                       error_code::value,
                       "Product of dimensions of inputs and outputs must be same. "
                       "Inputs: %d != Outputs: %d.",
                       tsize, 
                       tsize2);


            // C: Reshape output
            outputs[0]->reshape(shape_, true);


            // D: Inplace
            if (inplace_) 
            {
                outputs[0]->set_array(inputs[0]->array());
            }
        }


        template <typename T>
        void Reshape<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (inplace_) 
            {
                return;
            }

            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int s = 0; s < inputs[0]->size(); ++s) 
            {
                y[s] = x[s];
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Reshape, bool)
        NBLA_INSTANTIATE_CLASS(Reshape, char)
        NBLA_INSTANTIATE_CLASS(Reshape, unsigned char)
        NBLA_INSTANTIATE_CLASS(Reshape, short)
        NBLA_INSTANTIATE_CLASS(Reshape, unsigned short)
        NBLA_INSTANTIATE_CLASS(Reshape, int)
        NBLA_INSTANTIATE_CLASS(Reshape, unsigned int)
        NBLA_INSTANTIATE_CLASS(Reshape, long)
        NBLA_INSTANTIATE_CLASS(Reshape, unsigned long)
        NBLA_INSTANTIATE_CLASS(Reshape, long long)
        NBLA_INSTANTIATE_CLASS(Reshape, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Reshape, float)
        NBLA_INSTANTIATE_CLASS(Reshape, double)
        NBLA_INSTANTIATE_CLASS(Reshape, long double)
    }
}