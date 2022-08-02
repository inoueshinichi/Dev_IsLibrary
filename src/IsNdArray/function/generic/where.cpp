#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/function/where.hpp>
#include <IsNdArray/nd_array.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Where)

        template <typename T>
        void Where<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            auto cond_shape = inputs[0]->shape();
            auto true_shape = inputs[1]->shape();
            auto false_shape = inputs[2]->shape();

            NBLA_CHECK(true_shape == false_shape, 
                       error_code::value,
                       "x_true and x_false must have same dimensions.");

            NBLA_CHECK(cond_shape.size() <= true_shape.size(), 
                       error_code::value,
                       "Rank of condition must be less than or equal to that of x_true or x_false.");

            for (Shape_t::size_type d = 0; d < cond_shape.size(); ++d)
            {
                NBLA_CHECK(cond_shape[d] == true_shape[d], 
                           error_code::value,
                           "The first dimensions of x_true and x_false must be the same as "
                           "the shape of condition.");
            }
            outputs[0]->reshape(true_shape, true);
        }

        template <typename T>
        void Where<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* condition = inputs[0]->get_data_pointer<T>(this->ctx_);
            const T* x_true = inputs[1]->get_data_pointer<T>(this->ctx_);
            const T* x_false = inputs[2]->get_data_pointer<T>(this->ctx_);

            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);

            size_t csize = inputs[0]->size();
            size_t xsize = inputs[1]->size();
            size_t inner_size = xsize / csize;

            for (auto s = decltype(xsize){0}; s < xsize; ++s) 
            {
                auto c = s / inner_size;
                y[s] = condition[c] ? x_true[s] : x_false[s];
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Where, bool)
        NBLA_INSTANTIATE_CLASS(Where, char)
        NBLA_INSTANTIATE_CLASS(Where, unsigned char)
        NBLA_INSTANTIATE_CLASS(Where, short)
        NBLA_INSTANTIATE_CLASS(Where, unsigned short)
        NBLA_INSTANTIATE_CLASS(Where, int)
        NBLA_INSTANTIATE_CLASS(Where, unsigned int)
        NBLA_INSTANTIATE_CLASS(Where, long)
        NBLA_INSTANTIATE_CLASS(Where, unsigned long)
        NBLA_INSTANTIATE_CLASS(Where, long long)
        NBLA_INSTANTIATE_CLASS(Where, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Where, float)
        NBLA_INSTANTIATE_CLASS(Where, double)
        NBLA_INSTANTIATE_CLASS(Where, long double)
    }
}