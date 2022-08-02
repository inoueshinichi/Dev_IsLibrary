
#include <IsNdArray/exception.hpp>
#include <IsNdArray/memory/allocator_callback.hpp>

#include <iostream>
#include <typeinfo>


namespace is
{
    namespace nbla
    {
        PrintingAllocatorCallback::PrintingAllocatorCallback(const string& name)
            : name_(name)
        {}

        void PrintingAllocatorCallback::on_alloc(size_t bytes, const string& device_id)
        {
            std::cout << format_string(
                "PrintingAllocatorCallback::on_alloc(%s,%s) in %s.",
                std::to_string(bytes).c_str(), device_id.c_str(), name_.c_str())
            << std::endl;
        }

        void PrintingAllocatorCallback::on_free(size_t bytes, const string& device_id)
        {
            std::cout << format_string(
                "PrintingAllocatorCallback::on_free(%s,%s) in %s.",
                std::to_string(bytes).c_str(), device_id.c_str(), name_.c_str())
            << std::endl;
        }

        void PrintingAllocatorCallback::on_free_unused_device_caches(const string& device_id, size_t freed_bytes)
        {
            std::cout << format_string("PrintingAllocatorCallback::"
                "on_free_unused_device_caches(%s, %s) in %s.",
                device_id.c_str(), std::to_string(freed_bytes).c_str(), name_.c_str())
            << std::endl;
        }

        void PrintingAllocatorCallback::on_allocation_try_failure()
        {
            
            std::cerr << format_string(
                "PrintingAllocatorCallback::on_allocation_try_failure() in %s.",
                name_.c_str())
            << std::endl;
        }

        void PrintingAllocatorCallback::on_allocation_retry_failure()
        {
            std::cerr << format_string(
                "PrintingAllocatorCallback::on_allocation_retry_failure() in %s.",
                name_.c_str()) << std::endl;
        }
    }
}