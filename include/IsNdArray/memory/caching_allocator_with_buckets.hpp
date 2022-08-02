#pragma once

#include <IsNdArray/memory/allocator.hpp>

#include <map>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace is
{
    namespace nbla
    {
        using std::map;
		using std::unordered_map;
		using std::tuple;
		using std::make_shared;

        /** CachingAllocatorWithBucketsの基礎
		 * これはキャッシュロジックを実装しますが、
		 * Memoryクラスのインスタンス化を仮想関数CachingAllocatorWithBucketsBase::make_memory_implとして残します.
		 * これにより CpuMemoryやCudaMemoryなどの任意のメモリクラスの実装を持ったアロケータの実現を容易にする.
		 * 以下，キャッシングアルゴリズムの記述
		 *
		 * 1) 以前に要求されたメモリをメモリプールにキャッシングする(借りる)
		 *
		 * このアロケータは，要求されたメモリ構成から以前作成したメモリインスタンスへのマップとしてメモリプールを維持する.
		 * 作成されたメモリブロックは，割当てや解放無しで再利用される. これにより，メモリの割当てと割当て解除及びCUDAで
		 * デバイス実行キューの暗黙的な同期などによるオーバーヘッドが大幅に削減される.
		 *
		 *
		 * 2) メモリプールに依存するサイズ
		 *
		 * メモリプールは，それぞれ小さいサイズと大きいサイズのメモリ用の2つの別個のプールとして維持される.
		 * デフォルトでは，1MBより小さなメモリサイズは小さいブロックとして，その他は大きなブロックとみなす.
		 *
		 *
		 * 3) メモリサイズの丸め規則
		 *
		 * 要求されたメモリサイズは，小さいブロックまたは大きいブロックに対して，それぞれround_small_(Default 512B)
		 * またはround_large_(Default 128KB)の倍数に丸められる.
		 *
		 *
		 * 4) 生成ルール
		 *
		 * 要求されたサイズより大きな，以前に作成されたメモリブロックのいずれかが見つからない場合，
		 * 新しいメモリインスタンスが作成される. 見つかった場合，次の分割ルールを適用した後，
		 * 最小サイズのメモリブロックが使用される.
		 *
		 * 5) 分割ルール
		 *
		 * 見つかったメモリブロックのサイズがround_small_(Default 512B)またはsmall_alloc_(Default 1MB)以上の場合
		 * 見つかったメモリブロックは，要求されたオブセット位置で2つに分割される. 丸め後のサイズの場合，2番目のサイズが
		 * プールに戻され，最初のサイズが使用される.
		 *
		 * @sa CachingAllocatorWithBuckets
		 */
        // インターフェースクラス
		class NBLA_API CachingAllocatorWithBucketsBase : public Allocator
		{
		public:
			using Key = tuple<size_t, Memory*>;
			using DeviceCacheMap = map<Key, shared_ptr<Memory>>;
			using CacheMap = unordered_map<string, DeviceCacheMap>;

		private:
			CacheMap small_cache_map_;
			CacheMap large_cache_map_;
			MemCountMap small_memory_counter_;
			MemCountMap large_memory_counter_;
			static constexpr int round_small_ = 512;       // 512B
			static constexpr int round_large_ = 128 << 10; // 128KB
			static constexpr int small_alloc_ = 1 << 20;   // 1MB

			size_t round_size(size_t bytes) const;

			void free_impl(shared_ptr<Memory> memory) override;

			shared_ptr<Memory> alloc_impl(size_t orig_bytes, const string& device_id) override;

			size_t free_unused_device_caches_impl(const string& device_id) override;

			void print_memory_cache_map_impl() override;
			
			size_t get_max_cache_bytes(const string& device_id);

			size_t get_total_cache_bytes(const string& device_id);

		protected:
			/**
			 * 与えられた構成のメモリを作成.
			 * もしメモリプールに再利用可能なメモリがなければ，alloc_impl()から呼び出される
			 */
			shared_ptr<Memory> make_memory(size_t bytes, const string& device_id);

			/**
			 * CachingAllocatorWithBucketsは，メモリクラステンプレートで実装する.
			 */
			virtual shared_ptr<Memory> make_memory_impl(size_t bytes, const string& device_id) = 0;

		public:
			// コンストラクタ
			CachingAllocatorWithBucketsBase();


			size_t get_fragmentation_bytes(const string &device_id) override;

			size_t get_max_available_bytes(const string &device_id) override;

			vector<int> get_used_memory_counts(const string &device_id) override;
		};


        /**
		 * CachingAllocatorWithBucketsの実装.
		 * MemryType : CpuMemory, CudaMemoryのような実装クラスの型
		 */
		template <typename MemoryType>
		class CachingAllocatorWithBuckets : public CachingAllocatorWithBucketsBase
		{
			using memory_type = MemoryType;

			shared_ptr<Memory> make_memory_impl(size_t bytes, const string& device_id) override
			{
				return make_shared<memory_type>(bytes, device_id);
			}

		public:
			CachingAllocatorWithBuckets()
				: CachingAllocatorWithBucketsBase()
			{
#if 0
				this->callback_.reset(new PrintingAllocatorCallback(
							typeid(CachingAllocatorWithBuckets<memory_type>).name()));
#endif
			}
		};
    }
}