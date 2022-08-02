#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <algorithm>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Sum, const vector<int64_t>&, bool)
        // axes
        // keep_dims

        /**
         * @brief Reduction along axes with sum operation.
         * 
         * Inputs:
         * - N-D array
         * 
         * Outputs:
         * - N-D array
         * 
         * @param T Data type for computation.
         * @param axes A list of axes to be reduced.
         * @param keep_dims Flag whether the reduced axes are kept.
         */
        template <typename T>
        class Sum : public BaseFunction<const vector<int64_t>&, bool>
        {
        protected:
            vector<int64_t> axes_;
            bool keep_dims_;
            int reduction_size_;
            shared_ptr<Function> f_transpose_{nullptr};

        public:
            Sum(const Context& ctx, const vector<int64_t>& axes, bool keep_dims)
                : BaseFunction(ctx, axes, keep_dims)
                , axes_(axes)
                , keep_dims_(keep_dims) 
            {
                // Sum実施により1次元のみ加算(Reduction)する場合
                if (axes.size() <= 1)
                {
                    return;
                }
                // Sort axes vector;
                std::sort(axes_.begin(), axes_.end());
            }

            virtual ~Sum() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Sum(ctx_, axes_, keep_dims_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }


            virtual string name() { return "Sum"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);

            NBLA_API virtual void execute_impl_reduce(const T* x, T* y, 
                                                      int outer_size,
                                                      int reduction_size);

        };
    }
}