#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <algorithm>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Reshape, const vector<int64_t>&, bool)
        // shape
        // inplace

        /**
         * @brief Reshaping copy of an input variable.
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param shape Target shape.
         * @param inplace The output array is will be shared with the input array if true.
         */
        template <typename T>
        class Reshape : public BaseFunction<const vector<int64_t>&, bool> 
        {
        protected:
            Shape_t shape_;
            bool inplace_;
        
        public:
            Reshape(const Context& ctx, const vector<int64_t>& shape, bool inplace)
                : BaseFunction(ctx, shape, inplace)
                , shape_(shape.size())
                , inplace_(inplace) 
            {
                std::copy(shape.begin(), shape.end(), shape_.begin());
            }

            virtual ~Reshape() {}
            virtual shared_ptr<Function> copy() const 
            {
                vector<int64_t> shape(shape_.size());
                std::copy(shape_.begin(), shape_.end(), shape.begin());
                return create_Reshape(ctx_, shape, inplace_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Reshape"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

            virtual int inplace_data(int i) const 
            {
                return inplace_ ? Function::INPLACE_NOT_MODIFY : Function::NOT_INPLACE;
            }

            virtual int inplace_data_with(int i) const { return 0; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}