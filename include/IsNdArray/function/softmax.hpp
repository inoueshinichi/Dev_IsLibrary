#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <memory>
#include <string>


namespace is
{
    namespace nbla
    {
        using std::string;
        using std::make_shared;

        NBLA_REGISTER_FUNCTION_HEADER(Softmax, int)

        /**
         * @brief Softmax
         * 
         * Inputs:
         *   - N-D array.
         * Outputs:
         * - N-D array with the same shape as input.
         * 
         * @tparam T Data type for computation.
         * @param axis Axis normalization is taken.
         */
        template <typename T>
        class Softmax : public BaseFunction<int>
        {
        protected:
            int axis_;
            Size_t size0_, size1_, size2_;
        
        public:
            Softmax(const Context& ctx, int axis)
                : BaseFunction(ctx, axis), axis_(axis) {}
            
            virtual ~Softmax() {}

            virtual shared_ptr<Function> copy() const {
                return create_Softmax(ctx_, axis_);
            }
            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Softmax"; }
            virtual vector<string> allowed_array_classes() {
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