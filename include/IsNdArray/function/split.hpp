#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Split, int)
        // axis

        /**
         * @brief Split arrays at axis.
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * outputs must have the same shapes.
         * - list of N-D array.
         * 
         * @param T Data type for computation.
         * @param axis Axis along which to split.
         * 
         */
        template <typename T>
        class Split : public BaseFunction<int>
        {
        protected:
            int axis_;
            int num_outputs_;
            int inner_size_, outer_size_;
        public:
            Split(const Context& ctx, int axis)
                : BaseFunction(ctx, axis)
                , axis_(axis) {}
            
            virtual ~Split() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Split(ctx_, axis_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 2; }
            virtual string name() { return "Split"; }
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