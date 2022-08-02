#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Sort, int, bool, bool, bool)
        // axis
        // reverse
        // with_index
        // only_index


        /**
         * @brief Sorts the elements of `x` along a given `axis` in ascending order by value.
         * 
         * A negative `axis` counts from the last dimension of `x`, so the default of -1 sorts along the last dimension.
         * If `reverse` is True, then the elements are soreted in descending order. 
         * 
         * If `with_index` is True, result is a tuple ``(sorted, indices)`` or only ``indices`` if `only_index` is True.
         * Setting `only_index` to True implies that `with_index` is also True.
         * 
         * 
         * Inputs:
         * - N-D array
         * 
         * Outputs:
         * - one or two N-D arrays
         * 
         * @param T Data type for computation.
         * @param axis Axis along which to sort.
         * @param reverse Sort in descending order.
         * @param with_index Return sorted values and index.
         * @param only_index Return only the sort index.
         */
        template <typename T>
        class Sort : public BaseFunction<int, bool, bool, bool>
        {
        protected:
            int axis_;
            bool reverse_;
            bool with_index_;
            bool only_index_;
            size_t inner_size_;
            size_t outer_size_;
            size_t total_size_;
            NdArray sort_index_;
            NdArray temp_index_;
        
        public:
            Sort(const Context& ctx, int axis, bool reverse, bool with_index, bool only_index)
                : BaseFunction(ctx, axis, reverse, with_index, only_index)
                , axis_(axis)
                , reverse_(reverse)
                , with_index_(with_index)
                , only_index_(only_index)
            {}

            virtual ~Sort() {}

            virtual shared_ptr<Function> copy() const
            {
                return create_Sort(ctx_, axis_, reverse_, with_index_, only_index_);
            }

            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual string name() { return "Sort"; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}