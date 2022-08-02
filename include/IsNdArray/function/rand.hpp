#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <random>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Rand, float, float, const vector<int64_t>&, int)
        // low
        // high
        // shape
        // seed

        /**
         * @brief 
         * 
         */
        template <typename T>
        class Rand : public BaseFunction<float, float, const vector<int64_t>&, int>
        {
        protected:
            float low_;
            float high_;
            const vector<int64_t> shape_;
            int seed_;
            bool save_rng_ = false;
            std::mt19937 rgen_, rgen_for_recompute_;

        public:
            Rand(const Context& ctx, float low, float high, const vector<int64_t>& shape, int seed)
                : BaseFunction<float, float, const vector<int64_t>&, int>(ctx, low, high, shape, seed)
                , low_(low)
                , high_(high)
                , shape_(shape)
                , seed_(seed) 
            {
                NBLA_CHECK(high > low, 
                           error_code::value,
                           "`high` (%f given) must be larger than `low` (%f given).", 
                           high,
                           low);
            }


              virtual ~Rand() {}
            virtual shared_ptr<Function> copy() const 
            {
                return create_Rand(ctx_, low_, high_, shape_, seed_);
            }
            virtual vector<dtypes> in_types() { return vector<dtypes>(); }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Rand"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual bool need_setup_recompute(int o) const { return true; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);
            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);

            NBLA_API virtual void setup_recompute_impl(const NdArrays& inputs,
                                                       const NdArrays& outputs);
            NBLA_API virtual void recompute_impl(const NdArrays& inputs,
                                                const NdArrays& outputs);
        };
    }
}