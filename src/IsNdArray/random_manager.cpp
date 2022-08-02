#include <IsNdArray/random_manager.hpp>
#include <IsNdArray/singleton_manager-internal.hpp>

namespace is
{
    namespace nbla
    {
        RandomManager::RandomManager()
            : seed_(313)
            , count_(0)
        {
            rgen_ = std::mt19937(seed_);
        }


        RandomManager::~RandomManager() {}


        std::mt19937& RandomManager::get_rand_generator() { return rgen_; }


        unsigned int RandomManager::get_seed() const { return seed_; }


        void RandomManager::set_seed(unsigned int seed)
        {
            seed_ = seed;
            rgen_ = std::mt19937(seed);
            ++count_;
        }


        int RandomManager::get_count() const { return count_; }


        NBLA_INSTANTIATE_SINGLETON(NBLA_API, RandomManager)
    }
}