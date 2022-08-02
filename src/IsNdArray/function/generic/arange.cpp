#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/function/arange.hpp>
#include <IsNdArray/nd_array.hpp>

#include <limits>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Arange, float, float, float)
        // start
        // stop
        // step

        template <typename T>
        void Arange<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            NBLA_CHECK(this->step_ != 0, 
                       error_code::value,
                       "step argument must not be zero");

            Size_t count = 0;
            if (step_ < 0) 
            {
                for (float value = start_; value > stop_; value += step_)
                {
                    count++;
                }
            } 
            else 
            {
                for (float value = start_; value < stop_; value += step_)
                {
                    count++;
                }
            }

            outputs[0]->reshape(Shape_t{count}, true);
        }


        template <typename T>
        void Arange<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            auto& y = outputs[0];
            T *y_data = y->cast_data_and_get_pointer<T>(this->ctx_, true);
            auto value = this->start_;

            for (Size_t i = 0; i < y->size(); ++i) 
            {
                y_data[i] = value;
                value += this->step_;
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        // NBLA_INSTANTIATE_CLASS(Arange, bool)
        // NBLA_INSTANTIATE_CLASS(Arange, char)
        // NBLA_INSTANTIATE_CLASS(Arange, unsigned char)
        // NBLA_INSTANTIATE_CLASS(Arange, short)
        // NBLA_INSTANTIATE_CLASS(Arange, unsigned short)
        // NBLA_INSTANTIATE_CLASS(Arange, int)
        // NBLA_INSTANTIATE_CLASS(Arange, unsigned int)
        // NBLA_INSTANTIATE_CLASS(Arange, long)
        // NBLA_INSTANTIATE_CLASS(Arange, unsigned long)
        // NBLA_INSTANTIATE_CLASS(Arange, long long)
        // NBLA_INSTANTIATE_CLASS(Arange, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Arange, float)
        NBLA_INSTANTIATE_CLASS(Arange, double)
        NBLA_INSTANTIATE_CLASS(Arange, long double)
    }
}