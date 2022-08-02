#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Stack, int)
        // axis


        /**
         * @brief Stack joins two or more arrays on a new axis.
         * 
         * The sizes of all the arrays to be stacked must be the same. 
         * Unlike Concatenate, which joins arrays on an existing axis,
         * Stack joins arrays on a new axis.
         * 
         * Inputs:
         * - list of N-D arrays.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param axis The axis on which to concatenate arrays.
         * Axis indexes take on values 0, 1, 2, and so on from the left.
         * For example, to stack four (3,28,28) inputs on the second axis, specify 1.
         * In this case, the output size will be (3,4,28,28).
         * 
         */
        template <typename T> 
        class Stack : public BaseFunction<int> 
        {
        protected:
            int axis_;
            int num_inputs_;
            int inner_size_, outer_size_;
        public:
            Stack(const Context &ctx, int axis) 
                : BaseFunction(ctx, axis)
                , axis_(axis) {}

            virtual ~Stack() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Stack(ctx_, axis_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Stack"; }
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