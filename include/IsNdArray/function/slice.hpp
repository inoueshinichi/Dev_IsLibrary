#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Slice, const vector<int64_t>&, const vector<int64_t>&, const vector<int64_t>&)
        // start
        // stop
        // step

        /**
         * @brief Slice arrays along specified axis.
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - M-D array.
         * 
         * Slice input tensor.
         * y = x[start[0]:stop[0]:step[0], start[1]:stop[1]:step[1], ...]
         * 
         * @param T Data type for computation.
         */
        template <typename T>
        class Slice : public BaseFunction<const vector<int64_t>&, const vector<int64_t>&, const vector<int64_t>&>
        {
        protected:
            // These settings are array to realize different slice amount for each data.
            vector<vector<int64_t>> start_;
            vector<vector<int64_t>> stop_;
            vector<vector<int64_t>> step_;

            int base_axis_;

            // SPECIAL condition
            enum { SLICE_NONE = 0x7fffffff };

        public:
            Slice(const Context& ctx, const vector<int64_t>& start, const vector<int64_t>& stop, const vector<int64_t>& step)
                : BaseFunction(ctx, start, stop, step)
                , start_(1)
                , stop_(1)
                , step_(1)
                , base_axis_(0)
            {
                start_[0] = start;
                stop_[0] = stop;
                step_[0] = step;
            }


            virtual ~Slice() {}
            

            virtual shared_ptr<Function> copy() const 
            {
                return create_Slice(ctx_, start_[0], stop_[0], step_[0]);
            }


            virtual vector<dtypes> in_types() 
            {
                return vector<dtypes>{get_dtype<T>(), get_dtype<T>()};
            }


            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Slice"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }


        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);
            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);

            //NBLA_API bool skip_check(const NdArrays& outputs); // ver19.0に実装がない
        
        private:
            NBLA_API void slice_execute_recursive(const NdArrayPtr inp, NdArrayPtr outp,
                                                  const T* x, T* y, 
                                                  int x_offset, int y_offset, 
                                                  int dim, int& slice_index);
        };
    }
}