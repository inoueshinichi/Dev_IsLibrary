#include <IsNdArray/array.hpp>
#include <IsNdArray/function/flip.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>
#include <cstring>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Flip, const vector<int64_t>&)
        // axes

        template <typename T>
        void Flip<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            for (std::size_t i = 0; i < axes_.size(); ++i) 
            {
                if (axes_[i] < 0)
                    axes_[i] += inputs[0]->shape().size();
            }

            outputs[0]->reshape(inputs[0]->shape(), true);
            flip_.resize(inputs[0]->ndim());
        }



        template <typename T>
        void Flip<T>::flip_recursive(NdArrayPtr inp, const T* x, T* y,
                             const std::vector<bool> &flip, bool add,
                             int x_offset, int y_offset, int dim)
        {
            int current_x_offset = x_offset, current_y_offset = y_offset;
            const int y_stride = inp->strides()[dim];
            int x_stride = y_stride;
            const int size = inp->shape()[dim];

            if (flip[dim]) 
            {
                current_x_offset += x_stride * (size - 1);
                x_stride = -x_stride;
            }


            if (dim == inp->ndim() - 1) 
            {
                const T* current_x = x + current_x_offset;
                const T* end_x = current_x + size * x_stride;
                T* current_y = y + current_y_offset;
                if (add) 
                {
                    while (current_x != end_x) 
                    {
                        *current_y += *current_x;
                        current_x += x_stride;
                        current_y += y_stride;
                    }
                } 
                else 
                {
                    if (x_stride == 1) 
                    {
                        memcpy((void *)current_y, current_x, sizeof(T) * size);
                    } 
                    else
                    {
                        while (current_x != end_x) 
                        {
                            *current_y = *current_x;
                            current_x += x_stride;
                            current_y += y_stride;
                        }
                    }
                }
            } 
            else 
            {
                for (int i = 0; i < size; i++) 
                {
                    flip_recursive(inp, x, y, flip, add, current_x_offset, current_y_offset, dim + 1);
                    current_x_offset += x_stride;
                    current_y_offset += y_stride;
                }
            }
        }

        template <typename T>
        void Flip<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int id = 0; id < inputs[0]->ndim(); id++) 
            {
                auto itr = std::find(axes_.begin(), axes_.end(), id);
                flip_[id] = itr != axes_.end();
            }
            flip_recursive(inputs[0], x, y, flip_, false, 0, 0, 0);
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Flip, bool)
        NBLA_INSTANTIATE_CLASS(Flip, char)
        NBLA_INSTANTIATE_CLASS(Flip, unsigned char)
        NBLA_INSTANTIATE_CLASS(Flip, short)
        NBLA_INSTANTIATE_CLASS(Flip, unsigned short)
        NBLA_INSTANTIATE_CLASS(Flip, int)
        NBLA_INSTANTIATE_CLASS(Flip, unsigned int)
        NBLA_INSTANTIATE_CLASS(Flip, long)
        NBLA_INSTANTIATE_CLASS(Flip, unsigned long)
        NBLA_INSTANTIATE_CLASS(Flip, long long)
        NBLA_INSTANTIATE_CLASS(Flip, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Flip, float)
        NBLA_INSTANTIATE_CLASS(Flip, double)
        NBLA_INSTANTIATE_CLASS(Flip, long double)
    }
}