#pragma once

#include <IsNdArray/memory/allocator.hpp>

#include <map>
#include <tuple>
#include <typeinfo>
#include <unordered_map>

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
#endif

namespace is
{
    namespace nbla
    {
        /**
         * キャッシュなしのMemoryアロケータ.
         * このクラスはいつもアロケーション(Memory::alloc)で新しいMemoryインスタンスを作る.
         * そして，戻ったときfreeする.
         *
         * メモリタイプ: CpuMemory and CudaMemory
         *
         */
        template <typename MemoryType>
        class NaiveAllocator : public Allocator
        {
        public:
			using memory_type = MemoryType;


            NaiveAllocator() : Allocator()
			{
            #if 1
                #if __has_include(<cxxabi.h>)
                    std::string allocator_name;                                                
                    const std::type_info& id = typeid(*this); /* RTTI */                   
                    int stat{-1};                                                              
                    char* name = abi::__cxa_demangle(id.name(), 0, 0, &stat);                  
                    if (name != nullptr)                                                       
                    {                                                                          
                        if (stat == 0) /* success: stat == 0 */                                
                        {                                                                      
                            allocator_name = name;                                             
                            std::cout << allocator_name << std::endl;
                                callback_.reset(new PrintingAllocatorCallback(allocator_name));                                     
                            ::free(name);                                                      
                        }                                                                      
                    }                                                                          
                    if (stat != 0)                                                            
                    {                                                                          
                        std::cout << typeid(*this).name() << std::endl;
                            callback_.reset(new PrintingAllocatorCallback(typeid(*this).name()));                                             \
                    }
                #else
                    std::cout << typeid(*this).name() << std::endl;
                            callback_.reset(new PrintingAllocatorCallback(typeid(*this).name()));
                #endif
            #endif
			}


        private:
			shared_ptr<Memory> alloc_impl(size_t orig_bytes, const string& device_id) override 
			{
				// いつも新しいメモリを確保する
				auto mem = std::make_shared<memory_type>(orig_bytes, device_id);
				this->alloc_retry(mem);
				return mem;
			}


            void free_impl(shared_ptr<Memory> memory) override
			{
				// NaiveAllocatorの場合，何もしない
				// Memory(CpuMemory, CudaMemory)のデストラクタでストレージを解放する
			}


            size_t free_unused_device_caches_impl(const string& device_id) override
			{
				// NaiveAllocatorの場合，何もしない
				return 0;
			}
        };
    }
}