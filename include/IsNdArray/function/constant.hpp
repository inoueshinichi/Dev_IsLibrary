#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Constant, float, const vector<int>&)
        // value
        // shape

        /**
         * @brief 
         * 
         * Inputs:
         * None
         * 
         * Outputs:
         * - N-D array.
         * 
         */
        template <typename T>
        class Constant : public BaseFunction<float, const vector<int>&>
        {
        protected:
            float val_;
            const vector<int> shape_;

        public:
            Constant(const Context& ctx, float val, const vector<int>& shape)
                : BaseFunction<float, const vector<int>&>(ctx, val, shape)
                , val_(val)
                , shape_(shape) {}
            
            virtual ~Constant() {}
            virtual shared_ptr<Function> copy() const 
            {
                return create_Constant(ctx_, val_, shape_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>(); }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Constant"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            };

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}