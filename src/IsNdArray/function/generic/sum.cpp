#include <IsNdArray/array.hpp>
#include <IsNdArray/function/transpose.hpp>
#include <IsNdArray/imperative.hpp>
#include <IsNdArray/singleton_manager.hpp>
#include <IsNdArray/function/sum.hpp>
#include <IsNdArray/utils/eigen.hpp>
#include <IsNdArray/nd_array.hpp>

#include <algorithm>
#include <numeric> // iota


namespace is
{
    namespace nbla
    {
        using namespace ::is::nbla::eigen;

        NBLA_REGISTER_FUNCTION_SOURCE(Sum, const vector<int64_t>&, bool)

        template <typename T>
        void Sum<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            int ndim = inputs[0]->ndim();
            auto inshape = inputs[0]->shape();
            // =========
            // Transpose
            // =========
            // get transpose axes.
            // Create transpose axes and reduction size
            vector<int64_t> transpose_axes;
            int prev_a = -1;
            reduction_size_ = 1;
            Shape_t outshape;
            for (int a : axes_)
            {
                if (a < 0)
                {
                    a += inshape.size();
                }
                NBLA_CHECK(a < ndim && a >= 0, 
                           error_code::value,
                           "Axes out of range. 0 <= %d < %d", 
                           a, 
                           ndim);
                for (int b = prev_a + 1; b < a; ++b) 
                {
                    transpose_axes.push_back(b);
                    outshape.push_back(inshape[b]);
                }

                if (keep_dims_) 
                {
                    outshape.push_back(1);
                }
                reduction_size_ *= inshape[a];
                prev_a = a;
            } // for (int a : axes_)


            for (int a = prev_a + 1; a < ndim; ++a) 
            {
                transpose_axes.push_back(a);
                outshape.push_back(inshape[a]);
            }

            std::copy(axes_.begin(), axes_.end(), std::back_inserter(transpose_axes));

            // Sequence of numbers [0, ndim)
            vector<int64_t> seq(ndim);
            std::iota(seq.begin(), seq.end(), 0);

            if (transpose_axes != seq) 
            {
                // Need transpose
                // f_transpose_ = create_Transpose(this->ctx_, transpose_axes);
                f_transpose_ = std::make_shared<Transpose<T>>(this->ctx_, transpose_axes);
            }
            outputs[0]->reshape(outshape, true);
        }


        template <typename T>
        void Sum<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const int outer_size = inputs[0]->size() / reduction_size_;
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);

            if (f_transpose_) 
            {
                // For not overwriting the memory region of the transpose results,
                // call transpose and sum with i_transpose in the same scope.
                NdArrayPtr i_transpose = NdArray::create();
                NdArrays outputs { i_transpose };
                imp::execute(f_transpose_, inputs, outputs);
                const T* x_T = i_transpose->get_data_pointer<T>(this->ctx_);
                this->execute_impl_reduce(x_T, y, outer_size, reduction_size_);
            }
            else
            {
                const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
                this->execute_impl_reduce(x, y, outer_size, reduction_size_);
            }
        }


        template <typename T>
        void Sum<T>::execute_impl_reduce(const T* x, T* y, 
                                         int outer_size,
                                         int reduction_size) 
        {
            using namespace ::is::nbla::eigen;
            ConstMatrixMap<T> mx(x, outer_size, reduction_size);
            ColVectorMap<T> my(y, outer_size);
            my = mx.rowwise().sum();
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */

        NBLA_INSTANTIATE_CLASS(Sum, bool)
        NBLA_INSTANTIATE_CLASS(Sum, char)
        NBLA_INSTANTIATE_CLASS(Sum, unsigned char)
        NBLA_INSTANTIATE_CLASS(Sum, short)
        NBLA_INSTANTIATE_CLASS(Sum, unsigned short)
        NBLA_INSTANTIATE_CLASS(Sum, int)
        NBLA_INSTANTIATE_CLASS(Sum, unsigned int)
        NBLA_INSTANTIATE_CLASS(Sum, long)
        NBLA_INSTANTIATE_CLASS(Sum, unsigned long)
        NBLA_INSTANTIATE_CLASS(Sum, long long)
        NBLA_INSTANTIATE_CLASS(Sum, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Sum, float)
        NBLA_INSTANTIATE_CLASS(Sum, double)
        NBLA_INSTANTIATE_CLASS(Sum, long double)
    }
}