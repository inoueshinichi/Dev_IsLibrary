#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(CumProd, int, bool, bool)
        // axis
        // exclusive
        // reverse

        /**
         * @brief Performs cumulative product of a given array along a specific axis.
         * 
         * Given x = [x1, x2, x3]
         * y = [x1, x1*x2, x1*x2*x3] if not exclusive and not reverse
         * y = [1, x1, x1*x2] if exclusive and not reverse
         * y = [x1*x2*x3, x2*x3, x3] if not exclusive and reverse
         * y = [x2*x3, x3, 1] if exclusive and reverse
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param axis Axis along which to cumsum.
         * @param exclusive
         * @param reverse Sort in descending order.
         */
        template <typename T> 
        class CumProd : public BaseFunction<int, bool, bool> 
        {
        protected:
            int axis_;
            bool exclusive_;
            bool reverse_;
            Size_t size_, size0_, size1_, size2_;

        public:
            CumProd(const Context& ctx, int axis, bool exclusive, bool reverse)
                : BaseFunction(ctx, axis, exclusive, reverse)
                , axis_(axis)
                , exclusive_(exclusive)
                , reverse_(reverse) {}
            
            virtual ~CumProd() {}
            virtual shared_ptr<Function> copy() const 
            {
                return create_CumProd(ctx_, axis_, exclusive_, reverse_);
            }

            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual string name() { return "CumProd"; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}