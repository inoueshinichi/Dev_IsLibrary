#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <algorithm>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Transpose, const vector<int64_t>&)
        // axes

        /**
         * @brief Transpose
         * The function for tensor dimension conversion
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param axes Transposing index
         * 
         */
        template <typename T>
        class Transpose : public BaseFunction<const vector<int64_t>&>
        {
        protected:
            vector<int64_t> axes_;
            Shape_t x_shape_, x_strides_, x_strides_transposed_;
            Shape_t y_shape_, y_strides_, y_strides_transposed_;

        public:
            Transpose(const Context& ctx, const vector<int64_t>& axes)
                : BaseFunction(ctx, axes)
                , axes_(axes.size())
            {
                std::copy(axes.begin(), axes.end(), axes_.begin());
            }

            virtual ~Transpose() {}

            virtual shared_ptr<Function> copy() const 
            {
                vector<int64_t> axes(axes_.size());
                std::copy(axes_.begin(), axes_.end(), axes.begin());
                return create_Transpose(ctx_, axes);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Transpose"; }
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