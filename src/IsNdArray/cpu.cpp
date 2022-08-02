#include <IsNdArray/cpu.hpp>
#include <IsNdArray/singleton_manager-internal.hpp>
#include <IsNdArray/memory/caching_allocator_with_buckets.hpp>
#include <IsNdArray/memory/cpu_memory.hpp>
#include <IsNdArray/memory/naive_allocator.hpp>

namespace is
{
    namespace nbla
    {
        Cpu::Cpu()
			: native_allocator_(std::make_shared<NaiveAllocator<CpuMemory>>())
			, caching_allocator_(std::make_shared<CachingAllocatorWithBuckets<CpuMemory>>())
		{}


        Cpu::~Cpu() {}


        vector<string> Cpu::array_classes() const { return array_classes_; }
        void Cpu::_set_array_classes(const vector<string>& a) { array_classes_ = a; }
        void Cpu::register_array_class(const string& name) { array_classes_.push_back(name); }


        shared_ptr<Allocator> Cpu::caching_allocator() { return caching_allocator_; }
		shared_ptr<Allocator> Cpu::native_allocator() { return native_allocator_; }


        void Cpu::free_unused_host_caches()
        {
            caching_allocator_->free_unused_caches();
            native_allocator_->free_unused_caches();
        }


        void Cpu::device_synchronize(const string &device)
        {
            cpu_device_synchronize(device); // init.hpp
        }


        void Cpu::default_stream_synchronize(const string &device) {}


        // dll export for template
        NBLA_INSTANTIATE_SINGLETON(NBLA_API, Cpu)
    }
}