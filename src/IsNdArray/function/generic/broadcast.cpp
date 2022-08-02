#include <IsNdArray/array.hpp>
#include <IsNdArray/function/broadcast.hpp>
#include <IsNdArray/nd_array.hpp>


#include <cstring>
#include <iostream>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Broadcast, const vector<int64_t> &)

        template <typename T>
        void Broadcast<T>::setup_impl(const NdArrays& inputs,
                                      const NdArrays& outputs)
        {
            auto ndim = inputs[0]->ndim();

            // 動的な入力なのでチェック
            if (ndim > 0)
            {
                NBLA_CHECK(shape_.size() == static_cast<unsigned>(ndim),
                           error_code::value,
                           "Number of dimension must match. "
                           "Shape: %d != input: %d.",
                           shape_.size(), ndim);
            }

            // X stride and Y shape.
            stride_x_.reshape({ ndim }, true);
            shape_y_.reshape({ ndim }, true);
            Context cpu = Context().set_array_class("CpuCachedArray");
            // Context cpu = Context().set_array_class("CpuArray");

            int* stride_x = stride_x_.cast_data_and_get_pointer<int>(cpu, true);
            int* shape_y = shape_y_.cast_data_and_get_pointer<int>(cpu, true);
            auto stride_x_in = inputs[0]->strides();

            // Check shape, and store ndarrays.
            auto eshape = expand_shape(inputs[0]->shape(), shape_.size());
            auto estride_x_in = get_c_contiguous_strides(eshape);
            for (unsigned int d = 0; d < eshape.size(); ++d) {
                NBLA_CHECK(eshape[d] == shape_[d] || eshape[d] == 1, error_code::value,
                        "Trailing shapes must be same or dimension of trailing shape of "
                        "input has 1. Input shpae = (%s), Target shape = (%s)",
                        string_join(inputs[0]->shape(), string(", ")).c_str(),
                        string_join(shape_, string(", ")).c_str());

                shape_y[d] = shape_[d];
                if (eshape[d] == shape_[d]) {
                    stride_x[d] = estride_x_in[d];
                    continue;
                }
                stride_x[d] = 0;
            }
            Shape_t outshape(shape_.begin(), shape_.end());
            outputs[0]->reshape(outshape, true);
        }

        // --------------------------------------------------------------------------
        // Strided index getter
        // --------------------------------------------------------------------------
        template <int ND>
        struct strided_index
        {
            static int get(int y_index, const int* stride_x, const int* shape_y)
            {
                int stride = 1;
                int x_index = 0;
                strided_index<ND - 1>::_get(y_index, stride_x, shape_y, stride, x_index);
                return x_index;
            }

            static void _get(int y_index, const int* stride_x, const int* shape_y, int& stride, int& x_index)
            {
                const int dim_index = int(y_index / stride) % shape_y[ND];
                stride *= shape_y[ND];
                x_index += dim_index * stride_x[ND];
                strided_index<ND - 1>::_get(y_index, stride_x, shape_y, stride, x_index);
            }
        };

        // Base condition
        template <>
        struct strided_index<0>
        {
            static int get(int y_index, const int* stride_x, const int* shape_y)
            {
                return 0;
            }

            static void _get(int y_index, const int* stride_x, const int* shape_y, int& stride, int& x_index)
            {
                const int dim_index = int(y_index / stride) % shape_y[0];
                stride *= shape_y[0];
                x_index += dim_index * stride_x[0];
            }
        };


        // --------------------------------------------------------------------------
        // Broadcast all elements
        // --------------------------------------------------------------------------
        template <int Ndim, typename T>
        void broadcast(size_t size, const T* x, const int* stride_x, const int* shape_y, T* y)
        {
            for (size_t i = 0; i < size; ++i)
            {
                int j = strided_index<Ndim>::get(i, stride_x, shape_y);
                y[i] = x[j];
                // y[i] = 9;
            }
        }

        // --------------------------------------------------------------------------
        // Unrolled broadcast caller for templated dimension
        // --------------------------------------------------------------------------
        template <int ND, typename T>
        struct switch_broadcast
        {
            static void call(int num, size_t size, const T* x, const int* stride_x, const int* shape_y, T* y)
            {
                if (ND == num)
                {
                    return broadcast<ND, T>(size, x, stride_x, shape_y, y);
                }
                switch_broadcast<ND - 1, T>::call(num, size, x, stride_x, shape_y, y);
            }
        };

        // Base condition
        template <typename T>
        struct switch_broadcast<-1, T>
        {
            static void call(int num, size_t size, const T* x, const int* stride_x, const int* shape_y, T* y)
            {
                NBLA_ERROR(error_code::not_implemented,
                           "Broadcast is not implemented for %d dimensional array.",
                           num);
            }
        };


        // --------------------------------------------------------------------------
        // Execute
        // --------------------------------------------------------------------------
        template <typename T>
        void Broadcast<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            
            const int* stride_x = stride_x_.get_data_pointer<int>(this->ctx_);
            const int* shape_y = shape_y_.get_data_pointer<int>(this->ctx_);
            int ndim = inputs[0]->ndim();
            int size = outputs[0]->size();

            switch_broadcast<NBLA_BROADCAST_MAX_DIM, T>::call(ndim, size, x, stride_x, shape_y, y);
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Broadcast, bool)
        NBLA_INSTANTIATE_CLASS(Broadcast, char)
        NBLA_INSTANTIATE_CLASS(Broadcast, unsigned char)
        NBLA_INSTANTIATE_CLASS(Broadcast, short)
        NBLA_INSTANTIATE_CLASS(Broadcast, unsigned short)
        NBLA_INSTANTIATE_CLASS(Broadcast, int)
        NBLA_INSTANTIATE_CLASS(Broadcast, unsigned int)
        NBLA_INSTANTIATE_CLASS(Broadcast, long)
        NBLA_INSTANTIATE_CLASS(Broadcast, unsigned long)
        NBLA_INSTANTIATE_CLASS(Broadcast, long long)
        NBLA_INSTANTIATE_CLASS(Broadcast, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Broadcast, float)
        NBLA_INSTANTIATE_CLASS(Broadcast, double)
        NBLA_INSTANTIATE_CLASS(Broadcast, long double)
    }}