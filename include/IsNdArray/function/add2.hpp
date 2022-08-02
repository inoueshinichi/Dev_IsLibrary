#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Add2, bool)

        /**
         * @brief Elementwise Add
         * y_i = x0_i + x1_i
         * 
         * Inputs:
         *   - N-D array.
         *   - N-D array.
         * Outputs:
         *   - N-D array.
         */
        template <typename T>
        class Add2 : public BaseFunction<bool>
        {
        protected:
        public:
            Add2(const Context& ctx, bool inplace)
                : BaseFunction(ctx, inplace) {}

            virtual ~Add2() {}

            virtual shared_ptr<Function> copy() const { return create_Add2(ctx_, false /*inplace is obsoleted. */); }
            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>(), get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 2; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Add2"; }

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