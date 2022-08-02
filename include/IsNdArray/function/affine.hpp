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
        NBLA_REGISTER_FUNCTION_HEADER(Affine, int)
        // base_axis

        /**
         * @brief Affine also called as fully connected layer
         * 
         * mat_y = mat_A @ mat_x + mat_b
         * 
         * Inputs: (B is base_axis)
         * - N-D array with shape (M_0, M_1, ..., M_(B-1), D_B, ..., D_N).
         * Dimensions before and after base_axis are flattened as if it is a matrix.
         * - Weight matrix with shape (D_B, ..., D_N)
         * - (optional) Bias vector (L)
         * 
         * Outputs:
         * - (B+1)-D array. (M_0, ..., M_(B-1), L)
         * 
         * @param T Data type for computation.
         * @param base_axis Base axis of Affine operation. Dimensions up to base_axis is treated as sample dimension.
         */
        template <typename T>
        class Affine : public BaseFunction<int>
        {
        protected:
            int base_axis_;
            Size_t i_row_, i_col_, w_row_, w_col_, o_row_, o_col_;

        public:
             Affine(const Context& ctx, int base_axis)
                : BaseFunction(ctx, base_axis)
                , base_axis_(base_axis) {}
            
            virtual ~Affine() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Affine(ctx_, base_axis_);
            }

            virtual vector<dtypes> in_types() 
            {
                return vector<dtypes>{get_dtype<T>(), get_dtype<T>(), get_dtype<T>()};
            }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }

            virtual int min_inputs() { return 2; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Affine"; }
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