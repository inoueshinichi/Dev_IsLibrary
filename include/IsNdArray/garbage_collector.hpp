#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/singleton_manager.hpp>

#include <functional>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::vector;

        /**
         * @brief Singleton for garbage collector registry.
         * 
         * This singleton class is intended to be used to freeing cpu/device memory
         * held by an interpreter language which manages memory by GC (e.g. Python).
         * GC function as c++ callback function can be registered via `register_collector`
         * function. All registered GC function is called usually in case memory allocation fails.
         * 
         */
        class NBLA_API GarbageCollector
        {
        public:
            using collector_type = std::function<void()>;

            // void()型の関数としてGC関数を登録する
            void register_collector(collector_type func);

            // 登録済みのGC関数を呼び出す
            void collect();

            ~GarbageCollector();

        private:
            friend SingletonManager;
            vector<collector_type> collectors_;
            GarbageCollector();
        };
    }
}