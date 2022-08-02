#include <IsNdArray/function/max.hpp>

#include <cstring>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Max, const vector<int64_t>&, bool, bool, bool)


        template <typename T>
        void Max<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Sum<T>::setup_impl(inputs, outputs);

            int outer_size = inputs[0]->size() / this->reduction_size_;
            this->index_buff_ = make_shared<NdArray>(Shape_t{outer_size});

            if (this->with_index_ && !this->only_index_)
                outputs[1]->reshape(outputs[0]->shape(), true);
        }


        template <typename T>
        void Max<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Sum<T>::execute_impl(inputs, outputs); // 内部でthis->execute_impl_reduce()が実行される.

            // Indexを取得する場合
            if (this->with_index_ || this->only_index_) 
            {
                auto idx_sp_arr = this->only_index_ ? outputs[0] : outputs[1];
                auto idx_arr = idx_sp_arr->cast(get_dtype<size_t>(), this->ctx_, true);
                auto idx_buf = this->index_buff_->get(get_dtype<int>(), this->ctx_);

                idx_arr->copy_from(idx_buf);
            }
        }


        template <typename T>
        void Max<T>::execute_impl_reduce(const T *x, T *y, int outer_size, int reduction_size)
        {
            // Saving index is a bit inefficient if backward is not required.
            int* ind = this->index_buff_-> template cast_data_and_get_pointer<int>(this->ctx_, true);
            for (int o = 0; o < outer_size; ++o)
            {
                int max_index = 0;
                T max_val = -1e+8; // maxの初期値(最も小さい値)

                // maxを求める
                for (int i = 0; i < reduction_size; ++i) 
                {
                    const T val = x[o * reduction_size + i];
                    if (val > max_val) 
                    {
                        max_val = val;
                        max_index = i;
                    }
                }
                y[o] = max_val;
                ind[o] = max_index;
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Max, bool)
        NBLA_INSTANTIATE_CLASS(Max, char)
        NBLA_INSTANTIATE_CLASS(Max, unsigned char)
        NBLA_INSTANTIATE_CLASS(Max, short)
        NBLA_INSTANTIATE_CLASS(Max, unsigned short)
        NBLA_INSTANTIATE_CLASS(Max, int)
        NBLA_INSTANTIATE_CLASS(Max, unsigned int)
        NBLA_INSTANTIATE_CLASS(Max, long)
        NBLA_INSTANTIATE_CLASS(Max, unsigned long)
        NBLA_INSTANTIATE_CLASS(Max, long long)
        NBLA_INSTANTIATE_CLASS(Max, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Max, float)
        NBLA_INSTANTIATE_CLASS(Max, double)
        NBLA_INSTANTIATE_CLASS(Max, long double)
    }
}