#include <IsNdArray/function/mean.hpp>
#include <IsNdArray/utils/eigen.hpp>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Mean, const vector<int64_t>&, bool)

        template <typename T>
        void Mean<T>::execute_impl_reduce(const T *x, T *y, 
                                          int outer_size,
                                          int reduction_size) 
        {
            using namespace ::is::nbla::eigen;
            ConstMatrixMap<T> mx(x, outer_size, reduction_size);
            ColVectorMap<T> my(y, outer_size);
            my = mx.rowwise().sum() / reduction_size;
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Mean, bool)
        NBLA_INSTANTIATE_CLASS(Mean, char)
        NBLA_INSTANTIATE_CLASS(Mean, unsigned char)
        NBLA_INSTANTIATE_CLASS(Mean, short)
        NBLA_INSTANTIATE_CLASS(Mean, unsigned short)
        NBLA_INSTANTIATE_CLASS(Mean, int)
        NBLA_INSTANTIATE_CLASS(Mean, unsigned int)
        NBLA_INSTANTIATE_CLASS(Mean, long)
        NBLA_INSTANTIATE_CLASS(Mean, unsigned long)
        NBLA_INSTANTIATE_CLASS(Mean, long long)
        NBLA_INSTANTIATE_CLASS(Mean, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Mean, float)
        NBLA_INSTANTIATE_CLASS(Mean, double)
        NBLA_INSTANTIATE_CLASS(Mean, long double)
    }
}