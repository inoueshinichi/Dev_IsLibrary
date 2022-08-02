#include <IsNdArray/array.hpp>
#include <IsNdArray/function/randint.hpp>
#include <IsNdArray/random_manager.hpp>
#include <IsNdArray/nd_array.hpp>

#include <random>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Randint, int, int, const vector<int64_t> &, int)
        // low
        // high
        // shape
        // seed

        template <typename T>
        void Randint<T>::setup_impl(const NdArrays &inputs, const NdArrays &outputs)
        {
            outputs[0]->reshape(Shape_t(shape_.cbegin(), shape_.cend()), true);
            rgen_ = std::mt19937((seed_ == -1 ? std::random_device()() : seed_));
        }

        template <typename T>
        void Randint<T>::execute_impl(const NdArrays &inputs,
                                      const NdArrays &outputs)
        {
            // TODO: consider templating integer type.
            std::uniform_int_distribution<int> rdist(low_, high_ - 1);
            std::mt19937 &rgen =
                seed_ == -1 ? SingletonManager::get<RandomManager>()->get_rand_generator()
                            : rgen_;
            int *y = outputs[0]->cast_data_and_get_pointer<int>(this->ctx_, true);
            for (int s = 0; s < outputs[0]->size(); s++)
            {
                y[s] = (T)rdist(rgen);
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_CLASS(Randint, bool)
        NBLA_INSTANTIATE_CLASS(Randint, char)
        NBLA_INSTANTIATE_CLASS(Randint, unsigned char)
        NBLA_INSTANTIATE_CLASS(Randint, short)
        NBLA_INSTANTIATE_CLASS(Randint, unsigned short)
        NBLA_INSTANTIATE_CLASS(Randint, int)
        NBLA_INSTANTIATE_CLASS(Randint, unsigned int)
        NBLA_INSTANTIATE_CLASS(Randint, long)
        NBLA_INSTANTIATE_CLASS(Randint, unsigned long)
        NBLA_INSTANTIATE_CLASS(Randint, long long)
        NBLA_INSTANTIATE_CLASS(Randint, unsigned long long)
        NBLA_INSTANTIATE_CLASS(Randint, float)
        NBLA_INSTANTIATE_CLASS(Randint, double)
        NBLA_INSTANTIATE_CLASS(Randint, long double)
    }
}