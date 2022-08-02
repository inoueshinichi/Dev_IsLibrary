#include <IsNdArray/memory/caching_allocator_with_buckets.hpp>

#define DEBUG_DEVICE_CACHES

#if defined(DEBUG_DEVICE_CACHES)
    #include <cstdio>
    #define DEBUG_LOG(...) std::printf(__VA_ARGS__)
    #define DEBUG_PRINT_CACHES(M, S) print_device_cache_map(M, S)
#else
    #define DEBUG_LOG(...)
    #define DEBUG_PRINT_CACHES(M, S)
#endif

namespace is
{
    namespace nbla
    {
        // コンストラクタ
		CachingAllocatorWithBucketsBase::CachingAllocatorWithBucketsBase() {}


        // -------------------------------------------------------------------------------
		// インライン　ローカル関数
		// -------------------------------------------------------------------------------

		// Memory*からKeyを作成
		inline CachingAllocatorWithBucketsBase::Key create_key_by_memory(Memory* memory)
		{
			return CachingAllocatorWithBucketsBase::Key{ memory->bytes(), memory };
		}


        // DeviceCacheMap内の無効になったMemoryを削除
		inline void try_erase_cache(CachingAllocatorWithBucketsBase::DeviceCacheMap& device_cache_map, 
                                    Memory* memory)
		{
			/*
			  メモリインスタンスが他とマージされたならば、キャッシュから消す
			  (i.e., disabled)
			*/
			if (!memory || !memory->disabled())
			{
				return;
			}

			auto key = create_key_by_memory(memory);
			device_cache_map.erase(key); // std::shared_ptr<Memory>がどこも参照していないことになる.
		}


        // 指定したdevice_idにおけるDeviceCacheMapを取得
		inline CachingAllocatorWithBucketsBase::DeviceCacheMap&
			get_device_cache_map(CachingAllocatorWithBucketsBase::CacheMap& m, const string& device_id)
		{
			auto iter = m.find(device_id);
			if (iter == m.end())
			{
				// 新しいDeviceCacheMapを作る
				iter = m.emplace(device_id, CachingAllocatorWithBucketsBase::DeviceCacheMap()).first; // m.emplace()の戻り値は, pair<iterator, bool>
			}
			return iter->second;
		}


        // 指定したdevice_idにおけるDeviceCacheMapの中で使用していないキャッシュを破棄する
		inline size_t free_unused_device_caches_in_map(
			CachingAllocatorWithBucketsBase::CacheMap& cache_map,
			const string& device_id)
		{

			auto& device_cache_map = get_device_cache_map(cache_map, device_id);
			auto it = device_cache_map.begin();
			auto end = device_cache_map.end();
			size_t total_freed_bytes{ 0 };

			while (it != end) 
			{
                // divideされていないMemoryインスタンスが所有するバイト数
				size_t freed_bytes = it->second->bytes_active(); 
				if (freed_bytes)
				{
					total_freed_bytes += freed_bytes;
					// Memoryは, インスタンスのデストラクタでメモリ解放を行う
					it = device_cache_map.erase(it);
					continue;
				}
				++it;
			}
			return total_freed_bytes;
		}


        // -------------------------------------------------------------------------------
		// ローカル関数
		// -------------------------------------------------------------------------------

		// 要求bytes数を丸める
		size_t CachingAllocatorWithBucketsBase::round_size(size_t bytes) const
        {

			if (bytes < round_small_)
			{
				bytes = round_small_; // 要求されるメモリサイズの最小値はround_small_.
			}
			else if (bytes < small_alloc_)
			{
				// round_small_の整数倍(=要求されたbytes + スペーサーbytes)に丸める
				bytes = ((bytes + round_small_ - 1) / round_small_) * round_small_;
			}
			else
			{
				// round_large_の整数倍に丸める
				bytes = ((bytes + round_large_ - 1) / round_large_) * round_large_;
			}

			return bytes;
		}

#if defined(DEBUG_DEVICE_CACHES)
        static void print_device_cache_map(const CachingAllocatorWithBucketsBase::DeviceCacheMap& m, bool small) 
        {
            std::vector<size_t> sizes;
            for (auto& i : m)
            {
                sizes.push_back(i.second->bytes()); // memory bytes
            }
            printf("device_cache_map(%d): [%s]\n", (int)small, string_join(sizes, ", ").c_str());
        }
#endif


        // -------------------------------------------------------------------------------
		// Override メンバー関数
		// -------------------------------------------------------------------------------

