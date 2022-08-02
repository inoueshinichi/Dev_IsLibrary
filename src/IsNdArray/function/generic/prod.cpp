#include <IsNdArray/function/prod.hpp>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Prod, const vector<int64_t> &, bool)


        template <typename T>
        void Prod<T>::execute_impl_reduce(const T* x, T* y, int outer_size, int reduction_size) 
        {
            for (int o = 0; o < outer_size; ++o) 
            {
                T& val = y[o];
                val = 1;
                for (int i = 0; i < reduction_size; ++i)
                {
                    val *= x[o * reduction_size + i];
                }
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Prod, bool)
        NBLA_INSTANTIATE_CLASS(Prod, char)
        NBLA_INSTANTIATE_CLASS(Prod, unsigned char)
        NBLA_INSTANTIATE_CLASS(Prod, short)
        NBLA_INSTANTIATE_CLASS(Prod, unsigned short)
        NBLA_INSTANTIATE_CLASS(Prod, int)
        NBLA_INSTANTIATE_CLASS(Prod, unsigned int)
        NBLA_INSTANTIATE_CLASS(Prod, long)
        NBLA_INSTANTIATE_CLASS(Prod, unsigned long)
        NBLA_INSTANTIATE_CLASS(Prod, long long)
        NBLA_INSTANTIATE_CLASS(Prod, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Prod, float)
        NBLA_INSTANTIATE_CLASS(Prod, double)
        NBLA_INSTANTIATE_CLASS(Prod, long double)
    }
}