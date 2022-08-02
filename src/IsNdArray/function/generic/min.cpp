#include <IsNdArray/function/min.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Min, const vector<int64_t>&, bool, bool, bool)
        // axes
        // keep_dims
        // with_index
        // only_index

        template <typename T>
        void Min<T>::execute_impl_reduce(const T* x, T* y, int outer_size, int reduction_size)
        {
            // Saving index is a bit inefficient if backward is not required.
            // int* ind = this->index_buff_->cast_data_and_get_pointer<int>(this->ctx_, true);
            // for (int o = 0; o < outer_size; ++o)
            // {
            //     int min_index = 0;
            //     // T min_val = 1e+8; // minの初期値(最も大きい値)

            //     // minを求める
            //     for (int i = 0; i < reduction_size; ++i) 
            //     {
            //         const T val = x[o * reduction_size + i];
            //         if (val < min_val) 
            //         {
            //             min_val = val;
            //             min_index = i;
            //         }
            //     }
            //     y[o] = min_val;
            //     ind[o] = min_index;
            // }

            auto _cast = [this](NdArray* v) {
                return v->cast_data_and_get_pointer<int>(this->ctx_, true);
            };

            int *ind = _cast(this->index_buff_.get());
            
            for (int o = 0; o < outer_size; ++o) 
            {
                int min_index = 0;
                T min_val = 1e+8; // minの初期値(最も大きい値)

                for (int i = 0; i < reduction_size; ++i) 
                {
                    const T v = x[o * reduction_size + i];
                    if (v < min_val) 
                    {
                        min_val = v;
                        min_index = i;
                    }
                }
                y[o] = min_val;
                ind[o] = min_index;
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Min, bool)
        NBLA_INSTANTIATE_CLASS(Min, char)
        NBLA_INSTANTIATE_CLASS(Min, unsigned char)
        NBLA_INSTANTIATE_CLASS(Min, short)
        NBLA_INSTANTIATE_CLASS(Min, unsigned short)
        NBLA_INSTANTIATE_CLASS(Min, int)
        NBLA_INSTANTIATE_CLASS(Min, unsigned int)
        NBLA_INSTANTIATE_CLASS(Min, long)
        NBLA_INSTANTIATE_CLASS(Min, unsigned long)
        NBLA_INSTANTIATE_CLASS(Min, long long)
        NBLA_INSTANTIATE_CLASS(Min, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Min, float)
        NBLA_INSTANTIATE_CLASS(Min, double)
        NBLA_INSTANTIATE_CLASS(Min, long double)
    }
}