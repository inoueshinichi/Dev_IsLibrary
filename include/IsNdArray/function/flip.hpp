#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Flip, const vector<int64_t>&)
        // axes

        /**
         * @brief Flip reverses the order of elements of the specified dimension of an array.
         * 
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param axes The index of the dimension to reverse the order of the elements.
         * Axis indexes take on values 0, 1, 2, and so on from the left.
         * For example, to flip a 32 (W) by 24 (H) 100 RGB image 
         * (100,3,24,32) vertically and horizontally, specify (2,3).
         */
        template <typename T> 
        class Flip : public BaseFunction<const vector<int64_t> &> 
        {
        protected:
            vector<int64_t> axes_;
            vector<bool> flip_;
        public:
            Flip(const Context& ctx, const vector<int64_t>& axes)
                : BaseFunction(ctx, axes)
                , axes_(axes.size()) 
            {
                std::copy(axes.begin(), axes.end(), axes_.begin());
            }

            virtual ~Flip() {}
            virtual shared_ptr<Function> copy() const 
            {
                vector<int64_t> axes(axes_.size());
                std::copy(axes_.begin(), axes_.end(), axes.begin());
                return create_Flip(ctx_, axes);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Flip"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

            std::vector<int64_t>& axes() { return axes_; }


        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);

        private:
             void flip_recursive(NdArrayPtr inp, const T* x, T* y,
                      const std::vector<bool>& flip, 
                      bool add, int x_offset,
                      int y_offset, int dim);
        };
    }
}