#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#include <random>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Randint, int, int, const vector<int64_t>&, int)
        // low
        // high
        // shape
        // seed

        template <typename T>
        class Randint : public BaseFunction<int, int, const vector<int64_t> &, int>
        {
        protected:
            int low_;
            int high_;
            const vector<int64_t> shape_;
            int seed_;
            std::mt19937 rgen_;

        public:
            Randint(const Context& ctx, int low, int high, const vector<int64_t>& shape, int seed)
                : BaseFunction<int, int, const vector<int64_t>&, int>(ctx, low, high, shape, seed)
                , low_(low)
                , high_(high)
                , shape_(shape)
                , seed_(seed)
            {
                NBLA_CHECK(high > low, 
                            error_code::value,
                           "`high` (%d given) must be larger than `low` (%d given).", 
                           high,
                           low);
            }
            virtual ~Randint() {}
            virtual shared_ptr<Function> copy() const
            {
                return create_Randint(ctx_, low_, high_, shape_, seed_);
            }
            virtual vector<dtypes> in_types() { return vector<dtypes>{}; }
            virtual vector<dtypes> out_types()
            {
                return vector<dtypes>{get_dtype<int>()};
            }
            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Randint"; }
            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays &inputs,
                                             const NdArrays &outputs);
            NBLA_API virtual void execute_impl(const NdArrays &inputs,
                                               const NdArrays &outputs);
        
        };
    }
}