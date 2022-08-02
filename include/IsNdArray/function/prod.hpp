#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function/sum.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Prod, const vector<int64_t>&, bool)
        // axes
        // keep_dims

        /**
         * @brief Reduction along axes with product operation.
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
        class Prod : public Sum<T>
        {
        public:
            Prod(const Context& ctx, const vector<int64_t>& axes, bool keep_dims)
                : Sum<T>(ctx, axes, keep_dims) {}
            
            virtual ~Prod() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Prod(this->ctx_, this->axes_, this->keep_dims_);
            }

            virtual string name() { return "Prod"; }

        protected:
            NBLA_API virtual void execute_impl_reduce(const T *x, T *y, 
                                                      int outer_size,
                                                      int reduction_size);
        };
    }
}