// #pragma once

// #include <chrono>
// #include <map>
// #include <unordered_map>
// #include <memory>
// #include <queue>

// #include <IsNdArray/backend_registry.hpp>
// #include <IsNdArray/global_context.hpp>
// #include <IsNdArray/memory/allocator.hpp>

// namespace Is
// {
//     namespace nbla
//     {
//         using std::queue;
//         using std::make_shared;
//         using std::unordered_map;
//         using std::multimap;

//         using Tp = std::chrono::time_point<std::chrono::high_resolution_clock>;

//         /**
//          * @brief VirtualCachingAllocatorの基底クラス
//          * 
//          * 
//          * 
//          */
//         class NBLA_API VirtualCachingAllocatorBase : public Allocator
//         {
//         public:
//             // メモリキャッシュを実現する型
//             using PhysicalMemoryCache = queue<PhysicalMemoryPtr>;
//             using PhysicalMemoryCacheMap = unorderd_map<string, PhysicalMemoryCache>;

//             using MemPtrWithTime = pair<Tp, shared_ptr<Memory>>;
//             using MappedCache = multimap<size_t, MemPtrWithTime>;
//             using Wlist = std::priority_queue<MemPtwWithTime, std::vector<MemPtrWithTime>, std::greater<MemPtrWithTime>>;


//         };
//     }
// }