#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Where)


        /**
         * @brief Return elements, either from x_true or x_false, depending on condition.
         * 
         * Inputs:
         * - N-D array. (condition)
         * - N-D array. (x_true)
         * - N-D array. (x_false)
         * When condition[i] is true, yield x_true[i], otherwise x_false[i] for all i.
         * 
         * Outputs:
         * - N-D array.
         * 
         */
        template <typename T>
        class Where : public BaseFunction<>
        {
        public:
            Where(const Context& ctx)
                : BaseFunction(ctx) {}

            virtual ~Where() {}

            virtual shared_ptr<Function> copy() const { return create_Where(ctx_); }

            virtual int min_inputs() { return 3; }
            virtual int min_outputs() { return 1; }
            virtual vector<dtypes> in_types() 
            {
              return vector<dtypes>{get_dtype<T>(), get_dtype<T>(), get_dtype<T>()};
            }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual string name() { return "Where"; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}