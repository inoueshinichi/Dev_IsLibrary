#include <IsNdArray/array.hpp>
#include <IsNdArray/function/add2.hpp>
#include <IsNdArray/function/bc_add2.hpp>
#include <IsNdArray/nd_array.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Add2, bool)

        template <typename T>
        void Add2<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (inputs[0]->shape() == inputs[1]->shape()) 
            {
                outputs[0]->reshape(inputs[0]->shape(), true);
                return;
            }

            // ブロードキャスト
            // Trying to fallback to broadcastable Add2.
            this->fall_back_func_ = create_BcAdd2(this->ctx_, false);
            this->fall_back_func_->setup(inputs, outputs);
        }


        template <class T>
        void Add2<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x0 = inputs[0]->get_data_pointer<T>(this->ctx_);
            const T* x1 = inputs[1]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true/*!inplace_*/);

            for (int s = 0; s < inputs[0]->size(); s++) 
            {
                y[s] = x0[s] + x1[s]; // 要素毎の加算
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Add2, bool)
        NBLA_INSTANTIATE_CLASS(Add2, char)
        NBLA_INSTANTIATE_CLASS(Add2, unsigned char)
        NBLA_INSTANTIATE_CLASS(Add2, short)
        NBLA_INSTANTIATE_CLASS(Add2, unsigned short)
        NBLA_INSTANTIATE_CLASS(Add2, int)
        NBLA_INSTANTIATE_CLASS(Add2, unsigned int)
        NBLA_INSTANTIATE_CLASS(Add2, long)
        NBLA_INSTANTIATE_CLASS(Add2, unsigned long)
        NBLA_INSTANTIATE_CLASS(Add2, long long)
        NBLA_INSTANTIATE_CLASS(Add2, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Add2, float)
        NBLA_INSTANTIATE_CLASS(Add2, double)
        NBLA_INSTANTIATE_CLASS(Add2, long double)
    }
}