#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Arange, float, float, float)
        // start
        // stop
        // step


        /**
         * @brief Generate a range of values within the half-open interval 
         * `[start, stop)`` (the interval including start but excluding stop) with `step` increments.
         * 
         * Inputs:
         * - None
         * 
         * Outputs:
         * - 1-D Array.
         * 
         * @param T Data type for computation.
         * @param start Start value.
         * @param stop End value.
         * @param step Step value.
         */
        template <typename T>
        class Arange : public BaseFunction<float, float, float>
        {
        protected:
            float start_;
            float stop_;
            float step_;
        public:
            Arange(const Context &ctx, float start, float stop, float step)
                : BaseFunction(ctx, start, stop, step)
                , start_(start)
                , stop_(stop)
                , step_(step) {}

            virtual ~Arange() {}
            virtual shared_ptr<Function> copy() const 
            {
                return create_Arange(ctx_, start_, stop_, step_);
            }

            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual vector<dtypes> in_types() { return vector<dtypes>{}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual string name() { return "Arange"; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}