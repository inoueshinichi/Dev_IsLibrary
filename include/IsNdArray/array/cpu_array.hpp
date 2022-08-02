#pragma once

#include <IsNdArray/array.hpp>
#include <IsNdArray/memory/allocator.hpp>

namespace is
{
    namespace nbla
    {
        /**
         * @brief CPUデバイス用の配列
         * 
         */
        class NBLA_API CpuArray : public Array
        {
        public:
            CpuArray(const Size_t size, dtypes dtype, const Context& ctx);
            CpuArray(const Size_t size, dtypes dtype, const Context& ctx, AllocatorMemory&& mem);
            virtual ~CpuArray();
            virtual void copy_from(const Array* src_array); // NBLA_DEFINE_FUNC_COPY_FROMマクロで実装
            virtual void zero();
            virtual void fill(double value); // NBLA_DEFINE_FUNC_FILLマクロで実装
            static Context filter_context(const Context& ctx);
        };


        /**
         * @brief キャッシュ機能を搭載したCPUデバイス用の配列
         * 
         */
        class NBLA_API CpuCachedArray : public CpuArray
        {
        public:
            explicit CpuCachedArray(const Size_t size, dtypes dtype, const Context& ctx); // explicitはdtypesの暗黙の型変換を禁止する？
            virtual ~CpuCachedArray();
            static Context filter_context(const Context& ctx);
        };

    }
}