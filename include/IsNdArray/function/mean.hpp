#pragma once

#include <IsNdArray/function/sum.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Mean, const vector<int64_t> &, bool)
        // axes
        // keep_dims

        /**
         * @brief Reduction along axes with mean operation.
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
         * 
         */
        template <typename T>
        class Mean : public Sum<T>
        {
        public:
            Mean(const Context& ctx, const vector<int64_t>& axes, bool keep_dims)
                : Sum<T>(ctx, axes, keep_dims) {}
            
            virtual ~Mean() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Mean(this->ctx_, this->axes_, this->keep_dims_);
            }

            virtual string name() { return "Mean"; }

        protected:
            NBLA_API virtual void execute_impl_reduce(const T* x, T* y, 
                                                      int outer_size,
                                                      int reduction_size);
        };
    }
}