#include <IsNdArray/memory/allocator.hpp>

#include <iostream>


namespace is
{
    namespace nbla
    {
        /* =============================== Allocator =============================== */
        Allocator::Allocator() {}
        Allocator::~Allocator() {}


        AllocatorMemory Allocator::alloc(size_t bytes, const string& device_id)
        {
            std::lock_guard<std::mutex> locker(mtx_);

            // 少なくとも1byteを確保する. これが効率がいい
            bytes = std::max(bytes, (size_t)1);
            auto memory = this->alloc_impl(bytes, device_id); // ここで実際にMemoryクラスをインスタンス化

            device_memory_used_in_bytes_.insert({
                device_id, (size_t)0 // insert if not exists.
            });

            if (callback_)
            {
                callback_->on_alloc(memory->bytes(), memory->device_id());
            }

            // 注意) Allocator is always instantiated as a shared_ptr.
            // メンバ関数の中で自身thisのshared_ptr<>を呼ぶ. 事前にshared_ptr<Allocator>が生成されていないと, 
            // std::bat_weak_ptr例外が発生する.
            return AllocatorMemory(memory, shared_from_this()); // AllocatorMmeoryのムーブが発生
        }


        void Allocator::free(shared_ptr<Memory> memory)
        {
            std::lock_guard<std::mutex> locker(mtx_);

            size_t bytes = memory->bytes();
            string device_id = memory->device_id();
            this->free_impl(memory); // ここでMemoryクラスのインスタンスを解放

            if (callback_)
            {
                callback_->on_free(bytes, device_id);
            }
        }


        size_t Allocator::free_unused_caches()
        {
            size_t freed_bytes = 0;
            for (auto& i : device_memory_used_in_bytes_)
            {
                freed_bytes += free_unused_device_caches(i.first);
            }
            return freed_bytes;
        }


        size_t Allocator::free_unused_device_caches(const string& device_id)
        {
            size_t freed_bytes = this->free_unused_device_caches_impl(device_id);
            device_memory_used_in_bytes_[device_id] -= freed_bytes;
            
            if (callback_)
            {
                callback_->on_free_unused_device_caches(device_id, freed_bytes);
            }
            return freed_bytes;
        }


        size_t Allocator::device_memory_used_in_bytes(const string& device_id)
        {
            if (!device_memory_used_in_bytes_.contains(device_id))
            {
                return 0;
            }
            return device_memory_used_in_bytes_[device_id];
        }


        void Allocator::alloc_retry(shared_ptr<Memory> memory)
        {
            /*
                メモリ確保を試みる. もし例外(std::bad_alloc)が発生したら
                利用できるすべてのメモリを解放して, 再確保を試みる.
            */
            try
            {
                memory->alloc(); // Memoryインスタンスのbytesサイズと同じ数だけストレージを確保
            }
            catch (...)
            {
                std::cerr << "Failed to allocate. Freeing memory cache and retrying." << std::endl;
                if (callback_)
                {
                    callback_->on_allocation_try_failure();
                }

                // 解放
                free_unused_device_caches(memory->device_id());

                try
                {
                    memory->alloc();
                }
                catch (...)
                {
                    std::cerr << "Failed to allocate again." << std::endl;
                    if (callback_)
                    {
                        callback_->on_allocation_retry_failure();
                    }
                    throw; // 通常はメモリの再確保に失敗するとプログラムを終了させる
                }
            }

            device_memory_used_in_bytes_[memory->device_id()] += memory->bytes();
        }


        /* =============================== AllocatorMemory =============================== */

        AllocatorMemory::AllocatorMemory(shared_ptr<Memory> memory, shared_ptr<Allocator> allocator)
            : memory_(memory), allocator_(allocator)
        {
            // ここで初めて確保したストレージをロック
            memory->lock();
        }

        AllocatorMemory::AllocatorMemory()
            : memory_(nullptr), allocator_(nullptr)
        {}


        void AllocatorMemory::release()
        {
            if (!memory_)
            {
                return;
            }

            memory_->release(); // ロック解除

            // ここで初めて確保したストレージを解放. ただし，NativeAllocatorの場合，
            // 解放されず，Memory(CpuMemory)インスタンスの開放時に自動的に解放される
            allocator_->free(memory_); 

            // ここでshared_ptr<Memory>が無所有になるので，
            // CpuMemory(内部free(ptr_)) -> Memoryの順番でデストラクタが呼ばれる.
            memory_ = nullptr;
        }

        AllocatorMemory::~AllocatorMemory() 
        {
            release();
        }

        AllocatorMemory::AllocatorMemory(AllocatorMemory&& rhs)
        {
            *this = std::move(rhs);
        }
    
        AllocatorMemory& AllocatorMemory::operator=(AllocatorMemory&& rhs)
        {
            release();
            memory_ = rhs.memory_;
            allocator_ = rhs.allocator_;
            rhs.memory_ = nullptr; // 二重deleteを避ける
            return *this;
        }
    }
}
