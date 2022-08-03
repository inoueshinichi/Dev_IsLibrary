#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/function/sort.hpp>
#include <IsNdArray/function/transpose.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>
#include <numeric>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Sort, int, bool, bool, bool)
        // axis
        // reverse
        // with_index
        // only_index

        template <typename T>
        void Sort<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (this->axis_ > 0) 
            {
                NBLA_CHECK(static_cast<unsigned>(this->axis_) < inputs[0]->shape().size(),
                           error_code::value,
                           "Sort axis must be less than the number of input dimensions "
                           "but axis %d >= ndim of x %d.",
                           this->axis_, 
                           inputs[0]->shape().size());
            }
            
            if (this->axis_ < 0) 
            {
                NBLA_CHECK(inputs[0]->shape().size() + this->axis_ >= 0, 
                           error_code::value,
                           "Negative sort axis must not be less than -%d dimensions of "
                           "input variable, but got axis %d.",
                           inputs[0]->shape().size(), 
                           inputs[0]->shape().size(),
                           this->axis_);

               this->axis_ += inputs[0]->shape().size();
            }


            const auto& shape = inputs[0]->shape();

            this->inner_size_ = 1;
            for (int i = shape.size() - 1; i > this->axis_; --i)
            {
                this->inner_size_ *= shape[i];
            }

            this->outer_size_ = this->inner_size_ * shape[this->axis_];

            this->total_size_ = this->outer_size_;
            for (int i = this->axis_ - 1; i >= 0; --i)
            {
                this->total_size_ *= shape[i];
            }
                
            this->sort_index_.reshape(shape, true);
            this->temp_index_.reshape(Shape_t{shape[this->axis_]}, true);

            outputs[0]->reshape(shape, true);
            if (this->with_index_ && !this->only_index_)
            {
                outputs[1]->reshape(shape, true);
            }
        }


        template <typename T>
        void Sort<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const auto &shape = inputs[0]->shape();
            NdArray& sort_index_arr = this->sort_index_;
            NdArray& temp_index_arr = this->temp_index_;

            auto sort_index_ptr = sort_index_arr.cast_data_and_get_pointer<size_t>(ctx_);
            auto temp_index_ptr = temp_index_arr.cast_data_and_get_pointer<size_t>(ctx_);
            auto x_data = inputs[0]->get_data_pointer<T>(ctx_);

            auto outer_x_ptr = x_data;
            auto outer_i_ptr = sort_index_ptr;
            auto stride = this->inner_size_;

            while (outer_x_ptr < x_data + this->total_size_)
            {
                auto inner_x_ptr = outer_x_ptr;
                auto inner_i_ptr = outer_i_ptr;

                while (inner_x_ptr < outer_x_ptr + this->inner_size_)
                {
                    std::iota(temp_index_ptr, temp_index_ptr + temp_index_arr.size(), 0); // 0, 1, 2, ...
                    auto x = inner_x_ptr; // shorter name for compare function
                    auto s = stride;      // shorter name for compare function

                    if (this->reverse_)
                    {
                        std::sort(temp_index_ptr, temp_index_ptr + temp_index_arr.size(),
                                  [&](size_t i1, size_t i2) { return x[i1 * s] > x[i2 * s]; });
                    }
                    else
                    {
                        std::sort(temp_index_ptr, temp_index_ptr + temp_index_arr.size(),
                                  [&](size_t i1, size_t i2) { return x[i1 * s] < x[i2 * s]; });
                    }

                    for (size_t i = 0; i < static_cast<size_t>(shape[this->axis_]); ++i)
                    {
                        inner_i_ptr[i * stride] = temp_index_ptr[i];
                    }
                    inner_x_ptr++;
                    inner_i_ptr++;
                } // while
                outer_x_ptr += this->outer_size_;
                outer_i_ptr += this->outer_size_;
            } // while


            if (!this->only_index_) 
            {
                auto y_data = outputs[0]->cast_data_and_get_pointer<T>(ctx_, true);
                auto outer_x_ptr = x_data;
                auto outer_y_ptr = y_data;
                auto outer_i_ptr = sort_index_ptr;
                auto stride = this->inner_size_;

                while (outer_x_ptr < x_data + this->total_size_) 
                {
                    auto inner_x_ptr = outer_x_ptr;
                    auto inner_y_ptr = outer_y_ptr;
                    auto inner_i_ptr = outer_i_ptr;

                    while (inner_x_ptr < outer_x_ptr + this->inner_size_) 
                    {
                        for (size_t i = 0; i < static_cast<size_t>(shape[this->axis_]); ++i) 
                        {
                            const auto sort_index = inner_i_ptr[i * stride];
                            inner_y_ptr[i * stride] = inner_x_ptr[sort_index * stride];
                        }
                        inner_x_ptr++;
                        inner_y_ptr++;
                        inner_i_ptr++;
                    }
                    outer_x_ptr += this->outer_size_;
                    outer_y_ptr += this->outer_size_;
                    outer_i_ptr += this->outer_size_;
                }
            } // if


            if (this->with_index_ || this->only_index_) 
            {
                auto out_sp_arr = this->only_index_ ? outputs[0] : outputs[1];
                auto out_ptr = out_sp_arr-> template cast_data_and_get_pointer<size_t>(ctx_, true);
                std::copy_n(sort_index_ptr, out_sp_arr->size(), out_ptr);
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Sort, bool)
        NBLA_INSTANTIATE_CLASS(Sort, char)
        NBLA_INSTANTIATE_CLASS(Sort, unsigned char)
        NBLA_INSTANTIATE_CLASS(Sort, short)
        NBLA_INSTANTIATE_CLASS(Sort, unsigned short)
        NBLA_INSTANTIATE_CLASS(Sort, int)
        NBLA_INSTANTIATE_CLASS(Sort, unsigned int)
        NBLA_INSTANTIATE_CLASS(Sort, long)
        NBLA_INSTANTIATE_CLASS(Sort, unsigned long)
        NBLA_INSTANTIATE_CLASS(Sort, long long)
        NBLA_INSTANTIATE_CLASS(Sort, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Sort, float)
        NBLA_INSTANTIATE_CLASS(Sort, double)
        NBLA_INSTANTIATE_CLASS(Sort, long double)
    }
}