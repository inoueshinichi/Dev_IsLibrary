#pragma once

#include <IsNdArray/memory/memory.hpp>

namespace is
{
    namespace nbla
    {
        class NBLA_API CpuMemory : public Memory
        {
            CpuMemory(size_t bytes, const string& device_id, void* ptr);

        public:
            CpuMemory(size_t bytes, const string& device_id);
            virtual ~CpuMemory();

        protected:
            virtual bool alloc_impl() override final;
            virtual shared_ptr<Memory> divide_impl(size_t second_start) override final;
            virtual void merge_next_impl(Memory* from) override final;
            virtual void merge_prev_impl(Memory* from) override final;
        };
    }
}