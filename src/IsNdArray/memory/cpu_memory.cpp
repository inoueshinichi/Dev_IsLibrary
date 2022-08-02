
#include <IsNdArray/exception.hpp>
#include <IsNdArray/memory/cpu_memory.hpp>

#include <memory>

#if 0
#   include <cstdio>
#   define DEBUG_LOG(...) printf(__VA_ARGS__);
#else
#   define DEBUG_LOG(...)
#endif

namespace is
{
    namespace nbla
    {
        using std::make_shared;

        CpuMemory::CpuMemory(size_t bytes, const string& device_id, void* ptr)
            : Memory(bytes, device_id)
        {
            ptr_ = ptr;
        }

        CpuMemory::CpuMemory(size_t bytes, const string& device_id)
            : Memory(bytes, device_id)
        {}

        CpuMemory::~CpuMemory()
        {
            if (!ptr_)
            {
                return;
            }

            NBLA_FORCE_ASSERT(!prev(),
                            "Trying to free memory which has a prev (allocated "
                            "by another memory and split previously).");
            
            DEBUG_LOG("%s: %zu at %p\n", __func__, bytes(), ptr_);
            ::free(ptr_);
        }

        bool CpuMemory::alloc_impl()
        {
            ptr_ = ::malloc(bytes());
            DEBUG_LOG("%s: %zu at %p\n", __func__, bytes(), ptr_);
            return bool(ptr_);
        }

        shared_ptr<Memory> CpuMemory::divide_impl(size_t second_start)
        {
            size_t out_bytes = bytes() - second_start;
            void* out_ptr = (void*)( (uint8_t*)ptr_+ second_start );
            return shared_ptr<CpuMemory>(new CpuMemory(out_bytes, device_id(), out_ptr)); // shared_ptr<CpuMemory> -> shared_ptr<Memory>
        }

        void CpuMemory::merge_next_impl(Memory* from)
        {
            // this->ptr_　は既にマージされたメモリブロックの初期位置を指している
        }

        void CpuMemory::merge_prev_impl(Memory* from)
        {
            // マージされたメモリブロックの開始位置を表すthis->ptr_として開始ポインタを使用する
            ptr_ = from->pointer();
        }
    } // namespace nbla
}