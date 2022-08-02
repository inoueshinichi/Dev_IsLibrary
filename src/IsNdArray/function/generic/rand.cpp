#include <IsNdArray/array.hpp>
#include <IsNdArray/function/rand.hpp>
#include <IsNdArray/random_manager.hpp>
#include <IsNdArray/nd_array.hpp>

#include <random>


namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Rand, float, float, const vector<int64_t>&, int)
        // low
        // high
        // shape
        // seed

        template <typename T>
        void Rand<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs) 
        {
            outputs[0]->reshape(Shape_t(shape_.cbegin(), shape_.cend()), true);
            rgen_ = std::mt19937((seed_ == -1 ? std::random_device()() : seed_));
        }


        template <typename T>
        void Rand<T>::setup_recompute_impl(const NdArrays& inputs,
                                           const NdArrays& outputs) 
        {
            save_rng_ = true;
        }


        template <typename T>
        void Rand<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs) 
        {
            std::uniform_real_distribution<typename force_float<T>::type> rdist(low_, high_);
            std::mt19937& rgen = seed_ == -1 ? SingletonManager::get<RandomManager>()->get_rand_generator() : rgen_;
            // Remember the random state for recomputation.
            if (save_rng_) 
            {
                rgen_for_recompute_ = rgen;
            }

            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int s = 0; s < outputs[0]->size(); s++) 
            {
                y[s] = (T)rdist(rgen);
            }
        }


        template <typename T>
        void Rand<T>::recompute_impl(const NdArrays& inputs, const NdArrays& outputs) 
        {
            std::uniform_real_distribution<typename force_float<T>::type> rdist(low_, high_);
            auto rgen = rgen_for_recompute_;

            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int s = 0; s < outputs[0]->size(); s++) 
            {
                y[s] = rdist(rgen);
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        // NBLA_INSTANTIATE_CLASS(Rand, bool)
        // NBLA_INSTANTIATE_CLASS(Rand, char)
        // NBLA_INSTANTIATE_CLASS(Rand, unsigned char)
        // NBLA_INSTANTIATE_CLASS(Rand, short)
        // NBLA_INSTANTIATE_CLASS(Rand, unsigned short)
        // NBLA_INSTANTIATE_CLASS(Rand, int)
        // NBLA_INSTANTIATE_CLASS(Rand, unsigned int)
        // NBLA_INSTANTIATE_CLASS(Rand, long)
        // NBLA_INSTANTIATE_CLASS(Rand, unsigned long)
        // NBLA_INSTANTIATE_CLASS(Rand, long long)
        // NBLA_INSTANTIATE_CLASS(Rand, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Rand, float)
        NBLA_INSTANTIATE_CLASS(Rand, double)
        NBLA_INSTANTIATE_CLASS(Rand, long double)
    }
}