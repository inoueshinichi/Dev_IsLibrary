#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/singleton_manager.hpp>


#include <random>


namespace is
{
    namespace nbla
    {
        /**
         * @brief Singleton class for storing global context.
         * 
         */
        class NBLA_API RandomManager
        {
            std::mt19937 rgen_;
            unsigned int seed_;
            int count_;

        public:
            ~RandomManager();

            std::mt19937& get_rand_generator();

            unsigned int get_seed() const;

            void set_seed(unsigned int seed);

            // seedが設定された回数を取得
            int get_count() const;

        private:
            friend SingletonManager;
            // Never called by users.
            RandomManager();
            DISABLE_COPY_AND_ASSIGN(RandomManager)
        };
    }
}