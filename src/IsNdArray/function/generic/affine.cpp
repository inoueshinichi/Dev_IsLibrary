#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/function/affine.hpp>
#include <IsNdArray/utils/eigen.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Affine, int)
        // base_axis

        template <typename T>
        void Affine<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Shape_t shape_data = inputs[0]->shape();
            Shape_t shape_weights = inputs[1]->shape();

            NBLA_CHECK(shape_weights.size() >= 2, 
                       error_code::value,
                       "Weights(inputs[1]) must be matrix or tensor.");

            NBLA_CHECK(base_axis_ >= 0, 
                       error_code::value,
                       "base_axis must be a positive number, got %d", 
                       base_axis_);

            auto base_axis = static_cast<Shape_t::size_type>(this->base_axis_);

            NBLA_CHECK(base_axis < shape_data.size(), 
                       error_code::value,
                       "Base_axis must be less than ndim of input data(inputs[0]). "
                       "base_axis: %d >= ndim of input: %d.",
                       base_axis_, 
                       shape_data.size());
            
            NBLA_CHECK(inputs[0]->size(base_axis_) == shape_weights[0], 
                       error_code::value,
                       "Size of input data(inputs[0]) and weights(inputs[1]) mismatch. "
                       "size of input: %d != size of weights: %d.",
                       inputs[0]->size(base_axis_), 
                       shape_weights[0]);
            
            i_col_ = inputs[0]->size(base_axis_);
            i_row_ = inputs[0]->size() / i_col_;
            w_row_ = shape_weights[0];
            w_col_ = inputs[1]->size() / w_row_;
            o_row_ = i_row_;
            o_col_ = w_col_;
            Shape_t shape_out;

            for (int i = 0; i < base_axis_; ++i) 
            {
                shape_out.push_back(shape_data[i]);
            }

            for (Shape_t::size_type i = 1; i < shape_weights.size(); ++i) 
            {
                shape_out.push_back(shape_weights[i]);
            }
            outputs[0]->reshape(shape_out, true);

            NBLA_CHECK(i_col_ == w_row_, error_code::value, "Shape mismatch.");
            NBLA_CHECK(outputs[0]->size() == o_row_ * o_col_, 
                       error_code::value,
                       "Shape mismatch. %ld %ld %ld", 
                       outputs[0]->size(), 
                       o_row_, 
                       o_col_);
            
            if (inputs.size() == 3) 
            {
                // With bias
                Shape_t shape_bias = inputs[2]->shape();
                NBLA_CHECK(shape_bias.size() == shape_weights.size() - 1, 
                           error_code::value,
                           "Length of bias(inputs[2]) and weights(inputs[1]) mismatch. "
                           "bias length: %d != weights length-1: %d.",
                           shape_bias.size(), 
                           shape_weights.size() - 1);

                for (Shape_t::size_type i = 0; i < shape_bias.size(); ++i) 
                {
                    NBLA_CHECK(shape_bias[i] == shape_weights[i + 1], 
                            error_code::value,
                            "Shape of bias(inputs[2]) and weights(inputs[1]) mismatch. "
                            "shape_bias[%d]: %d != shape_weights[%d + 1]: %d.",
                            i, 
                            shape_bias[i], 
                            i, 
                            shape_weights[i + 1]);
                        
                }
            }
        }


        template <typename T>
        void Affine<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            using namespace ::is::nbla::eigen;
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            const T* w = inputs[1]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            ConstMatrixMap<T> mx(x, i_row_, i_col_);
            ConstMatrixMap<T> mw(w, w_row_, w_col_);
            MatrixMap<T> my(y, o_row_, o_col_);
            my = mx * mw;

            if (inputs.size() == 3) 
            {
                // With bias
                const T* b = inputs[2]->get_data_pointer<T>(this->ctx_);
                my.rowwise() += ConstRowVectorMap<T>(b, o_col_);
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Affine, bool)
        NBLA_INSTANTIATE_CLASS(Affine, char)
        NBLA_INSTANTIATE_CLASS(Affine, unsigned char)
        NBLA_INSTANTIATE_CLASS(Affine, short)
        NBLA_INSTANTIATE_CLASS(Affine, unsigned short)
        NBLA_INSTANTIATE_CLASS(Affine, int)
        NBLA_INSTANTIATE_CLASS(Affine, unsigned int)
        NBLA_INSTANTIATE_CLASS(Affine, long)
        NBLA_INSTANTIATE_CLASS(Affine, unsigned long)
        NBLA_INSTANTIATE_CLASS(Affine, long long)
        NBLA_INSTANTIATE_CLASS(Affine, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Affine, float)
        NBLA_INSTANTIATE_CLASS(Affine, double)
        NBLA_INSTANTIATE_CLASS(Affine, long double)
    }
}