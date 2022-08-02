#include "./cpu_array-internal.hpp"
#include <IsNdArray/array_registry.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/cpu.hpp>

#include <cstring> // memset
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::vector;
        using std::shared_ptr;
        using std::make_shared;

        CpuArray::CpuArray(const Size_t size, dtypes dtype, const Context& ctx)
            : Array::Array(size, dtype, ctx, 
                           SingletonManager::get<Cpu>()->native_allocator()->alloc(
                               Array::size_as_bytes(size, dtype), "")) 
        {}


        CpuArray::CpuArray(const Size_t size, dtypes dtype, const Context& ctx, AllocatorMemory&& mem)
            : Array::Array(size, dtype, ctx, std::move(mem))
        {}


        CpuArray::~CpuArray() {}


        void CpuArray::zero()
        {
            std::memset(this->pointer<void>(), 0, this->size() * sizeof_dtype(this->dtype_));
        }


        Context CpuArray::filter_context(const Context& ctx)
        {
            return Context({}, "CpuArray", "");
        }


        // void copy_from(const Array* src_array)の実装マクロ
        NBLA_DEFINE_FUNC_COPY_FROM(CpuArray, cpu_array_copy, cpu)

        // void fill(double value)の実装マクロ
        NBLA_DEFINE_FUNC_FILL(CpuArray, cpu_fill, cpu)


        /////////////////////////////////
        // CpuCachedArray implementation
        /////////////////////////////////
        CpuCachedArray::CpuCachedArray(const Size_t size, dtypes dtype, const Context& ctx)
            : CpuArray(size, dtype, ctx,
                       SingletonManager::get<Cpu>()->caching_allocator()->alloc(
                           Array::size_as_bytes(size, dtype), ""))
        {}


        CpuCachedArray::~CpuCachedArray() {}


        Context CpuCachedArray::filter_context(const Context& ctx)
        {
            return Context({}, "CpuCachedArray", "");
        }
    }
}