		shared_ptr<Memory> CachingAllocatorWithBucketsBase::alloc_impl(size_t orig_bytes, 
                                                                       const string& device_id)
		{
			/*
				要求されたバイト数に応じて大メモリサイズor小メモリサイズに分けれれたキャッシュ集合
				から丸められた最小のバイト数を持つMemoryを見つける.

				もしキャッシュにMemoryがない場合、新しいMemoryを作成することを試みる.

				発見/生成 されたメモ入りは

			*/

			// Rounding memory block size
			size_t bytes = round_size(orig_bytes);
			bool small = bytes <= small_alloc_;

			// キャッシュからメモリブロックサイズを見つけるor生成
			auto& cache_map = small ? small_cache_map_ : large_cache_map_;
			auto& device_cache_map = get_device_cache_map(cache_map, device_id);
			DEBUG_PRINT_CACHES(device_cache_map, small);
			Key key{ bytes, nullptr };
			shared_ptr<Memory> mem;
            
            // キャッシュを検索(丸めたバイト数より大きい最も最小なバイト数を持つMemoryインスタンスを取得)
			auto iter = device_cache_map.lower_bound(key); 

			if (iter != device_cache_map.end())
			{
				// キャッシュに要求サイズに合致するMemoryを見つけた
				mem = iter->second;
				device_cache_map.erase(iter);
				DEBUG_LOG("Found: %zu\n", mem->bytes());
			}
			else
			{
				// 新たにMemoryを生成
				size_t alloc_bytes = small ? small_alloc_ : bytes;
				mem = this->make_memory(alloc_bytes, device_id);
				DEBUG_LOG("Alloc: %zu\n", alloc_bytes);
				alloc_retry(mem); // ここで実メモリがストレージ上に確保される
			}

			// 確保したメモリサイズが大きすぎる場合、メモリを分割
			if (mem->bytes() - bytes >= (small ? round_small_ : small_alloc_ + 1))
			{
				DEBUG_LOG("Split (%d): %zu at %zu\n", (int)small, mem->bytes(), bytes);
				shared_ptr<Memory> remaining = mem->divide(bytes);
				device_cache_map[create_key_by_memory(remaining.get())] = remaining;
			}
			DEBUG_PRINT_CACHES(device_cache_map, small);
			return mem;
		}


        // Memoryインスタンスを生成してポインタを返す
		shared_ptr<Memory> CachingAllocatorWithBucketsBase::make_memory(size_t bytes, 
                                                                        const string& device_id)
		{
			return this->make_memory_impl(bytes, device_id);
		}


        void CachingAllocatorWithBucketsBase::free_impl(shared_ptr<Memory> memory)
		{
			/*
				引数のMemoryをキャッシュ集合に戻す.
				もしmemoryのnextとprevが使用されていない(locked)なら、マージする
			*/

			bool small = memory->bytes() <= small_alloc_;
			auto& cache_map = small ? small_cache_map_ : large_cache_map_;
			auto& device_cache_map = get_device_cache_map(cache_map, memory->device_id());

			// Try to merge prev and next
			Memory* n = memory->next();
			memory->try_merge(n);
			Memory* p = memory->prev();
			memory->try_merge(p);
			try_erase_cache(device_cache_map, n);
			try_erase_cache(device_cache_map, p);

			// Cache
			device_cache_map[create_key_by_memory(memory.get())] = memory;
			DEBUG_LOG("cache_impl\n");
			DEBUG_PRINT_CACHES(device_cache_map, small);
		}


        size_t CachingAllocatorWithBucketsBase::free_unused_device_caches_impl(const string& device_id)
		{
			/*
				指定のデバイスから未使用のキャッシュをすべて削除
			*/
			size_t freed_bytes{ 0 };
			freed_bytes += free_unused_device_caches_in_map(small_cache_map_, device_id);
			freed_bytes += free_unused_device_caches_in_map(large_cache_map_, device_id);
			return freed_bytes;
		}


		void CachingAllocatorWithBucketsBase::print_memory_cache_map_impl() 
		{
			auto print_func = [&](const CacheMap &m, const string &map_type) {
				for (auto &p1 : m) {
				vector<string> sz;
				unsigned long long ss = 0;

				string d_id = p1.first;
				for (auto &p2 : p1.second) {
					size_t s = p2.second->bytes();
					ss += s;
					sz.push_back(byte_to_human_readable(s));
				}

				size_t used = device_memory_used_in_bytes(d_id);

				printf("cache_map(device_id: %s, mem_type: %s, used: %s, free: %s): \n "
						"[%s]\n\n",
						d_id.c_str(), map_type.c_str(),
						byte_to_human_readable(used).c_str(),
						byte_to_human_readable(ss).c_str(), string_join(sz, ", ").c_str());
				}
			};

			// small
			print_func(small_cache_map_, "small");

			// large
			print_func(large_cache_map_, "large");
		}


		size_t CachingAllocatorWithBucketsBase::get_max_cache_bytes(const string &device_id) 
		{
			size_t max_bytes = 0;

			// small
			for (auto &p : small_cache_map_[device_id]) {
				max_bytes = std::max(max_bytes, p.second->bytes());
			}

			// large
			for (auto &p : large_cache_map_[device_id]) {
				max_bytes = std::max(max_bytes, p.second->bytes());
			}

			return max_bytes;
		}


		size_t CachingAllocatorWithBucketsBase::get_total_cache_bytes(const std::string &device_id) 
		{
			size_t total_bytes = 0;

			// small
			for (auto &p : small_cache_map_[device_id]) {
				total_bytes += p.second->bytes();
			}

			// large
			for (auto &p : large_cache_map_[device_id]) {
				total_bytes += p.second->bytes();
			}

			return total_bytes;
		}


		size_t CachingAllocatorWithBucketsBase::get_fragmentation_bytes(const std::string &device_id) 
		{
			return get_total_cache_bytes(device_id) - get_max_cache_bytes(device_id);
		}

		size_t CachingAllocatorWithBucketsBase::get_max_available_bytes(const string &device_id) 
		{
			return get_max_cache_bytes(device_id);
		}

		std::vector<int> CachingAllocatorWithBucketsBase::get_used_memory_counts(const std::string &device_id) 
		{
			// small
			auto small_cnt = small_memory_counter_[device_id];

			// large
			auto large_cnt = large_memory_counter_[device_id];

			return {small_cnt, large_cnt};
		}
    } // namespace nbla
}