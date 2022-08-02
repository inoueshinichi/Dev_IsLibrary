#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Concatenate, int)

        /**
         * @brief Concatenate a variable number of input arrays along specified axis.
         * Inputs:
         * Input arrays must have the same shapes except specified axis.
         * - N-D array.
         * - ...
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         */
        template <typename T> 
        class Concatenate : public BaseFunction<int> 
        {
        protected:
            int axis_;
            int outer_size_, inner_total_size_;

        public:
            Concatenate(const Context& ctx, int axis)
                : BaseFunction(ctx, axis)
                , axis_(axis) {}
            
            virtual ~Concatenate() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Concatenate(ctx_, axis_);
            }

            virtual vector<dtypes> in_types() 
            {
                return vector<dtypes>{get_dtype<T>(), get_dtype<T>()};
            }

            virtual vector<dtypes> out_types() 
            { 
                return vector<dtypes>{get_dtype<T>()}; 
            }

            virtual int min_inputs() { return 2; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Concatenate"; }

            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}