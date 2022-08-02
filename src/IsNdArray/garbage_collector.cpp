#include <IsNdArray/garbage_collector.hpp>
#include <IsNdArray/singleton_manager-internal.hpp>

#include <iostream>

namespace is
{
    namespace nbla
    {
        void GarbageCollector::register_collector(collector_type func)
        {
            collectors_.push_back(func);
        }


        void GarbageCollector::collect()
        {
            int i = 0;
            for (auto func : collectors_)
            {
                // TODO: Logger
                std::cout << "Calling gargabe collection" << i++ << std::endl;
                func();
            }
        }


        GarbageCollector::GarbageCollector() {}
        GarbageCollector::~GarbageCollector() {}

        NBLA_INSTANTIATE_SINGLETON(NBLA_API, GarbageCollector)
    }
}